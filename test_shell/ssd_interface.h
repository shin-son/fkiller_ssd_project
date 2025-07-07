#pragma once
#include <string>

using std::string;

class SSDInterface {
    virtual string read(const int LBA) = 0;
    virtual string write(const int LBA, const string& data) = 0;
};