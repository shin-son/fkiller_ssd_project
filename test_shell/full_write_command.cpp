#include "full_write_command.h"

void FullWriteCommand::printHelp()
{
	std::cout << " FULL Write - write value to all LBA\n" <<
		"\t usage - fullwrite <value> (ex. fullwrite 0xAAAABBBB)" << std::endl;
}

bool FullWriteCommand::prepare(std::istringstream& iss)
{
	if (!(iss >> data)) {
		logMessage = getErrorHeader() + ": Missing data";
		return false;
	}
	return true;
}

bool FullWriteCommand::execute()
{
	for (int lba = 0; lba < SSD_SIZE; ++lba) {
		std::string result = cmdRequester->write(lba, data);
		if (result == ERROR){
			printLog(getErrorHeader() + ": Failed at LBA " + std::to_string(lba));
			return false;
		}
	}
	return true;
}

void FullWriteCommand::wrapUp(bool noError)
{
	if (noError) logMessage = getDoneMessage();
	printLog(logMessage);
}