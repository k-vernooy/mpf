#include <iostream>
#include "../include/util.h"

void Err(std::string message) {
    std::cerr << "\e[31merror: \e[0m" << message << std::endl;
}