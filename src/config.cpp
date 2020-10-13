/****************************************
 * @file                       config.cpp
 * @author                      k-vernooy
 * 
 * Code for handling getting and setting
 * of a music player data structure. Has
 * read and write to file functionality.
 ***************************************/

#include <iostream>

#include "../include/mpf.h"
#include "../include/util.h"

using std::cout;
using std::endl;


void Config::setVariable(const std::string& var, const std::string& val) {
    VARIABLES[var] = val;
}

void Config::setVariable(std::string assignment) {
    std::string del = "=";
    int pos = assignment.find(del);
    std::string var = assignment.substr(0, pos);
    assignment.erase(0, pos + del.length());

    // should check for quote removal here
    VARIABLES[var] = assignment;
}

std::string Config::getVariable(const std::string& var) {
    return VARIABLES[var];
}

void Config::writeToFile(const std::string& filepath) {
    std::string file;
    for (auto& pair : VARIABLES) {
        if (!pair.second.empty()) {
            file += pair.first + "=" + pair.second + "\n";
        }
    }

    WriteFile(file, filepath);
}

Config Config::ReadFromFile(const std::string& path) {
    std::vector<std::string> lines = Split(ReadFile(path), "\n");
    Config config = Config();
    
    for (std::string& line : lines) {
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