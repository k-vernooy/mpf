#include <iostream>

#include "../include/mpf.h"
#include "../include/util.h"

// Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>


using std::cout;
using std::endl;

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
        if (!pair.second.empty()) {
            file += pair.first + "=" + pair.second + "\n";
        }
    }

    WriteFile(file, filepath);
}

MPConfig MPConfig::ReadFromFile(std::string path) {
    std::vector<std::string> lines = Split(ReadFile(path), "\n");
    MPConfig config = MPConfig();
    
    for (std::string line : lines) {
        ValidateConfig(line);
        std::vector<std::string> splits = Split(line, "=");
        std::string varToSet = splits[0];
        std::string valToSet = Join(std::vector<std::string>(
            splits.begin() + 1, splits.end()
        ), "=");
        config.setVariable(varToSet, valToSet);
    }

    return config;
}