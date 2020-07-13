#include <iostream>
#include "../include/mpf.h"
#include "../include/util.h"

void MPConfig::setVariable(std::string var, std::string val) {
    VARIABLES[var] = val;
}

void MPConfig::setVariable(std::string assignment) {
    std::string del = "=";
    int pos = assignment.find(del);
    std::string var = assignment.substr(0, pos);
    assignment.erase(0, pos + del.length());

    // should check for quote removal here
    VARIABLES[var] = assignment;
}

std::string MPConfig::getVariable(std::string var) {
    return VARIABLES[var];
}

void MPConfig::writeToFile(std::string filepath) {
    std::string file;
    for (auto& pair : VARIABLES) {
        file += pair.first + "=" + pair.second + "\n";
    }

    std::cout << file << std::endl;
    // write file to filepath
}

MPConfig MPConfig::ReadFromFile(std::string path) {
    std::string fileStr = ReadFile(path);
    MPConfig config = MPConfig();
    // **file parser happens here**
    return config;
}

void MusicPlayer::beginGUI(MPDHandler* handler) {
    // start window
    // load basic gui
    // wait events
    // send and recieve updates to mpd.
    return;
}