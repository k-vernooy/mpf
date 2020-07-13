#include <string>
#include <vector>
#include <map>

static std::map<std::string, CliArg> ARGLIST;


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
};

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
 * eCan be filtered and ordered by ID3v2 tags using id3lib
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
};