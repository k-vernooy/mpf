#ifndef MPF_H
#define MPF_H

#define MPF_VERSION "mpf 0.0.1"

#include <string>
#include <vector>
#include <map>
#include <boost/filesystem.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

class CliArg;
class MusicPlayer;
class MPConfig;
class FilesList;
class File;
class AudioFile;
class GUI;


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
        MPConfig() {}
        // Read and write to the ~/.music-filter.config file
        static MPConfig ReadFromFile(const std::string filePath);
        void writeToFile(const std::string filePath);  
        void setVariable(const std::string var);  
        void setVariable(const std::string var, const std::string val);     
        std::string getVariable(const std::string var);
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
        bool isBool = false;
        bool boolVal = false;

        int numVals;
        int isInf = false;

        std::vector<std::string> passedValue;

        // validate the quality of the passed value
        bool (*validation)(std::string);
};


// All file formats supported by mpf
enum class AudioFormat {
    MP3, WAV, OGG
};


/**
 * Represents a physical and validated
 * audio file. Can read tags from the file.
 */
class File {
    public:
        File(std::string filePath)
            : filePath(filePath) {};

        std::string filePath;
        bool validate();
        std::string getFileName(bool stem = false);
        friend bool operator== (const File& f1, const File& f2);
};


/**
 * @extends File
 * A class representing an audio file of
 * any format. Can read samples, metadata
 * and imbedded images.
 */
class AudioFile : public File {
private:
    Mix_Music* data;

public:
    AudioFormat getFormat();
    std::string readID3Tag(const std::string tagName);
    void beginPlayback(double start);
    void readData();
    SDL_Surface* readImage();
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
        File getFile(int pos);
        void addFile(File);
        void removeFile(File);
        void applyFilter(std::string);
        void applyFilterCmd(std::string, std::string, std::string);
        void applyOrder(std::string);
        void print();
        std::size_t size();
};


typedef std::string Directory;

class FileSystem {
    public:
        static FilesList GetAllFiles(Directory dir);
        static bool ValidateDirectory(Directory dir);
};


/**
 * The music player. Lists the playable files,
 * filters the files, opens a gui, and sends information
 * to MPD using a separate driver.
 */
class MusicPlayer {
public:
    MusicPlayer() {}
    MPConfig configuration;
    FilesList files;

    friend class GUI;
};

bool ValidateFilter(std::string arg);
bool ValidateOrder(std::string arg);
bool ValidateConfig(std::string arg);

extern std::map<std::string, CliArg> ARGLIST;


/**
 * Assorted GUI methods for handling draw calls,
 * rasterizing objects, and classes for catching/handling
 * mouse down events (buttons)
 */
class GUI {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* screenSurface;
    SDL_Surface* backgroundImage;

    Uint32 fProgress;
    Uint32 fDuration;

    int FPS;
    bool isActive;
    bool musicPaused;


    void clear();
    void renderFrame();
    void advanceFile();
    void playAudio();
    void pauseAudio();
    void isPlayingAudio();

public:
    GUI(int FPS = 20)
        : isActive(false), musicPaused(false), FPS(FPS) {}
    
    void beginGUI(MusicPlayer mp);
    static void SDL_DrawCircle(SDL_Renderer*, const int, const int, const int);
    static void SDL_DrawRoundedRect(SDL_Renderer*, const SDL_Rect*, const int);
    void init(); // Initializes SDL, window, mixer

}


class SDL_Button {
    SDL_Button(void(*callback)(GUI));
    void handleEvt(SDL_Event* evt);
    void (*callback)(GUI*);
};

#endif