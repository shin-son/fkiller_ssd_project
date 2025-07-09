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
    void print(const string& caller, const string& logMessage) {
        string logTime = makeLogTimeFormat(getTime());
        string oneLineLog = logTime + " " + caller + "\t: " + logMessage + "\n";
        std::ofstream file(LOG_FILE, std::ios::app);
        if (file.is_open()) {
            file << oneLineLog;
            file.close();
        }
        else {
            std::cout << "Rename failed: Log File\n";
        }

        try {
            auto fileSize = std::filesystem::file_size(LOG_FILE);
            if (fileSize > MAX_LOG_SIZE) {
                std::filesystem::rename(LOG_FILE, makeLogFileFormat(getTime()));
                checkLogFilesOver();
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Log rotation error: " << e.what() << std::endl;
        }
    }

private:
    TimeData getTime() {
        time_t timer;
        struct tm t;
        timer = time(NULL);
        localtime_s(&t, &timer);

        TimeData time;
        time.year = make2DigitString(t.tm_year - 100);
        time.month = make2DigitString(t.tm_mon + 1);
        time.day = make2DigitString(t.tm_mday);
        time.hour = make2DigitString(t.tm_hour);
        time.minunte = make2DigitString(t.tm_min);
        time.second = make2DigitString(t.tm_sec);

        return time;
    }

    string makeLogTimeFormat(const TimeData& time) {
        return "[" + time.year + "." + time.month + "." + time.day + " " +
            time.hour + ":" + time.minunte + "]";
    }

    string makeLogFileFormat(const TimeData& time) {
        std::cout << "log file size overflow\n";
        return "until_" + time.year + time.month + time.day + "_" + time.hour + "h_" + time.minunte + "m_" + time.second + "s.log";
    }

    string make2DigitString(const int number) {
        if (number < 10) return "0" + std::to_string(number);
        return std::to_string(number);
    }

    void checkLogFilesOver() {
        std::vector<std::filesystem::directory_entry> logFiles;

        for (const auto& entry : std::filesystem::directory_iterator("./")) {
            if (entry.path().extension() == ".log") {
                logFiles.push_back(entry);
            }
        }

        if (logFiles.size() > 1) {
            // 수정 시간 기준으로 정렬
            std::sort(logFiles.begin(), logFiles.end(), [](const auto& a, const auto& b) {
                return std::filesystem::last_write_time(a) < std::filesystem::last_write_time(b);
                });

            // 3. 가장 오래된 파일의 이름 변경
            std::filesystem::path oldPath = logFiles[0].path();
            std::filesystem::path newPath = oldPath;
            newPath.replace_filename(oldPath.stem().string() + ".zip");

            try {
                std::filesystem::rename(oldPath, newPath);
            }
            catch (const std::exception& e) {
                std::cerr << "Rename failed: " << e.what() << "\n";
            }
        }
    }

    const string LOG_FILE = "latest.log";
    const unsigned int SIZE_1KB = 1024;
    const unsigned int MAX_LOG_SIZE = 10 * SIZE_1KB;
};