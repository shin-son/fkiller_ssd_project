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
    void printHelp();

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

    friend class TestShellTest_Write_Pass_Test;
    friend class TestShellTest_Write_Fail_Test;
    friend class TestShellTest_FullWrite_Pass_Test;
    friend class TestShellTest_FullWrite_Fail_Test;
    friend class TestShellTest_PartialLBAWrite_WithData_Pass_Test;
    friend class TestShellTest_PartialLBAWrite_Write_Fail_Test;
    friend class TestShellTest_FullWriteAndReadCompare_Pass_Test;
    friend class TestShellTest_FullWriteAndReadCompare_Fail_Test;
    friend class TestShellTest_ReadPass_Test;
    friend class TestShellTest_ReadFailWrongLBA_Test;
    friend class TestShellTest_FullReadPass_Test;
    friend class TestShellTest_FullReadFail_Test;

    SSDInterface* ssdAdapter;
    unsigned int SSD_SIZE = 100;
};