#include "write_command.h"

NEXT_TEST WriteCommand::process(std::istringstream& iss)
{
	int lba;
	std::string data;
	std::string extra;

	if (!(iss >> lba)) {
		printLog(getErrorHeader() + ": Missing lba");
		return NEXT_KEEP_GOING;
	}

	if (!(iss >> data)) {
		printLog(getErrorHeader() + ": Missing data");
		return NEXT_KEEP_GOING;
	}

	if (iss >> extra) {
		printLog(getErrorHeader() + ": Too many arguments");
		return NEXT_KEEP_GOING;
	}

	std::string result = cmdRequester->write(lba, data);
	if (result == "") printLog(getDoneMessage());
	else printLog(getErrorHeader() + ": Write Fail");

	return NEXT_KEEP_GOING;
}

void WriteCommand::printHelp() 
{
	std::cout << " WRITE - write value to LBA(Logical Block Addressing) \n" <<
		"\t usage - write <LBA> <value> (ex.write 3 0xAAAABBBB)" << std::endl;
}