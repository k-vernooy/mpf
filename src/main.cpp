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
int main(int argc, char** argv) {    
    // Create a music player object
    MusicPlayer player = MusicPlayer::CreateDefaultMP();
    MPConfig globalConfig = MPConfig::ReadFromFile("~/.music-filter.config");

    // Parse CLI arguments, add to music player configuration
    for (int i = 1; i < argc; i++) {
        // if the argument passed is a recognized CLI arg, parse it
        if (ARGLIST.find(argv[i]) != ARGLIST.end()) {
            CliArg arg = ARGLIST[argv[i]];
            // set to true, since flag was set
            if (arg.isBool) {
                ARGLIST[argv[i]].boolVal = true;
            }
            else {
                // arg takes infinite parameters, add all to value
                if (arg.isInf) {
                    while (i != argc) {
                        // validate all passed values, add to array
                        ARGLIST[argv[i]].validation(argv[i]);
                        ARGLIST[argv[i]].passedValue.push_back(argv[i]);
                        i++;
                    }

                    // check whether the user supplied any values for arg
                    if (ARGLIST[argv[i]].passedValue.size() == 0) {
                        Err("fatal: " + arg.name + " expects at least 1 value passed. Exiting...");
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
                            ARGLIST[argv[i]].validation(argv[i]);
                            ARGLIST[argv[i]].passedValue.push_back(argv[i]);
                        }
                    }
                }
            }
        }
        else {
            // is a file, add to FilesList
            player.files.addFile(argv[i]);
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
        cout << "Succesfully updated configurations. Exitting..." << endl;
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

    MPDHandler mpd = MPDHandler(player.files);
    player.beginGUI(&mpd);
}