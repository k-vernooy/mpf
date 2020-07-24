/****************************************
 * @file                          gui.cpp
 * @author                      k-vernooy
 * 
 * Code for displaying a GUI, handling
 * events, and playing back audio via
 * SDL_Mixer
 ***************************************/

#include "../include/mpf.h"


static void GUI::SDL_DrawRoundedRect(SDL_Renderer* renderer, const SDL_Rect* r, const int br) {
    // SDL_SetRenderDrawColor(renderer,)
    SDL_RenderDrawRect(renderer, r);
}

void GUI::init() {
    if (isActive) return;

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
    SDL_SetRenderDrawColor(255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

void GUI::playAudio() {
    // resume `currentFile` at `fProgress`

}

void pauseAudio() {
    Mix_HaltMusic(-1);
}

void GUI::isPlayingAudio() {
    return (Mix_PlayingMusic() || musicPaused)
}


void GUI::renderFrame() {
    // clear renderer
    SDL_RenderClear(renderer);
    SDL_RenderPresent(render);
}


void GUI::beginGUI(MusicPlayer* player) {
    this->init();
    while (!isActive) {
        // render the current state
        renderFrame();

        // // handle all events
        // SDL_Event evt;
        // while (SDL_PollEvent(*evt)) {
            
        // }

        // // check if we need to start a new song
        // if (!isPlayingAudio()) {
        //     if (!advanceFile()) {
        //         cout << "Completed all files." << endl;
        //         isActive = false;
        //     }
        //     else {
        //         // begin playback of new audio file
        //     }
        // }

        // sleep FPS
        usleep(500);
    }

    // SDL_UpdateWindowSurface(window);
    SDL_DestroyWindow(window);
    SDL_Quit();

    cout << "App terminated successfully." << endl;
    return;
}