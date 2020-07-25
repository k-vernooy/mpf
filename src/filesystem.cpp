#include <iostream>
#include <algorithm>
#include <random>

// mpf class + method declarations
#include "../include/mpf.h"
#include "../include/util.h"

// for reading tag metadata
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/tpropertymap.h>


using std::cout;
using std::endl;

bool operator== (const File& f1, const File& f2) {
    return f1.filePath == f2.filePath;
}

bool File::validate() {
    return (boost::filesystem::exists(filePath));
}

std::string File::getFileName(bool stem) {
    boost::filesystem::path p(this->filePath);
    if (!stem) {
        return std::string(p.filename().c_str());
    }
    else {
        return std::string(p.stem().c_str());
    }
}


void FilesList::addFile(AudioFile f) {
    files.push_back(f);
}


void FilesList::removeFile(AudioFile f) {
    files.erase(std::remove(files.begin(), files.end(), f), files.end());
}

AudioFile FilesList::getFile(int pos) {
    return files[pos];
}

std::size_t FilesList::size() {
    return files.size();
}

std::string AudioFile::readTag(std::string tag) {
    return "";
}

std::vector<AudioFile>::iterator FilesList::begin() {
    return files.begin();
}

std::vector<AudioFile>::iterator FilesList::end() {
    return files.end();
}

void FilesList::applyFilterCmd(std::string lv, std::string rv, std::string comparator) {
    UpperCase(lv); UpperCase(rv);

    for (size_t i = 0; i < files.size(); i++) {
        std::string tagVal = files[i].readTag(lv);
        UpperCase(tagVal);
        bool keepFile = true;

        if (!tagVal.empty()) {
            // if (IsNumber(tagVal))
            if (comparator == "==") {
                keepFile = (tagVal == rv);
            }
            else {
                if (IsNumber(tagVal) && IsNumber(rv)) {
                    // Convert to the appropriate type
                    double ld = stod(tagVal);
                    double rd = stod(rv);

                    // perform comparison
                    if (comparator == "<")
                        keepFile = ld < rd;
                    else if (comparator == ">")
                        keepFile = ld > rd;
                    else if (comparator == ">=")
                        keepFile = ld >= rd;
                    else if (comparator == "<=")
                        keepFile = ld <= rd;
                }
                else if (!IsNumber(tagVal) && !IsNumber(rv)) {
                    // perform alphabetical check
                    if (comparator == ">")
                        keepFile = tagVal > rv;
                    else if (comparator == "<")
                        keepFile = tagVal < rv;
                    else if (comparator == ">=")
                        keepFile = tagVal >= rv;
                    else if (comparator == "<=")
                        keepFile = tagVal <= rv;
                }
                else {
                    Err("fatal: cannot compare with quantity operators (<, >, <=, >=) a numeric lv and a string rv, or vice versa.");
                    exit(1);
                }
            }
        }
        else if (!ARGLIST["--keep"].boolVal)
            keepFile = false;
        
        if (!keepFile) {
            removeFile(files[i]);
        }
    }
    return;
}

void FilesList::applyFilter(std::string filter) {
    std::vector<std::string> cmds = Split(filter, ";");
    std::vector<std::string> separators = {
        "==", "<=", ">=", "<", ">"
    };

    for (std::string cmd : cmds) {
        std::string comperator, lv, rv;
        for (std::string separator : separators) {
            if (cmd.find(separator) != std::string::npos) {
                comperator = separator;
                std::vector<std::string> values = Split(cmd, comperator);
                lv = values[0];
                rv = Join(std::vector<std::string>(
                    values.begin() + 1, values.end()
                ), comperator);
                break;
            }
        }

        this->applyFilterCmd(lv, rv, comperator);
    }
}

void FilesList::applyOrder(std::string orderCmd) {
    if (files.size() == 0) return;
    if (orderCmd == "shuffle") {
        // shuffle the `files` vector
        File firstFile = *files.begin();
        if (files.size() > 1) {
            // loop shuffling to avoid replaying the same song twice
            do {
                std::shuffle(files.begin(), files.end(), std::random_device());
            } while (*files.begin() == firstFile);
        }
        return;
    }
}


void FilesList::print() {
    cout << "[";
    for (File x : files) {
        cout << x.filePath << ", ";
    }
    cout << "\b\b] " << endl;
}


FilesList FileSystem::GetAllFiles(Directory) {
    FilesList files;
    // create boost path; iterate over files
    return files;
}


bool FileSystem::ValidateDirectory(Directory dir) {
    return true;
}