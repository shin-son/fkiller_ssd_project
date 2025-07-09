#include "logger.h"

void Logger::print(const string& caller, const string& logMessage) {
    string oneLineLog = makeOneLineLog(caller, logMessage);
    std::ofstream file(LOG_FILE, std::ios::app);
    if (file.is_open()) {
        file << oneLineLog;
        file.close();
    }
    else {
        std::cout << "Rename failed: Log File\n";
    }

    checkLogFIleSize();
}

inline string Logger::makeOneLineLog(const string& caller, const string& logMessage)
{
    return makeLogTimeFormat(getTime()) + " " + caller + "\t: " + logMessage + "\n";
}

void Logger::checkLogFIleSize()
{
    try {
        auto fileSize = std::filesystem::file_size(LOG_FILE);
        if (fileSize > MAX_LOG_SIZE) {
            std::filesystem::rename(LOG_FILE, makeLogFileFormat(getTime()));
            ifTooManyLogFileCompress();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Log rotation error: " << e.what() << std::endl;
    }
}

TimeData Logger::getTime() {
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

string Logger::makeLogTimeFormat(const TimeData& time) {
    return "[" + time.year + "." + time.month + "." + time.day + " " +
        time.hour + ":" + time.minunte + "]";
}

string Logger::makeLogFileFormat(const TimeData& time) {
    return "until_" + time.year + time.month + time.day + "_" + time.hour + "h_" + time.minunte + "m_" + time.second + "s.log";
}

string Logger::make2DigitString(const int number) {
    if (number < 10) return "0" + std::to_string(number);
    return std::to_string(number);
}

void Logger::ifTooManyLogFileCompress() {
    std::vector<std::filesystem::directory_entry> logFiles;

    for (const auto& entry : std::filesystem::directory_iterator("./")) {
        if (entry.path().extension() == ".log") {
            logFiles.push_back(entry);
        }
    }

    if (logFiles.size() > 1) {
        std::sort(logFiles.begin(), logFiles.end(), [](const std::filesystem::path& a, const std::filesystem::path& b) {
            return a.filename().string() < b.filename().string();
            });

        std::filesystem::path oldPath = logFiles[0].path();
        std::filesystem::path newPath = oldPath;
        newPath.replace_filename(oldPath.stem().string() + ".zip");

        try {
            std::filesystem::rename(oldPath, newPath);
        }
        catch (const std::exception& e) {
            std::cerr << "Compress failed: " << e.what() << "\n";
        }
    }
}
