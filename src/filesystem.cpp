#include <iostream>
#include <algorithm>
#include "../include/mpf.h"
#include "../include/util.h"


bool operator== (const File& f1, const File& f2) {
    return f1.filePath == f2.filePath;
}

void FilesList::addFile(File f) {
    files.push_back(f);
}

void FilesList::removeFile(File f) {
    files.erase(std::remove(files.begin(), files.end(), f), files.end());
}

std::size_t FilesList::size() {
    return files.size();
}

void FilesList::applyFilterCmd(std::string lv, std::string rv, std::string comperator) {
    return;
}

void FilesList::applyFilter(std::string filter) {
    std::vector<std::string> cmds = Split(filter, ";");
    for (std::string cmd : cmds) {
        // parse cmd into comperator, lv, and rv
        // if (cmd.))
            // this->applyFilterCmd(cmd);
        std::cout << "Applying command " << cmd << std::endl;
    }
}

void FilesList::applyOrder(std::string orderCmd) {
    return;
}

FilesList FileSystem::GetAllFiles(Directory) {
    FilesList files;
    // create boost path; iterate over files
    return files;
}

bool FileSystem::ValidateDirectory(Directory dir) {
    return true;
}