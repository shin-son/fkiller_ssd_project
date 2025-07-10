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

private:
    bool isSSDCommand(const std::string& cmdType);
    NEXT_TEST processTestShellCommand(const std::string& cmdType);
    NEXT_TEST processSsdTest(const std::string& cmd);

    void printHelp();
    void fullWrite(const std::string& data);
    void fullRead();
    void fullWriteAndReadCompare();
    void partialLBAWrite(const string& data = INPUT_DATA_FOR_PARTIAL_LBA_WRITE);
    void writeReadAging();
    void eraseWriteAging();

    string readWithPrintScreen(const int LBA);
    string write(const int LBA, const string& data);
    string erase(const int LBA, const int size = 1);
    string intToHexString(int value);
    bool writeTheSequence(const std::vector<int>& lbaSequence, const std::string& data);
    bool verifyTheSequence(const std::string& data, const vector<int>& lbaSequence);
    void eraseWithSize(std::istringstream& iss);
    void eraseWithEndLBA(std::istringstream& iss);
    bool getEraseParameter(int& startLBA, int& size, std::istringstream& iss);
    bool isVaiidEraseRange(const int startLBA, const int endLBA);
    bool eraseRange(int startLBA, int endLBA);
    bool eraseWriteAgingOneCycle(int firstStartLBA, int lastStartLBA, int increment);
    string getRandomInput();

    friend class TestShellFixture_ReadPass_Test;
    friend class TestShellFixture_ReadFailWrongLBA_Test;
    friend class TestShellFixture_FullReadPass_Test;
    friend class TestShellFixture_FullReadFail_Test;
    friend class TestShellFixture_WritePass_Test;
    friend class TestShellFixture_WriteFail_Test;
    friend class TestShellFixture_FullWritePass_Test;
    friend class TestShellFixture_FullWriteFail_Test;
    friend class TestShellFixture_FullWriteAndReadComparePass_Test;
    friend class TestShellFixture_FullWriteAndReadCompareFail_Test;
    friend class TestShellFixture_PartialLBAWriteWithDataPass_Test;
    friend class TestShellFixture_PartialLBAWriteWriteFail_Test;
    friend class TestShellFixture_WriteReadAgingPass_Test;
    friend class TestShellFixture_WriteReadAgingFail_Test;
    friend class TestShellFixture_PartialLBAWriteVerifyFail_Test;
    friend class TestShellFixture_EraseWithSize_Test;
    friend class TestShellFixture_EraseWithStartEnd_Test;

    SSDInterface* ssdAdapter;
    Logger& logger = Logger::getInstance();
    CommandFactory cmdCreator;
    const string CLASS_NAME = "TestShell";
};