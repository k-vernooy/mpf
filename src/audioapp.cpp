#include "../include/mpf.h"


// bool AudioApp::advanceFile() {
//     if (currentFile != files.size())
//         currentFile++;
//     return (currentFile != files.size());
// }

// void AudioApp::pauseAudio() {
//     musicPaused = true;
//     Mix_HaltMusic();
// }

bool AudioApp::isPlayingAudio() {
    return (Mix_PlayingMusic() || musicPaused);
}
