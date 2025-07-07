#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

class TestShell
{
public:
    TestShell() = default;
    void runCommand(const string& command);

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
};