/****************************************
 * @file                          mpf.cpp
 * @author                      k-vernooy
 * 
 * Single include file for all source
 * mpf files; contains any classes, enums
 * or structs for file reading and
 * interface implementation (CLI/GUI)
 ***************************************/

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
class Config;
class FilesList;
class File;
class AudioFile;
class GUI;


/**
 * A class representing an AudioApp configuration.
 * Contains default variables, as well as
 * filter and order vars used instantially.
 */
class Config {
    // A list of blank variables, to be read from
    // config files or to be specified at runtime
    std::map<std::string, std::string> VARIABLES = {
        {"MUSIC_DIR", ""},
        {"FILTER", ""},
        {"ORDER", ""}
    };

    public:
        Config() {}
        static Config ReadFromFile(const std::string& filePath);
        void writeToFile(const std::string& filePath);  
        void setVariable(std::string);  
        void setVariable(const std::string& var, const std::string& val);     
        std::string getVariable(const std::string& var);
};


/**
 * Represents a command line argument.
 * Contains information about each CLI argument
 * as well as the value passed.
 */
class CliArg {
public:
    CliArg(const std::string& name, bool isBool, bool (*validation)(const std::string&))
        : name(name), isBool(isBool), validation(validation) {}
    CliArg() {}

    std::string name;
    bool isBool = false;
    bool boolVal = false;

    int numVals;
    int isInf = false;

    std::vector<std::string> passedValue;

    // validate the quality of the passed value
    bool (*validation)(const std::string&);
};

// Validate strings passed into the CLI
bool ValidateFilter(const std::string& arg);
bool ValidateOrder(const std::string& arg);
bool ValidateConfig(const std::string& arg);

// Create global argument collector
extern std::map<std::string, CliArg> ARGLIST;


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
        File(const std::string& filePath)
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
    AudioFile(std::string fp)
        : File(fp) {};
    AudioFormat getFormat();
    std::string readTag(const std::string& tagName);
    void beginPlayback(double start);
    void readData();
    SDL_Surface* readImage();
};


/**
 * A wrapper for a vector of Files.
 * Can be filtered and ordered by ID3v2 tags using id3lib
 */
class FilesList {
private:
    std::vector<AudioFile> files;
    static bool ValidateFile(std::string&);
    void applyFilterCmd(std::string, std::string, std::string);

public:
    FilesList() {}

    bool validateAllFiles();

    AudioFile getFile(int pos);
    void addFile(const AudioFile&);
    void print();

    void removeFile(const AudioFile&);
    void applyFilter(const std::string&);
    void applyOrder(const std::string&);

    std::vector<AudioFile>::iterator begin();
    std::vector<AudioFile>::iterator end();
    std::size_t size();
};


class FileSystem {
public:
    static FilesList GetAllFiles(const std::string& dir);
    static bool ValidateDirectory(const std::string& dir);
};


class AudioApp {
protected:
    enum class ACTIONS {
        FORWARD, BACKWARD, PREV, NEXT, TOGGLE_PAUSE
    };

    int FPS;
    bool isActive;
    bool musicPaused;

    bool executeAction(ACTIONS action);
    bool isPlayingAudio();

    Config config;
    FilesList files;
    

    void filterFiles();
    void orderFiles();
    virtual void run() = 0;

public:
    AudioApp(const Config& config, const FilesList& files)
        : config(config), files(files), isActive(false), musicPaused(false) {}
};


/**
 * Assorted GUI methods for handling draw calls,
 * rasterizing objects, and classes for catching/handling
 * mouse down events (buttons)
 */
class GUI : public AudioApp {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* screenSurface;
    SDL_Surface* backgroundImage;
    int FPS;

    void clear();
    void renderFrame();
    void handleSdlEvent(SDL_Event& evt);
    void init();


public:
    GUI(const Config& config, const FilesList& files, const int FPS = 20)
        : FPS(FPS), AudioApp(config, files) {
    }

    void run();

    /**
     * Static graphics methods that draw to SDL_Rendereres
     * and rasterize various necessary GUI components (circles, rounded rects).
     * SDL_SetRenderDrawColor should be set before calling these.
     */
    class SDL_Button {
        SDL_Button(void(*callback)(GUI));
        void handleEvt(SDL_Event* evt);
        void (*callback)(GUI*);
    };

    typedef struct SDL_Circle {
        int x, y;
        int r;
    } SDL_Circle;

    static void SDL_DrawCircle(SDL_Renderer*, const ::GUI::SDL_Circle*);
    static void SDL_DrawRoundedRect(SDL_Renderer*, const SDL_Rect*, const int);
};


class CLI : public AudioApp {
    public:
        CLI(Config config, FilesList files)
            : AudioApp(config, files) {};
};

#endif