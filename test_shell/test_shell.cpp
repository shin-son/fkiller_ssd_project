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

    if ((TEST_SCRIPT_2_FULL_COMMAND_NAME == command)
            || (TEST_SCRIPT_2_SHORT_COMMAND_NAME == command))
    {
        if ("" == HandlePartialLbaWrite()) { retFlag = 3; }
        else { retFlag = 2; }
        
        return retFlag;
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

    if (cmd == "read") {
        int LBA;

        if (!(iss >> LBA)) {
            std::cout << "[Read] ERROR: Missing LBA" << std::endl;
            return 1;
        }

        string result = read(LBA);
        std::cout << result << std::endl;

        if (result == "[Read] ERROR") return 1;
        return 3;
    }

    if (cmd == "fullread") {
        retFlag = 3;
        vector<string> fullResult = fullRead();
        for (string oneResult : fullResult) {
            std::cout << oneResult << std::endl;
            if (oneResult == "[Read] ERROR") retFlag = 1;
        }
        return retFlag;
    }

    return retFlag;
}

string TestShell::HandlePartialLbaWrite()
{
    string ret = "";
    vector<int> lbaSequence = { 4, 0, 3, 1, 2 };

    for (int count = 0; count < LOOP_COUT_FOR_PARTIAL_LBA_WRITE; count++)
    {
        for (int lba : lbaSequence)
        {
            if ("[Write] Done" == write(lba, INPUT_DATA_FOR_PARTIAL_LBA_WRITE))
            {
                continue;
            }
            else
            {
                std::cout << "[TestSecript2 - PartialLBAWrite] Write Fail" << std::endl;
                ret = "[TestSecript2] Error";
                return ret;
            }
        }
    }

    for (int lba : lbaSequence)
    {
        string exptected = "[Read] LBA " + std::to_string(lba) + " : " + string(INPUT_DATA_FOR_PARTIAL_LBA_WRITE);
        string actual = read(lba);

        if (exptected == actual)
        {
            continue;
        }
        else
        {
            std::cout << "[TestSecript2 - PartialLBAWrite] Verify Fail" << std::endl;
            ret = "[TestSecript2] Error";
            return ret;
        }
    }

    std::cout << "[TestSecript2 - PartialLBAWrite] Done" << std::endl;
    return ret;
}

void TestShell::printHelp()
{
    std::cout << "--------------------------------- HELP "
        << "---------------------------------" << std::endl;
    std::cout << " READ - read one LBA (Logical Block Addressing) \n" <<
        "\t usage - READ <LBA>(ex.read 0)" << std::endl;
    std::cout << " WRITE - write value to LBA(Logical Block Addressing) \n" <<
        "\t usage - WRITE <LBA> <value> (ex.write 3 0xAAAABBBB)" << std::endl;
    std::cout << " Test script - 2 (repeats following steps 30 times) \n" <<
        "\t  step1) write the data to lba 4\n" <<
        "\t  step2) write the data to lba 0\n" <<
        "\t  step3) write the data to lba 3\n" <<
        "\t  step4) write the data to lba 1\n" <<
        "\t  step5) write the data to lba 2\n" <<
        "\t  step6) Check if dat of all LBA 0 to 4 are the same\n" <<
        "\t usage - 2_PartialLBAWrite(or 2_)" << std::endl;
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

string TestShell::read(const int LBA)
{
    string result = ssdAdapter->read(LBA);
    if (result == "ERROR") return "[Read] ERROR";
    return "[Read] LBA " + std::to_string(LBA)+" : " + result;
}

vector<string> TestShell::fullRead()
{
    vector<string> result;
    for (int LBA = 0; LBA < SSD_SIZE; LBA++) {
        result.push_back(read(LBA));
    }
    return result;
}
