#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "ssd_interface.h"

using std::string;

class SSDAdapter : public SSDInterface {
public:
    string read(const int LBA);
    string write(const int LBA, const string& data);

private:
    void executeSystemCall(const string& command);
    string readOutputFile();
    const string ERROR_CODE = "ERROR";
    const string SSD_EXECUTE_FILE_PATH = "..\\x64\\Release\\ssd.exe";
    const string SSD_OUTPUT_FILE_PATH = "..\\ssd\\ssd_output.txt";
};