#include "logger.h"

void Logger::print(const string& className, const string& funcName, const string& logMessage) {
    const string caller = className + "." + funcName + "()";
    string oneLineLog = makeOneLineLog(caller, logMessage);
    std::ofstream file(LOG_FILE, std::ios::app);
    if (file.is_open()) {
        file << oneLineLog;
        file.close();
    }
    else {
        std::cout << "Rename failed: Log File\n";
    }

    checkLogFileSize();
}

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

    checkLogFileSize();
}

inline string Logger::makeOneLineLog(const string& caller, const string& logMessage)
{
    return makeLogTimeFormat(getTime()) + " " + caller + "\t: " + logMessage + "\n";
}

void Logger::checkLogFileSize()
{
    try {
        auto fileSize = fs::file_size(LOG_FILE);
        if (fileSize > MAX_LOG_SIZE) {
            fs::rename(LOG_FILE, makeLogFileFormat(getTime()));
            compressIfTooManyLogFile();
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
    return "until_" + time.year + time.month + time.day + "_" + time.hour +
        "h_" + time.minunte + "m_" + time.second + "s.log";
}

string Logger::make2DigitString(const int number) {
    if (number < 10) return "0" + std::to_string(number);
    return std::to_string(number);
}

void Logger::compressIfTooManyLogFile() {

    std::vector<fs::directory_entry> logFiles = findLogFiles();

    if (logFiles.size() > 1) {
        fs::directory_entry oldestLogFile = getOldestLogFile(logFiles);

        fs::path oldPath = oldestLogFile.path();
        fs::path newPath = oldPath;
        newPath.replace_filename(oldPath.stem().string() + ".zip");

        try {
            fs::rename(oldPath, newPath);
        }
        catch (const std::exception& e) {
            std::cerr << "Compress failed: " << e.what() << "\n";
        }
    }
}

std::vector<fs::directory_entry> Logger::findLogFiles()
{
    std::vector<fs::directory_entry> logFiles;
    for (const auto& entry : fs::directory_iterator("./")) {
        if (entry.path().extension() == ".log") {
            logFiles.push_back(entry);
        }
    }
    return logFiles;
}

fs::directory_entry Logger::getOldestLogFile(std::vector<fs::directory_entry>& logFiles) {
    std::sort(logFiles.begin(), logFiles.end(),
        [](const fs::path& a, const fs::path& b) {
            return a.filename().string() < b.filename().string();
        });

    return logFiles[0];
}