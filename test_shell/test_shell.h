#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "ssd_interface.h"

using std::string;
using std::vector;

class TestShell
{
public:
    TestShell() = default;
    void setSsdAdapter(SSDInterface* adapter);
    void runShell();
    void printHelp();    

private:
    string read(const int LBA);
    string write(const int LBA, const string& data);
    void exit();
    void help();
    void fullWrite(const std::string& data);
    void fullRead();
    void fullWriteAndReadCompare();
    void partialLBAWrite(const string& data);
    void writeReadAging();

    SSDInterface* ssdAdapter;
};