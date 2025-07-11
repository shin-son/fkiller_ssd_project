#include "ssd_adapter.h"

string SSDAdapter::read(const int LBA) {
    std::string command = SSD_EXECUTE_FILE_PATH + " r " + std::to_string(LBA);
    LOG_PRINT(command);
    string result;
        try {
        executeSystemCall(command);
        result = readOutputFile();
    }
    catch (std::exception& e) {
        std::cout << "[SSD_ADAPTER] Read error - " << e.what() << std::endl;
        result = ERROR_CODE;
    }
    return result;
}

string SSDAdapter::write(const int LBA, const string& data) {
    std::string command = SSD_EXECUTE_FILE_PATH + " w " + std::to_string(LBA) + " " + data;
    LOG_PRINT(command);
    string result;
    try {
        executeSystemCall(command);
        result = readOutputFile();
    }
    catch (std::exception& e) {
        std::cout << "[SSD_ADAPTER] Write error - " << e.what() << std::endl;
        result = ERROR_CODE;
    }
    return result;
}

string SSDAdapter::erase(const int LBA, const int size)
{
    std::string command = SSD_EXECUTE_FILE_PATH + " e " + std::to_string(LBA) + " " + std::to_string(size);
    LOG_PRINT(command);
    string result;
    try {
        executeSystemCall(command);
        result = readOutputFile();
    }
    catch (std::exception& e) {
        std::cout << "[SSD_ADAPTER] Erase error - " << e.what() << std::endl;
        result = ERROR_CODE;
    }
    return result;
}

string SSDAdapter::readOutputFile() {
    LOG_PRINT("open " + SSD_OUTPUT_FILE_PATH);
    std::ifstream infile(SSD_OUTPUT_FILE_PATH);
    if (!infile.is_open()) {
        throw std::runtime_error("Output file open error");
    }

    string result((std::istreambuf_iterator<char>(infile)),
        std::istreambuf_iterator<char>());
    infile.close();
    if (!result.empty() && result.back() == '\n') {
        result.erase(result.size() - 1);
    }
    return result;
}

void SSDAdapter::executeSystemCall(const std::string& command)
{
    LOG_PRINT("called");
    if (std::system(command.c_str()) != 0) {
        throw std::runtime_error("System call fail");
    }
}