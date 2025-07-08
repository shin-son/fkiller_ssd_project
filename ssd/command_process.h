#pragma once
#include <string>
#include <vector>
#include "pattern_checker.h"

class CommandProcessor {
public:
    PatternChecker patterChecker;
    int process(int argc, char* argv[]);
private:
    bool isWriteCommand(const std::string& cmd);
    bool isReadCommand(const std::string& cmd);
    void executeWrite(const std::vector<std::string>& args);
    void executeRead(const std::vector<std::string>& args);
    bool isWriteValidArgument(const std::vector<std::string>& args);
    bool isReadValidArgument(const std::vector<std::string>& args);
    int dispatchCommand(const std::string& cmd, const std::vector<std::string>& args);
    int handleWrite(const std::vector<std::string>& args);
    int handleRead(const std::vector<std::string>& args);
    void printErrorAndWriteToOutput();
};