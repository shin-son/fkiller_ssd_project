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

    if (cmd == "write") {
        int lba;
        std::string data;
        std::string extra;
        if (!(iss >> lba)) {
            std::cout << "[Write] ERROR: Missing lba" << std::endl;
            return 3;
        }

        if (!(iss >> data)) {
            std::cout << "[Write] ERROR: Missing data" << std::endl;
            return 3;
        }

        if (iss >> extra) {
            std::cout << "[Write] ERROR: Too many arguments" << std::endl;
            return 3;
        }

        std::string result = write(lba, data);
        std::cout << result << std::endl;
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

string TestShell::write(const int LBA, const string& data) {
    if (!ssdAdapter) return "[Write] ERROR";

    std::string result = ssdAdapter->write(LBA, data);
    if (result == "") return "[Write] Done";
    else return "[Write] ERROR";
}

void TestShell::fullWrite(const string& data) {
    for (int i = 0; i < 100; ++i) {
        std::string result = write(i, data);
        if (result == "[Write] ERROR") {
            std::cout << "[fullWrite] Failed at LBA " << i << std::endl;
            return;
        }
    }
    std::cout << "[fullWrite] Done" << std::endl;
}
