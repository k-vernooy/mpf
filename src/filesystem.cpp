#include <iostream>
#include <algorithm>

// mpf class + method declarations
#include "../include/mpf.h"
#include "../include/util.h"

// for reading tag metadata
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>

using std::cout;
using std::endl;

bool operator== (const File& f1, const File& f2) {
    return f1.filePath == f2.filePath;
}

bool File::validate() {
    return (boost::filesystem::exists(filePath));
}

std::string File::readID3Tag(std::string tag) {
    TagLib::FileRef f(filePath.c_str());

    if (!f.isNull() && f.file()) {
        TagLib::PropertyMap tags = f.file()->properties();
        if (tags.find(tag) != tags.end()) {
            if (tags[tag].size() > 1) Err("lots of vals");
            return std::string(tags[tag][0].toCString());
        }
        else {
            Err("Tag '" + tag + "' is not available in file '" + filePath + "'");
            return "";
        }
    }
    else {
        Err("uncaught error reading tag '" + tag + "'");
        return "";
    }
}

void FilesList::addFile(File f) {
    files.push_back(f);
}

void FilesList::removeFile(File f) {
    files.erase(std::remove(files.begin(), files.end(), f), files.end());
}

File FilesList::getFile(int pos) {
    return files[pos];
}

std::size_t FilesList::size() {
    return files.size();
}

void FilesList::applyFilterCmd(std::string lv, std::string rv, std::string comparator) {
    UpperCase(lv); UpperCase(rv);

    for (size_t i = 0; i < files.size(); i++) {
        std::string tagVal = files[i].readID3Tag(lv);
        UpperCase(tagVal);
        if (!tagVal.empty()) {
            cout << tagVal << ", " << rv << ", " << comparator << endl;
        }
        else if (ARGLIST["--keep"].boolVal)
            continue;
        else
            removeFile(files[i]);
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