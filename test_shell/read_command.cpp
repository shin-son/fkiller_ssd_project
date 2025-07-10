#include "read_command.h"

SSDInterface* ICommand::adapter = nullptr;

NEXT_TEST ReadCommand::process(const string& command) 
{
	int LBA;
	std::istringstream iss(command);
	std::cout << "[Read] ";

	if (!(iss >> LBA)) {
		std::cout << "ERROR: Missing LBA" << std::endl;
		return NEXT_KEEP_GOING;
	}

	string result = adapter->read(LBA);

	std::cout << result << std::endl;

	return NEXT_KEEP_GOING;
}

void ReadCommand::printHelp() 
{
	std::cout << " READ - read one LBA (Logical Block Addressing) \n" <<
		"\t usage - read <LBA>(ex.read 0)" << std::endl;
}