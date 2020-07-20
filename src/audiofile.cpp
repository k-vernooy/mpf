/****************************************
 * @file                    audiofile.cpp
 * @author                      k-vernooy
 * 
 * Code to perform reading, writing, DSP,
 * playback and other analysis on various
 * audio files.
 ***************************************/

#include "../include/mpf.h"

void AudioFile::readData() {
    data = Mix_LoadMUS(filePath.c_str()); 
    if (song == nullptr) {
        Err("Error loading file " + filePath + "; " + std::string(Mix_GetError()));
    }
}


SDL_Surface* AudioFile::readImage() {
    const char* imagePath = this->readTag("URL");
    return IMG_Load(imagePath);
}


void AudioFile::beginPlayback(double start) {
    Mix_SetMusicPosition(start);
    Mix_PlayMusic(song, -1);
}