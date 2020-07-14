#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "../include/util.h"


void Err(std::string message) {
    std::cerr << "\e[31merror: \e[0m" << message << std::endl;
}

std::vector<std::string> Split(std::string str, std::string del) {
    std::vector<std::string> array; // to be returned
    int pos = 0;
    std::string token;

    while ((pos = str.find(del)) != std::string::npos) {
        // loop through string, remove parts as it iterates
        token = str.substr(0, pos);
        array.push_back(token);
        str.erase(0, pos + del.length());
    }

    array.push_back(str); // add what remains of the string
    return array;
}


std::string Join(std::vector<std::string> str, std::string del) {
    if (str.size() == 0)
        return "";
        
    std::string ret = "";
    for (int i = 0; i < str.size() - 1; i++)
        ret += str[i] + del;

    ret += str[str.size() - 1];
    return ret;
}


std::string ReadFile(std::string path) {
    // initiate stream and buffer
    std::ifstream f(path);
    std::stringstream buffer;
    // add the file to the buffer
    buffer << f.rdbuf();
    // return buffer string
    return buffer.str();
};


void WriteFile(std::string contents, std::string path) {
    std::ofstream of(path);
    of << contents;
    of.close();
}


void UpperCase(std::string& str) {
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
}