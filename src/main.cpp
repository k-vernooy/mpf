/****************************************
 * @file                         main.cpp
 * @author                      k-vernooy
 * 
 * Driver for mpf - handles command line
 * arguments, creates MusicPlayer and
 * AudioApp objects to begin playback
 ***************************************/


#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "../include/mpf.h"
#include "../include/util.h"

using std::cout;
using std::endl;


// set up the CLI arguments list
std::map<std::string, CliArg> ARGLIST = {
    {"--order", CliArg("--order", false, ValidateOrder)},
    {"--filter", CliArg("--filter", false, ValidateFilter)},
    {"--config", CliArg("--config", false, ValidateConfig)},
    {"--verbose", CliArg("--verbose", true, nullptr)},
    {"--keep", CliArg("--keep", true, nullptr)},
    {"--loop", CliArg("--loop", true, nullptr)},
    {"--version", CliArg("--version", true, nullptr)},
    {"--no-gui", CliArg("--no-gui", true, nullptr)}
};


int main(int argc, char** argv) {
    // configure CLI arguments
    ARGLIST["--config"].isInf = true;
    ARGLIST["--filter"].numVals = 1;
    ARGLIST["--order"].numVals = 1;

    // read the user's home directory
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL)
        homedir = getpwuid(getuid())->pw_dir;
    std::string CONFIG_PATH = std::string(homedir) + "/.config/mpf/config";

    // Create a files list and configurations object
    Config config = Config::ReadFromFile(CONFIG_PATH);
    FilesList files = FilesList();

    // Parse CLI arguments, add to music player configuration
    for (int i = 1; i < argc; i++) {
        // if the argument passed is a recognized CLI arg, parse it
        if (ARGLIST.find(argv[i]) != ARGLIST.end()) {
            CliArg arg = ARGLIST[argv[i]];
            // set to true, since flag was set
            if (arg.isBool) {
                ARGLIST[arg.name].boolVal = true;
            }
            else {
                // arg takes infinite parameters, add all to value
                if (arg.isInf) {
                    i++; // increase arg count to check for vals
                    while (i != argc) {
                        // validate all passed values, add to array
                        std::string argname(argv[i]);
                        ARGLIST[arg.name].validation(argname);
                        ARGLIST[arg.name].passedValue.push_back(argv[i]);
                        i++;
                    }

                    // check whether the user supplied any values for arg
                    if (ARGLIST[arg.name].passedValue.size() == 0) {
                        Err("fatal: " + arg.name + " expects at least 1 value passed.");
                        return 1;
                    }
                }
                else {
                    // check for N values passed to the flag
                    for (int x = 0; x < arg.numVals; x++) {
                        i++;

                        if (i == argc) {
                            Err("fatal: " + arg.name + " expects " + std::to_string(arg.numVals) + " arguments, not enough provided.");
                            return 1;
                        }
                        else {
                            std::string argname(argv[i]);
                            ARGLIST[arg.name].validation(argname);
                            ARGLIST[arg.name].passedValue.push_back(argv[i]);
                        }
                    }
                }
            }
        }
        else {
            // is a file, add to FilesList
            AudioFile file = AudioFile(argv[i]);
            if (!file.validate()) {
                Err("fatal: " + file.filePath + " is neither a valid argument or file.");
                return 1;
            }

            files.addFile(file);
        }
    }

    // If the user asks to see version, print macro
    if (ARGLIST["--version"].boolVal) {
        cout << MPF_VERSION << endl;
        return 0;
    }

    // if the configuration has been modified, update the global
    // configuration object and write it back to ~/.mpf.config
    if (ARGLIST["--config"].passedValue.size() != 0) {
        for (std::string conf : ARGLIST["--config"].passedValue) {
            config.setVariable(conf);
        }
        config.writeToFile(CONFIG_PATH);

        // exit, because we cannot play files while configuring
        Log("Succesfully updated configurations.");
        return 0;
    }


    /**
     * If the files object contains no files, check
     * to see if the user has a default dir. Validate dir,
     * set files to contents of dir
     */
    if (files.size() == 0) {
        std::string defaultDir = config.getVariable("MUSIC_DIR");
        if (defaultDir.empty()) {
            Err("fatal: No files provided and no MUSIC_DIR set.");
            Err("hint: Input file(s) or see `--config`");
            return 1;
        }
        else {
            // validate defaultDir
            if (FileSystem::ValidateDirectory(defaultDir)) {
                // set the FilesList in `player` to all files in defaultDir
                files = FileSystem::GetAllFiles(defaultDir);
                if (files.size() == 0) {
                    Err("fatal: No files in MUSIC_DIR");
                }
            }
            else {
                Err("fatal: Music directory '" + defaultDir + "' does not exist.");
                return 1;
            }
        }
    }
    
    // If we have been passed something other than default vals,
    // input them into the config object for use in the AudioApp
    if (ARGLIST["--filter"].passedValue.size() != 0)
        config.setVariable("FILTER", ARGLIST["--filter"].passedValue[0]);
    if (ARGLIST["--order"].passedValue.size() != 0)
        config.setVariable("ORDER", ARGLIST["--order"].passedValue[0]);

    if (!ARGLIST["--no-gui"].boolVal) {
        GUI gui = GUI(config, files, 30);
        gui.run();
    }
}