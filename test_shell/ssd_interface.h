#pragma once
#include <iostream>
#include <string>

using std::string;

class SSDInterface {
public:
    virtual string read(const int LBA) = 0;
    virtual string write(const int LBA, const string& data) = 0;
    virtual string erase(const int LBA, const int size) = 0;
};