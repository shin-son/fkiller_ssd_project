#pragma once
#include <string>
#include <vector>
#include "pattern_checker.h"

class CommandProcessor {
public:
    PatternChecker patterChecker;
    int process(int argc, char* argv[]);
    int flushProcess(const std::vector<std::string>& args);
    int getOperator();
    std::string getInputValue();
    int getAddress();
    int getSize();
    void printErrorAndWriteToOutput();
private:
    int dispatchCommand(const std::string& cmd, const std::vector<std::string>& args);
    
    bool isWriteCommand(const std::string& cmd);
    bool isReadCommand(const std::string& cmd);
    bool isEraseCommand(const std::string& cmd);

    bool isWriteValidArgument(const std::vector<std::string>& args);
    bool isReadValidArgument(const std::vector<std::string>& args);
    bool isEraseValidArgument(const std::vector<std::string>& args);
    
    int handleWrite(const std::vector<std::string>& args);
    int handleRead(const std::vector<std::string>& args);
    int handleErase(const std::vector<std::string>& args);
    
    int ssdOperator;
    std::string memoryValue;
    int address;
    int size;
};