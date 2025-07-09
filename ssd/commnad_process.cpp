#include "command_process.h"
#include "ssd_write.h"
#include "ssd_Read.h"
#include "ssd_constants.h"
#include <iostream>

int CommandProcessor::process(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Usage: ssd.exe [w/r] [args...]\n";
		return -1;
	}
	std::string cmd = argv[1];
	std::vector<std::string> args(argv + 2, argv + argc);

	return dispatchCommand(cmd, args);
}
int CommandProcessor::dispatchCommand(const std::string& cmd, const std::vector<std::string>& args) {
	if (isWriteCommand(cmd)) {
		return handleWrite(args);
	}
	if (isReadCommand(cmd)) {
		return handleRead(args);
	}
}

int CommandProcessor::handleWrite(const std::vector<std::string>& args) {
	if (!isWriteValidArgument(args)) {
		printErrorAndWriteToOutput();
		return INVALID_ARGUMENT;
	}
	this->ssdOperator = WRITE_OPERATION;
	this->address = std::stoi(args[0]);
	this->memoryValue = args[1];

	return SUCCESS;
}

int CommandProcessor::handleRead(const std::vector<std::string>& args) {
	if (!isReadValidArgument(args)) {
		printErrorAndWriteToOutput();
		return INVALID_ARGUMENT;
	}

	this->ssdOperator = READ_OPERATION;
	this->address = std::stoi(args[0]);
	return SUCCESS;
}

bool CommandProcessor::isWriteCommand(const std::string& cmd) {
	return cmd == "w" || cmd == "W";
}

bool CommandProcessor::isWriteValidArgument(const std::vector<std::string>& args) {
	if (args.size() != 2) return false;
	return patterChecker.isValidAddress(args[0]) && patterChecker.isValidMemoryValue(args[1]);
}

bool CommandProcessor::isReadValidArgument(const std::vector<std::string>& args) {
	if (args.size() != 1) return false;
	return patterChecker.isValidAddress(args[0]);
}

bool CommandProcessor::isReadCommand(const std::string& cmd) {
	return cmd == "r" || cmd == "R";
}

void CommandProcessor::printErrorAndWriteToOutput() {
	SsdWrite writer;
	writer.writeOutputFile(ERROR_STRING);
}

int CommandProcessor::getOperator() {
	return this->ssdOperator;
}

int CommandProcessor::getAddress() {
	return this->address;
}

std::string CommandProcessor::getInputValue() {
	return this->memoryValue;
}