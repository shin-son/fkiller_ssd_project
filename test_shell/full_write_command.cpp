#include "full_write_command.h"

NEXT_TEST FullWriteCommand::process(std::istringstream& iss)
{
	std::string data;

	if (!(iss >> data)) {
		printLog(getErrorHeader() + ": Missing data");
		return NEXT_KEEP_GOING;
	}

	LOG_PRINT("called");
	for (int i = 0; i < 100; ++i) {
		std::string result = cmdRequester->write(i, data);
		if (result == "[Write] ERROR") {
			printLog(getErrorHeader() + ": Failed at LBA " + std::to_string(i));
			return NEXT_KEEP_GOING;
		}
	}

	printLog(getDoneMessage());

	return NEXT_KEEP_GOING;
}

void FullWriteCommand::printHelp()
{
	std::cout << " FULL Write - write value to all LBA\n" <<
		"\t usage - fullwrite <value> (ex. fullwrite 0xAAAABBBB)" << std::endl;
}