#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "ssd_interface.h"
#include "constants.h"

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
    int runCommand(std::string& command);
    string HandlePartialLbaWrite();
    void printHelp();

private:
    string read(const int LBA);
    string write(const int LBA, const string& data);
    void exit();
    void help();
    void fullWrite(const std::string& data);
    vector<string> fullRead();
    void fullWriteAndReadCompare();
    void partialLBAWrite(const string& data);
    void writeReadAging();

    friend class TestShellTest_Write_Pass_Test;
    friend class TestShellTest_Write_Fail_Test;
    friend class TestShellTest_FullWrite_Pass_Test;
    friend class TestShellTest_FullWrite_Fail_Test;

    SSDInterface* ssdAdapter;
    unsigned int SSD_SIZE = 100;
};