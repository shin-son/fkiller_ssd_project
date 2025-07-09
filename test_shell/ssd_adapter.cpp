#include "ssd_adapter.h"

string SSDAdapter::read(const int LBA) {
    std::string command = "./x64/Release/ssd.exe read " + std::to_string(LBA);
    string result;
    try {
        executeSystemCall(command);
        result = readOutputFile();
    }
    catch (std::exception& e) {
        result = "ERROR";
    }
    return result;
}

string SSDAdapter::write(const int LBA, const string& data) {
    std::string command = "./x64/Release/ssd.exe write " + std::to_string(LBA) + " " + data;
    string result;
    try {
        executeSystemCall(command);
        result = readOutputFile();
    }
    catch (std::exception& e) {
        result = "ERROR";
    }
    return result;
}

string SSDAdapter::readOutputFile() {
    std::ifstream infile("nand_output.txt");
    if (!infile.is_open()) {
        throw std::runtime_error("nand_output.txt 파일을 열 수 없음");
    }

    string result((std::istreambuf_iterator<char>(infile)),
        std::istreambuf_iterator<char>());
    infile.close();
    return result;

}

void SSDAdapter::executeSystemCall(const std::string& command)
{
    if (std::system(command.c_str()) != 0) {
        throw std::runtime_error("System Call Fail");
    }
}