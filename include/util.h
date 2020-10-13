#pragma once
#include <iostream>
#include <string>
#include <vector>


template<typename T> void Err(T message);
template<typename T> void Log(T message);
std::vector<std::string> Split(std::string, std::string);
std::string Join(std::vector<std::string>, std::string);
std::string ReadFile(std::string path);
void WriteFile(std::string contents, std::string path);
void UpperCase(std::string& str);
bool IsNumber(std::string);


template<typename T>
void Err(T message) {
    std::cerr << "\e[31merror: \e[0m" << message << std::endl;
}

template<typename T>
void Log(T message) {
    std::cout << "mpf: " << message << std::endl;
}