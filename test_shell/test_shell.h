#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "ssd_interface.h"
#include "constants.h"
#include "logger.h"
#include "command_factory.h"
#include "ICommand.h"

using std::string;
using std::vector;

class TestShell
{
public:
    TestShell() = default;
    TestShell(SSDInterface* ssdInterface)
        : ssdAdapter{ ssdInterface }
    {
    }
    void setSsdAdapter(SSDInterface* adapter);
    void runShell();
    int runCommand(const std::string& command);

#ifdef _DEBUG
    string intToHexString(int value);
#endif

private:
    bool isSSDCommand(const std::string& cmdType);
    NEXT_TEST processTestShellCommand(const std::string& cmdType);
    NEXT_TEST processSsdTest(const std::string& cmd);
    void printHelp();
    
    SSDInterface* ssdAdapter;
    Logger& logger = Logger::getInstance();
    CommandFactory cmdCreator;
    const string CLASS_NAME = "TestShell";
};