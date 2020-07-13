#ifndef MPF_H
#define MPF_H

#include <string>
#include <vector>
#include <map>
#include <boost/filesystem.hpp>


class CliArg;
class MusicPlayer;
class MPConfig;
class FilesList;
class File;
class MPDHandler;

/**
 * A class representing a MusicPlayer configuration.
 * Contains default variables, as well as
 * filter and order vars used instantially.
 */
class MPConfig {
    // A list of blank variables, to be read from
    // config files or to be specified at runtime
    std::map<std::string, std::string> VARIABLES = {
        {"DEFAULT_DIR", ""},
        {"DEFAULT_FILTER", ""},
        {"DEFAULT_ORDER", ""},
        {"FILTER", ""},
        {"ORDER", ""}
    };

    public:
        // Read and write to the ~/.music-filter.config file
        static MPConfig ReadFromFile(std::string filePath);
        void writeToFile(std::string filePath);  
        void setVariable(std::string var);  
        void setVariable(std::string var, std::string val);     
        std::string getVariable(std::string var);
};


/**
 * Represents a command line argument.
 * Contains information about each CLI argument
 * as well as the value passed.
 */
class CliArg {
    public:
        CliArg(std::string name, bool isBool, bool (*validation)(std::string))
            : name(name), isBool(isBool), validation(validation) {}
        CliArg() {}

        std::string name;
        bool isBool;
        bool boolVal;

        int numVals;
        int isInf;

        std::vector<std::string> passedValue;

        // validate the quality of the passed value
        bool (*validation)(std::string);
};


// All file formats supported by mpf
enum FileFormat {
    MP3, WAV, OGG
};


/**
 * Represents a physical and validated
 * audio file. Can read tags from the file.
 */
class File {
    std::string filePath;
    std::string readID3Tag(std::string tagName);
    bool validate();
    FileFormat format;
};


/**
 * A wrapper for a vector of Files.
 * Can be filtered and ordered by ID3v2 tags using id3lib
 */
class FilesList {
    std::vector<File> files;
    static bool ValidateFile(std::string);

    public:
        FilesList() {}

        bool validateAllFiles();
        void addFile(std::string);
        void removeFile(std::string);
        void applyFilter(std::string);
        void applyOrder(std::string);
        int  size();
};


typedef std::string Directory;

class FileSystem {
    public:
        static FilesList GetAllFiles(Directory dir);
        static bool ValidateDirectory(Directory dir);
};


/**
 * Handles sending signals to MPD.
 * Contains a list of files.
 */
class MPDHandler {
    public:
        MPDHandler(FilesList files)
            : files(files) {}
        FilesList files;
};


/**
 * The music player. Lists the playable files,
 * filters the files, opens a gui, and sends information
 * to MPD using a separate driver.
 */
class MusicPlayer {
    public:
        static MusicPlayer CreateDefaultMP();
        MPConfig configuration;
        FilesList files;

        void beginGUI(MPDHandler* handler);
};

bool ValidateFilter(std::string arg);
bool ValidateOrder(std::string arg);
bool ValidateConfig(std::string arg);

static std::map<std::string, CliArg> ARGLIST = {
    {"--order", CliArg("--order", false, ValidateOrder)},
    {"--filter", CliArg("--filter", false, ValidateFilter)},
    {"--config", CliArg("--config", false, ValidateConfig)}
};

#endif