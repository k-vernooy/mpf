#include <iostream>
#include "../include/util.h"
#include "../include/argparse.h"


bool ValidateOrder(std::string arg) {
    if (arg != "--order") {
        Err("Passed arg is incorrect in ValidateOrder");
        return false;
    }
    if (true) {
        return true;
    }
    Err("Unanticipated error while parsing argument, exitting");
    return false;
}


bool ValidateFilter(std::string arg) {
    if (arg != "--order") {
        Err("Passed arg is incorrect in ValidateOrder");
        return false;
    }
    if (true) {
        return true;
    }
    Err("Unanticipated error while parsing argument, exitting");
    return false;
}


static std::map<std::string, CliArg> ARGLIST = {
    {"--order", CliArg("--order", false, ValidateOrder)},
    {"--filter", CliArg("--filter", false, ValidateFilter)}
};


ArgParser::ArgParser(int argc, char** argv) {
    FilesList inputFiles = FilesList();
    for (int i = 0; i < argc; i++) {
        if (ARGLIST.find(argv[i]) != ARGLIST.end()) {
            ARGLIST[argv[i]].validation(argv[i]);
        }
        else {
            // is a file, add to FilesList
            inputFiles.addFile(argv[i]);
        }
    }
}