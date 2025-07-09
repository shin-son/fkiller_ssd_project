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
        if (retFlag == NEXT_EXIT) break;
    }
}

int TestShell::runCommand(const std::string& command)
{
    int retFlag = NEXT_KEEP_GOING;
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "exit") {
        std::cout << "PROGRAM EXIT" << std::endl;
        return NEXT_EXIT;
    }

    if (cmd == "help") {
        printHelp();
        return NEXT_KEEP_GOING;
    }

    if (cmd == "1_" || cmd == "1_FullWriteAndReadCompare") {
        fullWriteAndReadCompare();
        return NEXT_KEEP_GOING;
    }

    if ((cmd == "2_PartialLBAWrite") || (cmd == "2_")){
        partialLBAWrite();
        return NEXT_KEEP_GOING;
    }

    if (("3_WriteReadAging" == command) || ("3_" == command)) {
        writeReadAging();
        return NEXT_KEEP_GOING;
    }

    if (cmd == "write") {
        int lba;
        std::string data;
        std::string extra;
        if (!(iss >> lba)) {
            std::cout << "[Write] ERROR: Missing lba" << std::endl;
            return NEXT_KEEP_GOING;
        }

        if (!(iss >> data)) {
            std::cout << "[Write] ERROR: Missing data" << std::endl;
            return NEXT_KEEP_GOING;
        }

        if (iss >> extra) {
            std::cout << "[Write] ERROR: Too many arguments" << std::endl;
            return NEXT_KEEP_GOING;
        }

        std::string result = write(lba, data);
        std::cout << result << std::endl;
        return NEXT_KEEP_GOING;
    }

    if (cmd == "read") {
        int LBA;

        if (!(iss >> LBA)) {
            std::cout << "[Read] ERROR: Missing LBA" << std::endl;
            return NEXT_KEEP_GOING;
        }

        string result = read(LBA);

        if (result == "[Read] ERROR") return 1;
        return NEXT_KEEP_GOING;
    }

    if (cmd == "fullread") {
        fullRead();
        return NEXT_KEEP_GOING;
    }

    if (cmd == "erase")
    {
        eraseWithSize(iss);
        return NEXT_KEEP_GOING;
    }

    if (cmd == "erase_range")
    {
        int startLBA = 0;
        int endLBA = 0;

        iss >> startLBA;
        iss >> endLBA;

        return NEXT_KEEP_GOING;
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
        "\t  step6) Check if data of all LBA 0 to 4 are the same\n" <<
        "\t usage - 2_PartialLBAWrite(or 2_)" << std::endl;
    std::cout << " Test script - 3 (repeats following steps 200 times) \n" <<
        "\t  step1) write the data to lba 0\n" <<
        "\t  step2) write the data to lba 99\n" <<
        "\t  step3) read the data to lba 0\n" <<
        "\t  step4) read the data to lba 99\n" <<
        "\t  step6) Check if data of LBA 0 and 99 is the same\n" <<
        "\t usage - 3_WriteReadAging(or 3_)" << std::endl;
    std::cout << "---------------------------------------"
        << "---------------------------------" << std::endl;
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

void TestShell::writeReadAging() {
    bool allMatch = true;

    for (int i = 0; i < 200; ++i) {
        std::stringstream ss;
        ss << "0x" << std::uppercase << std::hex << (rand() & 0xFFFFFFFF);
        std::string randData = ss.str();

        ssdAdapter->write(0, randData);
        ssdAdapter->write(99, randData);

        std::string result0 = ssdAdapter->read(0);
        std::string result99 = ssdAdapter->read(99);

        if (result0 != result99) {
            std::cout << "[Aging] ERROR mismatch value LBA[0] : " << result0 << " LBA[99] : " << result99 << std::endl;
            allMatch = false;
        }
    }

    if (allMatch) {
        std::cout << "[Aging] PASS" << std::endl;
    }
}

string TestShell::write(const int LBA, const string& data) {
    if (!ssdAdapter) return "[Write] ERROR";

    std::string result = ssdAdapter->write(LBA, data);
    if (result == "") return "[Write] Done";
    else return "[Write] ERROR";
}

string TestShell::read(const int LBA)
{
    string result = ssdAdapter->read(LBA);
    if (result == "ERROR") result = "[Read] ERROR";
    else result = "[Read] LBA " + std::to_string(LBA) + " : " + result;
    std::cout << result << std::endl;
    return result;
}

string TestShell::erase(const int LBA, const int size)
{
    string result = ssdAdapter->erase(LBA, size);
    if (result == "ERROR") result = "[Erase] ERROR";
    else result = "[Erase] Done";
    std::cout << result << std::endl;
    return result;
}

string TestShell::intToHexString(int value) {
    std::stringstream ss;
    ss << "0x"
        << std::setfill('0') << std::setw(8)
        << std::hex << std::uppercase
        << value;
    return ss.str();
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

void TestShell::eraseWithSize(std::istringstream& iss)
{
    int startLBA = 0;
    int size = 0;

    if (false == getEraseParameter(startLBA, size, iss)) return;

    int endLBA = startLBA + size - 1;
    if (false == isVaiidEraseRange(startLBA, endLBA))
    {
        std::cout << "[Erase] Error: invalid Range(startLBA, size)\n";
        return;
    }

    if (false == eraseRange(startLBA, endLBA))
    {
        std::cout << "[Erase] Error: Erase Operation Fail\n";
        return;
    }
    return;
}

bool TestShell::getEraseParameter(int& startLBA, int& size, std::istringstream& iss)
{
    if (!(iss >> startLBA))
    {
        std::cout << "[Erase] Error: missing startLBA\n";
        return false;
    }

    if (!(iss >> size))
    {
        std::cout << "[Erase] Error: missing size\n";
        return false;
    }

    return true;
}

bool TestShell::isVaiidEraseRange(const int startLBA, const int endLBA)
{
    if (startLBA < 0)  return false;
    else if (endLBA >= SSD_SIZE) return false;
    else if (startLBA >= endLBA) return false;

    return true;
}

bool TestShell::eraseRange(int startLBA, int endLBA)
{
    int retSuccess = true;
    string eraseResult = "";
    int eraseCount = (endLBA - startLBA + 1) / ERASE_UNIT_LBA_COUNT;
    eraseCount += ((endLBA - startLBA + 1) % ERASE_UNIT_LBA_COUNT) ? 1 : 0;
    
    for (int loopCount = 0; loopCount < eraseCount; loopCount++)
    {
        if (endLBA < startLBA + ERASE_UNIT_LBA_COUNT)
        {
            eraseResult = erase(startLBA, endLBA - startLBA + 1);
            break;
        }
        else
        {
            eraseResult = erase(startLBA, ERASE_UNIT_LBA_COUNT);
            startLBA += ERASE_UNIT_LBA_COUNT;
        }
    }

    if ("" != eraseResult) return false;    
    return true;
}