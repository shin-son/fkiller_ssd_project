#include "full_read_command.h"

NEXT_TEST FullReadCommand::process(const string& command, std::istringstream& iss)
{
	printLog(getCommandName());

	for (int LBA = 0; LBA < SSD_SIZE; LBA++) {
		string result = adapter->read(LBA);
		if (result != "ERROR")
		{
			result = ("LBA " + std::to_string(LBA) + " : " + result);
			std::cout << result << std::endl;
		}
		else
		{
			result = ("LBA " + std::to_string(LBA) + " : " + ERROR);
			printLog(result);
		}
	}

	return NEXT_KEEP_GOING;
}

void FullReadCommand::printHelp()
{
	std::cout << " FULL READ - read all LBA \n" <<
		"\t usage - fullread" << std::endl;
}
