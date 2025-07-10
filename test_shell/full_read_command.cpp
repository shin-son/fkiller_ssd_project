#include "full_read_command.h"

void FullReadCommand::printHelp()
{
	std::cout << " FULL READ - read all LBA \n" <<
		"\t usage - fullread" << std::endl;
}

bool FullReadCommand::prepare(std::istringstream& iss)
{
	printLog(getCommandName());
	return true;
}

bool FullReadCommand::execute()
{
	for (int LBA = 0; LBA < SSD_SIZE; LBA++) {
		string result = cmdRequester->read(LBA);
		if (result != ERROR)
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
	return true;
}

void FullReadCommand::wrapUp(bool noError)
{

}