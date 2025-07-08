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

        if (command == "exit") {
            std::cout << "PROGRAM EXIT" << std::endl;
            break;
        }

        if (command == "help") {
            printHelp();
            continue;
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
