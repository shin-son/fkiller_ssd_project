#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "ssd_interface.h"
#include "constants.h"

using std::string;
using std::vector;

enum TEST_NEXT
{
    NEXT_KEEP_GOING = 0,
    NEXT_EXIT = 2,
    NEXT_MAX = 0xFFFFFFFF,
};

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
    int runCommand(std::string& command);

private:
    void printHelp();
    string read(const int LBA);
    string write(const int LBA, const string& data);
    void exit();
    void help();
    void fullWrite(const std::string& data);
    void fullRead();
    void fullWriteAndReadCompare();
    void partialLBAWrite(const string& data = INPUT_DATA_FOR_PARTIAL_LBA_WRITE);
    bool writeTheSequence(const std::vector<int>& lbaSequence, const std::string& data);
    bool verifyTheSequence(const std::string& data, const vector<int>& lbaSequence);
    void writeReadAging();
    string intToHexString(int value);

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

    SSDInterface* ssdAdapter;
    unsigned int SSD_SIZE = 100;
};