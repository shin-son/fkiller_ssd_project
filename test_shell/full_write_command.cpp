#include "full_write_command.h"

NEXT_TEST FullWriteCommand::process(const string& command, std::istringstream& iss)
{
	std::string data;

	if (!(iss >> data)) {
		std::cout << "[Write] ERROR: Missing data" << std::endl;
		return NEXT_KEEP_GOING;
	}

	LOG_PRINT("called");
	for (int i = 0; i < 100; ++i) {
		std::string result = adapter->write(i, data);
		if (result == "[Write] ERROR") {
			//LOG_PRINT("Failed at LBA " + std::to_string(i));
			std::cout << "[fullWrite] Failed at LBA " << i << std::endl;
			return NEXT_KEEP_GOING;
		}
	}
	std::cout << "[fullWrite] Done" << std::endl;

	return NEXT_KEEP_GOING;
}

void FullWriteCommand::printHelp()
{
	std::cout << " FULL Write - write value to all LBA\n" <<
		"\t usage - fullwrite <value> (ex. fullwrite 0xAAAABBBB)" << std::endl;
}