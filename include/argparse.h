#include <string>
#include <vector>
#include <map>

class CliArg {
    public:
        CliArg(std::string name, bool isBool, bool (*validation)(std::string))
            : name(name), isBool(isBool), validation(validation) {}

        std::string name;
        bool isBool;
        std::string passedValue;
        bool (*validation)(std::string);
};


class FilesList {
    std::vector<std::string> files;

    public:
        FilesList() {}
        void addFile(std::string);
        void removeFile(std::string);
};


class ArgParser {
    public:
        ArgParser(int argc, char** argv);
};