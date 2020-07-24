#include <string>
#include <vector>

void Err(std::string message);
void Log(std::string message);
std::vector<std::string> Split(std::string, std::string);
std::string Join(std::vector<std::string>, std::string);
std::string ReadFile(std::string path);
void WriteFile(std::string contents, std::string path);
void UpperCase(std::string& str);
bool IsNumber(std::string);