#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <filesystem>

using std::string;
namespace fs = std::filesystem;

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
    void print(const string& className, const string& funcName, const string& logMessage);
    void print(const string& caller, const string& logMessage);

private:
    string makeOneLineLog(const string& caller, const string& logMessage);
    void checkLogFileSize();
    TimeData getTime();
    string makeLogTimeFormat(const TimeData& time);
    string makeLogFileFormat(const TimeData& time);
    string make2DigitString(const int number);
    void compressIfTooManyLogFile();

    std::vector<std::filesystem::directory_entry> findLogFiles();
    fs::directory_entry getOldestLogFile(std::vector<fs::directory_entry>& logFiles);

    const string LOG_FILE = "latest.log";
    const unsigned int SIZE_1KB = 1024;
    const unsigned int MAX_LOG_SIZE = 10 * SIZE_1KB;
};