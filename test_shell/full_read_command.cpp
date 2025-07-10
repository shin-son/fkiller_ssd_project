#include "full_read_command.h"

NEXT_TEST FullReadCommand::process(const string& command, std::istringstream& iss)
{
	LOG_PRINT("called");
	std::cout << "[fullRead]\n";
	for (int LBA = 0; LBA < SSD_SIZE; LBA++) {
		string result = adapter->read(LBA);
		if (result != "ERROR") result = ("LBA " + std::to_string(LBA) + " : " + result);
		else result = ("LBA " + std::to_string(LBA) + " : " + ERROR);
		std::cout << result << std::endl;
	}
	return NEXT_KEEP_GOING;
}

void FullReadCommand::printHelp()
{
	std::cout << " FULL READ - read all LBA \n" <<
		"\t usage - fullread" << std::endl;
}
