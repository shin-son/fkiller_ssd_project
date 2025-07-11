#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "ssd_interface.h"
#include "logger.h"

using std::string;

class SSDAdapter : public SSDInterface {
public:
    string read(const int LBA) override;
    string write(const int LBA, const string& data) override;
    string erase(const int LBA, const int size) override;

private:
    void executeSystemCall(const string& command);
    string readOutputFile();
    ;
    Logger& logger = Logger::getInstance();
    const string ERROR_CODE = "ERROR";
    const string SSD_EXECUTE_FILE_PATH = "..\\x64\\Release\\ssd.exe";
    const string SSD_OUTPUT_FILE_PATH = "..\\ssd_output.txt";
    const string CLASS_NAME = "SSDAdapter";
};