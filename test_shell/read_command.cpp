#include "read_command.h"

void ReadCommand::printHelp() 
{
	std::cout << " READ - read one LBA (Logical Block Addressing) \n" <<
		"\t usage - read <LBA>(ex.read 0)" << std::endl;
}

bool ReadCommand::prepare(std::istringstream& iss)
{
	if (!(iss >> lba)) {
		logMessage = getErrorHeader() + ": Missing lba";
		return false;
	}
	return true;
}

bool ReadCommand::execute()
{
	string result = cmdRequester->read(lba);
	if (result == ERROR)
	{
		logMessage = getErrorHeader() + ": Read Fail";
		return false;
	}

	std::ostringstream oss;
	oss << "LBA " << std::setfill('0') << std::setw(2)
		<< lba << " : " << result << std::endl;
	printLog(getCommandName() + oss.str());
	return true;
}

void ReadCommand::wrapUp(bool noError)
{
	printLog(logMessage);
}