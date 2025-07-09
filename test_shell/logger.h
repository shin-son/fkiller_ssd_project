#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <filesystem>

using std::string;

struct TimeData {
    string year;
    string month;
    string day;
    string hour;
    string minunte;
    string second;
};

class Logger {
public:
    void print(const string& caller, const string& logMessage);

private:
    string makeOneLineLog(const string& caller, const string& logMessage);
    void checkLogFIleSize();
    TimeData getTime();
    string makeLogTimeFormat(const TimeData& time);
    string makeLogFileFormat(const TimeData& time);
    string make2DigitString(const int number);
    void ifTooManyLogFileCompress();

    const string LOG_FILE = "latest.log";
    const unsigned int SIZE_1KB = 10;
    const unsigned int MAX_LOG_SIZE = 10 * SIZE_1KB;
};