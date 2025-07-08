#include <regex>
#include <sstream>
#include "test_shell.h"

void TestShell::setSsdAdapter(SSDInterface* adapter)
{
    ssdAdapter = adapter;
}

void TestShell::runShell() {
    std::string command;
    while (true) {
        std::cout << " SHELL> ";
        std::getline(std::cin, command);

        int retFlag = runCommand(command);
        if (retFlag == 2) break;
        if (retFlag == 3) continue;
    }
}

int TestShell::runCommand(std::string& command)
{
    int retFlag = 1;
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "exit") {
        std::cout << "PROGRAM EXIT" << std::endl;
        { retFlag = 2; return retFlag; };
    }

    if (cmd == "help") {
        printHelp();
        { retFlag = 3; return retFlag; };
    }

    if (cmd == "read") {
        string LBA;

        if (!(iss >> LBA)) {
            std::cout << "[Read] ERROR: Missing lba" << std::endl;
            return 1;
        }

        string result = read(stoi(LBA));
        std::cout << result << std::endl;

        if (result == "[Read] ERROR") return 1;
        return 3;
    }

    if (cmd == "fullread") {
        vector<string> result = fullRead();
        for (string one_result : result) {
            std::cout << one_result << std::endl;
            if (one_result == "[Read] ERROR") return 1;
        }
        return 3;
    }

    return retFlag;
}

void TestShell::printHelp()
{
    std::cout << "--------------------------------- HELP "
        << "---------------------------------" << std::endl;
    std::cout << " READ - read one LBA (Logical Block Addressing) \n" << 
        "\t usage - READ <LBA>(ex.read 0)" << std::endl;
    std::cout << " WRITE - write value to LBA(Logical Block Addressing) \n" <<
        "\t usage - WRITE <LBA> <value> (ex.write 3 0xAAAABBBB)" << std::endl;
    std::cout << "---------------------------------------"
        << "---------------------------------" << std::endl;
}

string TestShell::read(const int LBA)
{
    string result = ssdAdapter->read(LBA);
    if (result == "ERROR") return "[Read] ERROR";
    return "[Read] LAB " + std::to_string(LBA)+" : " + result;
}

vector<string> TestShell::fullRead()
{
    vector<string> result;
    for (int i = 0; i < 100; i++) {
        result.push_back(read(i));
    }
    return result;
}
