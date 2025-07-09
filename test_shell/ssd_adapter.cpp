#include "ssd_adapter.h"

string SSDAdapter::read(const int LBA) {
    std::string command = SSD_EXECUTE_FILE_PATH + " read " + std::to_string(LBA);
    logger.print(CLASS_NAME, __func__, command);
    string result;
        try {
        executeSystemCall(command);
        result = readOutputFile();
    }
    catch (std::exception& e) {
        std::cout << "[SSD_ADAPTER] Read error" << e.what() << std::endl;
        result = ERROR_CODE;
    }
    return result;
}

string SSDAdapter::write(const int LBA, const string& data) {
    std::string command = SSD_EXECUTE_FILE_PATH + " write " + std::to_string(LBA) + " " + data;
    logger.print(CLASS_NAME, __func__, command);
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
    return "";
}

string SSDAdapter::readOutputFile() {
    logger.print(CLASS_NAME, __func__, "open " + SSD_OUTPUT_FILE_PATH);
    std::ifstream infile(SSD_OUTPUT_FILE_PATH);
    if (!infile.is_open()) {
        throw std::runtime_error("Output file open error");
    }

    string result((std::istreambuf_iterator<char>(infile)),
        std::istreambuf_iterator<char>());
    infile.close();
    return result;
}

void SSDAdapter::executeSystemCall(const std::string& command)
{
    logger.print(CLASS_NAME, __func__, "called");
    if (std::system(command.c_str()) != 0) {
        throw std::runtime_error("System call fail");
    }
}