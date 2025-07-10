#include "read_command.h"

SSDInterface* ICommand::adapter = nullptr;

NEXT_TEST ReadCommand::process(const string& command, std::istringstream& iss)
{
	int LBA;
	std::istringstream iss_(command);
	std::cout << "[Read] ";

	if (!(iss >> LBA)) {
		std::cout << "ERROR: Missing LBA" << std::endl;
		return NEXT_KEEP_GOING;
	}

	string result = adapter->read(LBA);

	if (result == ERROR)
	{
		std::cout << ERROR;
	}
	else
	{
		std::cout << "LBA " << std::setfill('0') << std::setw(2)
			<< LBA << " : " << result << std::endl;
	}


	return NEXT_KEEP_GOING;
}

void ReadCommand::printHelp() 
{
	std::cout << " READ - read one LBA (Logical Block Addressing) \n" <<
		"\t usage - read <LBA>(ex.read 0)" << std::endl;
}