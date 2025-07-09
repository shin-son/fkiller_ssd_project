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

    if (cmd == "1_" || cmd == "1_FullWriteAndReadCompare") {
        fullWriteAndReadCompare();
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

        if (result == "[Read] ERROR") return 1;
        return 3;
    }

    if (cmd == "fullread") {
        fullRead();
        return 3;
    }

    if ((TEST_SCRIPT_2_FULL_COMMAND_NAME == command)
        || (TEST_SCRIPT_2_SHORT_COMMAND_NAME == command))
    {
        partialLBAWrite();
        return NEXT_KEEP_GOING;
    }

    // Reaching here means that the command is invalid.
    std::cout << INVALID_COMMAND_MSG << std::endl;

    return retFlag;
}

void TestShell::printHelp()
{
    std::cout << "--------------------------------- HELP "
        << "---------------------------------" << std::endl;
    std::cout << " READ - read one LBA (Logical Block Addressing) \n" <<
        "\t usage - read <LBA>(ex.read 0)" << std::endl;
    std::cout << " WRITE - write value to LBA(Logical Block Addressing) \n" <<
        "\t usage - write <LBA> <value> (ex.write 3 0xAAAABBBB)" << std::endl;
    std::cout << " Test script - 1 FullWriteAndReadCompare \n" <<
        "\t write and read Test all indices in 5-unit\n" <<
        "\t usage - 1_FullWriteAndReadCompare(or 1_)" << std::endl;
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
    if (result == "ERROR") result = "[Read] ERROR";
    else result = "[Read] LBA " + std::to_string(LBA)+" : " + result;
    std::cout << result << std::endl;
    return result;
}

void TestShell::fullRead()
{
    for (int LBA = 0; LBA < SSD_SIZE; LBA++) {
        read(LBA);
    }
}

void TestShell::fullWriteAndReadCompare()
{
    int j = 1;
    bool failFlag = false;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 5; j++) {
            auto ret = ssdAdapter->write(5 * i + j, intToHexString(i));
            if (ret != "") {
                failFlag = true;
                break;
            }
        }
        // TODO: fix to if (isFail()) break;
        if (failFlag == true) break;

        for (int j = 0; j < 5; j++) {
            if (intToHexString(i) != ssdAdapter->read(5 * i + j)) {
                failFlag = true;
                break;
            }
        }
        if (failFlag == true) break;
    }
    if (failFlag == true) {
        std::cout << "FAIL" << std::endl;
    }
    else {
        std::cout << "PASS" << std::endl;
    }
}

string TestShell::intToHexString(int value) {
    std::stringstream ss;
    ss << "0x"
        << std::setfill('0') << std::setw(8)
        << std::hex << std::uppercase
        << value;
    return ss.str();
}

void TestShell::partialLBAWrite(const string& data)
{
    vector<int> lbaSequence = INPUT_LBA_SEQUENCE;
    
    for (int count = 0; count < LOOP_COUNT_FOR_PARTIAL_LBA_WRITE; count++)
    {
        if (false == writeTheSequence(lbaSequence, data)) return;
    }

    if (false == verifyTheSequence(data, lbaSequence)) return;

    std::cout << TEST_SCRIPT_2_SUCCESS_MSG << std::endl;
}

bool TestShell::writeTheSequence(const std::vector<int>& lbaSequence, const std::string& data)
{
    for (int lba : lbaSequence)
    {
        if (RETURN_WRITE_DONE != write(lba, data))
        {
            std::cout << TEST_SCRIPT_2_WRITE_FAIL_MSG << std::endl;
            return false;
        }
    }
    return true;
}

bool TestShell::verifyTheSequence(
    const std::string& data, const vector<int>& lbaSequence)
{
    for (int lba : lbaSequence)
    {
        string exptected = "[Read] LBA " + std::to_string(lba) + " : " + data;
        if (exptected != read(lba))
        {
            std::cout << TEST_SCRIPT_2_VERIFY_FAIL_MSG << std::endl;
            return false;
        }
    }
    return true;
}