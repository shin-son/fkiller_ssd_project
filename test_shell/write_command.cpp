#include "write_command.h"

void WriteCommand::printHelp() 
{
	std::cout << " WRITE - write value to LBA(Logical Block Addressing) \n" <<
		"\t usage - write <LBA> <value> (ex.write 3 0xAAAABBBB)" << std::endl;
}

bool WriteCommand::prepare(std::istringstream& iss)
{
	logMessage = getErrorHeader();

	if (!(iss >> lba)) {
		logMessage += ": Missing lba";
		return false;
	}

	if (!(iss >> data)) {
		logMessage += ": Missing data";
		return false;
	}

	string extra;
	if (iss >> extra) {
		logMessage += + ": Too many arguments";
		return false;
	}

	return true;
}

bool WriteCommand::execute()
{
	std::string result = cmdRequester->write(lba, data);
	if (result == "") return true;
	else logMessage = getErrorHeader() + ": Write Fail";
	return false;
}

void WriteCommand::wrapUp(bool noError)
{
	if (noError) logMessage = getDoneMessage();
	printLog(logMessage);
}