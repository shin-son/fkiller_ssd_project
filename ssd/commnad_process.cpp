#include "command_process.h"
#include "ssd_write.h"
#include "ssd_Read.h"
#include <iostream>

int CommandProcessor::process(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: ssd.exe [w/r] [args...]\n";
		return 1;
	}

	std::string cmd = argv[1];
	std::vector<std::string> args(argv + 2, argv + argc);

	if (isWriteCommand(cmd)) {
		if (isWriteValidArgument(args)) {
			executeWrite(args);
		}
		else {
			SsdWrite ssdWrite;
			ssdWrite.writeOutputFile("ERROR");
			return -2;
		}
	}
	else if (isReadCommand(cmd)) {
		if (isReadValidArgument(args)) {
			executeRead(args);
		}
		else {
			SsdWrite ssdWrite;
			ssdWrite.writeOutputFile("ERROR");
			return -2;
		}
	}
	else {
		std::cout << "Unknown command: " << cmd << "\n";
		return 1;
	}

	return 0;
}

bool CommandProcessor::isWriteCommand(const std::string& cmd) {
	return cmd == "w" || cmd == "W";
}

bool CommandProcessor::isWriteValidArgument(std::vector<std::string>& args) {
	if (args.size() != 2) {
		std::cout << "Usage: ssd.exe w <address> <value>\n";
		return false;
	}

	std::cout << args[0] << std::endl;
	std::cout << args[1] << std::endl;
	if (!patterChecker.isValidAddress(args[0]))  return false;
	if (!patterChecker.isValidMemoryValue(args[1]))  return false;

	return true;
}

bool CommandProcessor::isReadValidArgument(std::vector<std::string>& args) {
	if (args.size() != 1) {
		std::cout << "Usage: ssd.exe r <address>\n";
		return false;
	}

	std::cout << args[0] << std::endl;
	if (!patterChecker.isValidAddress(args[0]))  return false;
	return true;
}

bool CommandProcessor::isReadCommand(const std::string& cmd) {
	return cmd == "r" || cmd == "R";
}

void CommandProcessor::executeWrite(const std::vector<std::string>& args) {
	SsdWrite writer;
	writer.write(args[0], args[1]);
}

void CommandProcessor::executeRead(const std::vector<std::string>& args) {


}