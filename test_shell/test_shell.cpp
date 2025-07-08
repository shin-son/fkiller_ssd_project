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

    if (command == "exit") {
        std::cout << "PROGRAM EXIT" << std::endl;
        { retFlag = 2; return retFlag; };
    }

    if (command == "help") {
        printHelp();
        { retFlag = 3; return retFlag; };
    }

    if (0 == command.find("2_")) {
        HandlePartialLbaWrite();
    }

    return retFlag;
}

void TestShell::HandlePartialLbaWrite()
{
    vector<int> lbaSequence = { 4, 0, 3, 1, 2 };
    for (int count = 0; count < WRITE_COUT_FOR_PARTIAL_LBA_WRITE; count++)
    {
        for (int lba : lbaSequence)
        {
            ssdAdapter->write(lba, INPUT_DATA_FOR_PARTIAL_LBA_WRITE);
        }
    }
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
