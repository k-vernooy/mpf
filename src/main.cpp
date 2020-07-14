#include <iostream>

#include "../include/mpf.h"
#include "../include/util.h"

using std::cout;
using std::endl;


/**
 * The main driver for the entire program
 * Creates a music player, parses CLI args, and runs
 * the gui app.
 */
std::map<std::string, CliArg> ARGLIST = {
    {"--order", CliArg("--order", false, ValidateOrder)},
    {"--filter", CliArg("--filter", false, ValidateFilter)},
    {"--config", CliArg("--config", false, ValidateConfig)},
    {"--verbose", CliArg("--verbose", true, nullptr)},
    {"--keep", CliArg("--keep", true, nullptr)}
};

int main(int argc, char** argv) {
    // configure CLI arguments
    ARGLIST["--config"].isInf = true;
    ARGLIST["--filter"].numVals = 1;
    ARGLIST["--order"].numVals = 1;

    // Create a music player object
    MusicPlayer player = MusicPlayer();
    MPConfig globalConfig = MPConfig::ReadFromFile("~/.music-filter.config");

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
                        ARGLIST[arg.name].validation(argv[i]);
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
                            ARGLIST[arg.name].validation(argv[i]);
                            ARGLIST[arg.name].passedValue.push_back(argv[i]);
                        }
                    }
                }
            }
        }
        else {
            // is a file, add to FilesList
            File file = File(argv[i]);
            if (!file.validate()) {
                Err("fatal: " + file.filePath + " is neither a valid argument or file.");
                return 1;
            }

            player.files.addFile(file);
        }
    }

    // if the configuration has been modified, update the global
    // configuration object and write it back to ~/.mpf.config
    if (ARGLIST["--config"].passedValue.size() != 0) {
        for (std::string conf : ARGLIST["--config"].passedValue) {
            globalConfig.setVariable(conf);
        }
        globalConfig.writeToFile("~/.mpf.config");

        // exit, because we cannot play files while configuring
        cout << "Succesfully updated configurations." << endl;
        return 0;
    }

    player.configuration = globalConfig;
    
    // if the player has no files, check for a default dir
    if (player.files.size() == 0) {
        std::string defaultDir = player.configuration.getVariable("DEFAULT_DIR");
        if (defaultDir.empty()) {
            Err("fatal: No files provided and no DEFAULT_DIR set.");
            return 1;
        }
        else {
            // validate defaultDir
            if (FileSystem::ValidateDirectory(defaultDir)) {
                // set the FilesList in `player` to all files in defaultDir
                player.files = FileSystem::GetAllFiles(defaultDir);
            }
            else {
                Err("fatal: Default directory '" + defaultDir + "' does not exist.");
                return 1;
            }
        }
    }

    // determine filter to use
    std::string filterStr;
    if (ARGLIST["--filter"].passedValue.size() != 0)
        filterStr = ARGLIST["--filter"].passedValue[0];
    else if (!player.configuration.getVariable("DEFAULT_FILTER").empty())
        filterStr = player.configuration.getVariable("DEFAULT_FILTER");
    
    // apply filter if given
    if (!filterStr.empty())
        player.files.applyFilter(filterStr);

    std::string orderStr;
    if (ARGLIST["--order"].passedValue.size() != 0)
        orderStr = ARGLIST["--order"].passedValue[0];
    else if (!player.configuration.getVariable("DEFAULT_ORDER").empty())
        orderStr = player.configuration.getVariable("DEFAULT_ORDER");

    if (!orderStr.empty())
        player.files.applyOrder(orderStr);

    if (ARGLIST["--verbose"].boolVal) {
        for (size_t i = 0; i < player.files.size(); i++) {
            cout << player.files.getFile(i).filePath << endl;
        }
    }
    MPDHandler mpd = MPDHandler(player.files);
    player.beginGUI(&mpd);
}