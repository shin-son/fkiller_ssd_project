#include "write_command.h"

NEXT_TEST WriteCommand::process(const string& command, std::istringstream& iss)
{
	//std::istringstream iss(command);
	string printlog = "[Write] ";

	int lba;
	std::string data;
	std::string extra;

	if (!(iss >> lba)) {
		printlog += "ERROR: Missing lba";
		return NEXT_KEEP_GOING;
	}

	if (!(iss >> data)) {
		printlog += "ERROR: Missing data";
		return NEXT_KEEP_GOING;
	}

	if (iss >> extra) {
		printlog += "ERROR: Too many arguments";
		return NEXT_KEEP_GOING;
	}

	std::string result = adapter->write(lba, data);
	if (result == "") printlog = WRITE_DONE_RETURN;
	else printlog += "ERROR";

	std::cout << printlog << std::endl;

	return NEXT_KEEP_GOING;
}

void WriteCommand::printHelp() 
{
	std::cout << " WRITE - write value to LBA(Logical Block Addressing) \n" <<
		"\t usage - write <LBA> <value> (ex.write 3 0xAAAABBBB)" << std::endl;
}