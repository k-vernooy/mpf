/****************************************
 * @file                    audiofile.cpp
 * @author                      k-vernooy
 * 
 * Code to perform reading, writing, DSP,
 * playback and other analysis on various
 * audio files.
 ***************************************/

#include "../include/mpf.h"
#include "../include/util.h"

void AudioFile::readData() {
    data = Mix_LoadMUS(filePath.c_str()); 
    if (data == nullptr) {
        Err("Error loading file " + filePath + "; " + std::string(Mix_GetError()));
    }
}


SDL_Surface* AudioFile::readImage() {
    const char* imagePath = this->readTag("URL").c_str();
    return IMG_Load(imagePath);
}


void AudioFile::beginPlayback(double start) {
    if (data == nullptr) readData();
    Mix_PlayMusic(data, 1);
    Mix_SetMusicPosition(start);
}