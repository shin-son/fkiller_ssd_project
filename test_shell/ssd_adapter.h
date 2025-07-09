#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "ssd_interface.h"
#include "gmock/gmock.h"

using std::string;

class SSDAdapter : public SSDInterface {
public:
    string read(const int LBA);
    string write(const int LBA, const string& data);

private:
    void executeSystemCall(const string& command);
    string readOutputFile();
};