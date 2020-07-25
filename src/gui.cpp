/****************************************
 * @file                          gui.cpp
 * @author                      k-vernooy
 * 
 * Code for displaying a GUI, handling
 * events, and playing back audio via
 * SDL_Mixer
 ***************************************/

#include <iostream>
#include "../include/mpf.h"
#include "../include/util.h"

void GUI::SDL_DrawRoundedRect(SDL_Renderer* renderer, const SDL_Rect* r, const int br) {
    // SDL_SetRenderDrawColor(renderer,)
    SDL_RenderDrawRect(renderer, r);
}

void GUI::init() {
    if (isActive) return;
    int SCREEN_WIDTH = 500,
        SCREEN_HEIGHT = 500;

    // Initialize SDL video/audio
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        Err("fatal: SDL could not initialize; " + std::string(SDL_GetError()));
        exit(1);
    }

    // Initialize SDL_Mixer audio loading
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_OPUS) <= 0) {
        Err("fatal: SDL_Mixer could not initialize; " + std::string(Mix_GetError()));
        exit(1);
    }

    // Open the audio channel
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        Err("fatal: SDL_Mixer could not open a channel; " + std::string(Mix_GetError()));


    // create an SDL Window
    window = SDL_CreateWindow(
        (std::string("mpf - ") + files.getFile(0).getFileName()).c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        Err("fatal: SDL_Window could not be created; " + std::string(SDL_GetError()));
        exit(1);
    }

    // Fill the surface with nullptr
    screenSurface = SDL_GetWindowSurface(window);
    isActive = true;
}

void GUI::clear() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}


bool AudioApp::advanceFile() {
    currentFile++;
    return (currentFile != files.size());
}

// void GUI::playAudio() {
//     // resume `currentFile` at `fProgress`

// }

void AudioApp::pauseAudio() {
    musicPaused = true;
    Mix_HaltMusic();
}

bool AudioApp::isPlayingAudio() {
    return (Mix_PlayingMusic() || musicPaused);
}


void GUI::renderFrame() {
    // clear renderer
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}


void GUI::run() {
    files.applyFilter(config.getVariable("FILTER"));
    this->init();

    while (isActive) {
        // render the current state
        renderFrame();

        // handle all events
        SDL_Event evt;
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) {
                isActive = false;
            }
            if (evt.type == SDL_KEYDOWN) {
                switch (evt.key.keysym.sym) {
                    case SDLK_SPACE:
                        if (musicPaused) {
                            files.getFile(currentFile).beginPlayback(fProgress / 1000.0);
                            musicPaused = false;
                        }
                        else {
                            pauseAudio();
                        }
                        break;
                    case SDLK_LEFT:
                        fProgress -= 5000;
                        if (fProgress < 0) fProgress = 0;
                        Mix_SetMusicPosition(fProgress / 1000.0);
                        break;
                    case SDLK_RIGHT:
                        fProgress += 5000;
                        Mix_SetMusicPosition(fProgress / 1000.0);
                        break;
                }
            }
        }

        // check if we need to start a new song
        if (!isPlayingAudio()) {
            if (!advanceFile()) {
                Log("Completed all files.");
                isActive = false;
            }
            else {
                Log("Advanced to file " + files.getFile(currentFile).filePath);
                // begin playback of new audio file
                fProgress = 0;
                SDL_SetWindowTitle(window, files.getFile(currentFile).filePath.c_str());

                // begin playback
                files.getFile(currentFile).beginPlayback(0);
            }
        }

        if (!musicPaused)
            fProgress += 1000.0 / (double)FPS;
        SDL_Delay(1000.0 / FPS);
    }

    // SDL_UpdateWindowSurface(window);
    SDL_DestroyWindow(window);
    SDL_Quit();

    Log("App terminated successfully.");
    return;
}