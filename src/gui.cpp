/****************************************
 * @file                          gui.cpp
 * @author                      k-vernooy
 * 
 * Code for displaying a GUI, handling
 * events, and playing back audio via
 * SDL_Mixer
 ***************************************/

#include <iostream>
#include <SDL2/SDL_ttf.h>

#include "../include/mpf.h"
#include "../include/util.h"


void GUI::SDL_DrawCircle(SDL_Renderer* renderer, const GUI::SDL_Circle* c) {
    for (int w = 0; w < c->r * 2; w++) {
        for (int h = 0; h < c->r * 2; h++) {
            int dx = c->r - w;
            int dy = c->r - h;
            if ((dx*dx + dy*dy) <= (c->r * c->r)) {
                SDL_RenderDrawPoint(renderer, c->x + dx, c->y + dy);
            }
        }
    }
}


void GUI::SDL_DrawRoundedRect(SDL_Renderer* renderer, const SDL_Rect* rect, const int r) {
    SDL_Circle tl, tr, bl, br;
    tl.x = rect->x + r;
    tl.y = rect->y + r;
    tr.x = rect->x + rect->w - r;
    tr.y = tl.y;
    bl.x = tl.x;
    bl.y = rect->y + rect->h - r;
    br.x = tr.x;
    br.y = bl.y;

    for (SDL_Circle c : {tl, tr, bl, br}) {
        c.r = r;
        SDL_DrawCircle(renderer, &c);
    }

    SDL_Rect top, left;
    top.x = rect->x + r;
    top.y = rect->y;
    top.w = rect->w - (2 * r);
    top.h = rect->h;

    left.x = rect->x;
    left.y = rect->y + r;
    left.w = rect->w;
    left.h = rect->h - (2 * r);

    SDL_RenderFillRect(renderer, &left);
    SDL_RenderFillRect(renderer, &top);
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
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        Err("fatal: SDL_Mixer could not open a channel; " + std::string(Mix_GetError()));
    }

    if (TTF_Init() < 0) {
        Err("fatal: SDL_TTF could not initialize; " + std::string(Mix_GetError()));
    }

    // create an SDL Window
    window = SDL_CreateWindow(
        (std::string("mpf - ") + files.getFile(0).getFileName()).c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(window, -1, 0);

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

void GUI::renderFrame() {
    // clear renderer
    SDL_SetRenderDrawColor(renderer, 47, 68, 89, 255);
    SDL_RenderClear(renderer);

    // SDL_Circle c;
    // c.x = 50;
    // c.y = 50;
    // c.r = 30;

    SDL_Rect roundedRect;
    roundedRect.h = 100;
    roundedRect.w = 100;
    roundedRect.x = 30;
    roundedRect.y = 30;

    SDL_SetRenderDrawColor(renderer, 2, 194, 251, 255);
    SDL_DrawRoundedRect(renderer, &roundedRect, 20);

    // std::string fileName = this->files.getFile(0).getFileName();
    // const char* text = fileName.c_str();

    // TTF_Font* Sans = TTF_OpenFont("assets/Hack.ttf", 18);
    // SDL_Color White = {255, 255, 255};
    // SDL_Surface* surfaceMessage = TTF_RenderText_Blended(Sans, text, White);
    // SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    // SDL_Rect Message_rect; //create a rect
    // Message_rect.x = 0;  //controls the rect's x coordinate 
    // Message_rect.y = 0; // controls the rect's y coordinte
    // TTF_SizeText(Sans, text, &Message_rect.w, &Message_rect.h);
    // SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

    // SDL_FreeSurface(surfaceMessage);
    // SDL_DestroyTexture(Message);
    SDL_RenderPresent(renderer);
}


void GUI::handleSdlEvent(SDL_Event& evt) {
    if (evt.type == SDL_QUIT || (evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE)) {
        this->isActive = false;
    }
    //     else if (evt.type == SDL_KEYDOWN) {
    //         switch (evt.key.keysym.sym) {
    //             case SDLK_SPACE:
    //                 if (musicPaused) {
    //                     files.getFile(currentFile).beginPlayback(fProgress / 1000.0);
    //                     musicPaused = false;
    //                 }
    //                 else {
    //                     pauseAudio();
    //                 }
    //                 break;
    //             case SDLK_LEFT:
    //                 fProgress -= 5000;
    //                 if (fProgress < 0) fProgress = 0;
    //                 Mix_SetMusicPosition(fProgress / 1000.0);
    //                 break;
    //             case SDLK_RIGHT:
    //                 fProgress += 5000;
    //                 Mix_SetMusicPosition(fProgress / 1000.0);
    //                 break;
    //         }
    //     }
    else if (evt.type == SDL_MOUSEBUTTONDOWN) {
        Log("Click!");
        Log(evt.button.x);
        Log(evt.button.y);
    }
}


void GUI::run() {
    this->files.applyFilter(config.getVariable("FILTER"));
    this->files.applyOrder(config.getVariable("ORDER"));
    this->init(); // Initialize SDL renderer and audio

    while (isActive) {
        // render the current states
        renderFrame();

        // handle all events
        SDL_Event evt;
        while (SDL_PollEvent(&evt)) {
            this->handleSdlEvent(evt);
        }
        

        // // check if we need to start a new song
        // if (!isPlayingAudio()) {
        //     if (!advanceFile()) {
        //         // Log("Completed all files.");
        //         // isActive = false;
        //     }
        //     else {
        //         Log("Advanced to file " + files.getFile(currentFile).filePath);
        //         // begin playback of new audio file
        //         fProgress = 0;
        //         SDL_SetWindowTitle(window, files.getFile(currentFile).filePath.c_str());

        //         // begin playback
        //         files.getFile(currentFile).beginPlayback(0);
        //     }
        // }

        // if (!musicPaused)
        //     fProgress += 1000.0 / (double)FPS;
        SDL_Delay(1000.0 / FPS);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    Log("App terminated successfully.");
    return;
}