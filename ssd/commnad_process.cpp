#include "command_process.h"
#include "ssd_write.h"
#include "ssd_Read.h"
#include "ssd_constants.h"
#include <iostream>
#include <sstream>
#include <iomanip>

int CommandProcessor::process(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Usage: ssd.exe [w/r] [args...]\n";
		return INVALID_COMMAND;
	}
	std::string cmd = argv[1];
	std::vector<std::string> args(argv + 2, argv + argc);

	return dispatchCommand(cmd, args);
}

int CommandProcessor::flushProcess(const std::vector<std::string>& args) {
	if (args.size() != 4) {
		std::cout << "flushProcess: ssd.exe [w/r] [args...]\n";
		return INVALID_COMMAND;
	}

	std::string cmd = args[1];
	std::vector<std::string> cmdArgs{ args[2], args[3] };
	return dispatchCommand(cmd, cmdArgs);
}

int CommandProcessor::dispatchCommand(const std::string& cmd, const std::vector<std::string>& args) {
	if (isWriteCommand(cmd)) {
		return handleWrite(args);
	}
	if (isReadCommand(cmd)) {
		return handleRead(args);
	}
	if (isEraseCommand(cmd)) {
		return handleErase(args);
	}
}

int CommandProcessor::handleWrite(const std::vector<std::string>& args) {
	if (!isWriteValidArgument(args)) {
		return INVALID_ARGUMENT;
	}
	this->ssdOperator = WRITE_OPERATION;
	this->address = std::stoi(args[0]);
	this->memoryValue = args[1];

	return SUCCESS;
}

int CommandProcessor::handleRead(const std::vector<std::string>& args) {
	if (!isReadValidArgument(args)) {
		return INVALID_ARGUMENT;
	}

	this->ssdOperator = READ_OPERATION;
	this->address = std::stoi(args[0]);
	return SUCCESS;
}

int CommandProcessor::handleErase(const std::vector<std::string>& args) {
	if (!isEraseValidArgument(args)) {
		return INVALID_ARGUMENT;
	}

	this->ssdOperator = ERASE_OPERATION;
	this->address = std::stoi(args[0]);
	this->size = std::stoi(args[1]);

	if (address + size >= 100) return INVALID_ARGUMENT;

	return SUCCESS;
}

bool CommandProcessor::isWriteCommand(const std::string& cmd) {
	return cmd == "w" || cmd == "W";
}

bool CommandProcessor::isReadCommand(const std::string& cmd) {
	return cmd == "r" || cmd == "R";
}

bool CommandProcessor::isEraseCommand(const std::string& cmd) {
	return cmd == "e" || cmd == "E";
}

bool CommandProcessor::isWriteValidArgument(const std::vector<std::string>& args) {
	if (args.size() != 2) return false;
	return patternChecker.isValidAddress(args[0]) && patternChecker.isValidMemoryValue(args[1]);
}

bool CommandProcessor::isReadValidArgument(const std::vector<std::string>& args) {
	if (args.size() != 1) return false;
	return patternChecker.isValidAddress(args[0]);
}

bool CommandProcessor::isEraseValidArgument(const std::vector<std::string>& args) {
	if (args.size() != 2) return false;
	return patternChecker.isValidAddress(args[0]) && patternChecker.isValidSize(args[1]);;
}

void CommandProcessor::printWriteToOutput(std::string value) {
	SsdWrite writer;
	writer.writeOutputFile(value);
}

int CommandProcessor::getOperator() {
	return this->ssdOperator;
}

int CommandProcessor::getAddress() {
	return this->address;
}

std::string CommandProcessor::getInputValue() {

	std::stringstream ss;
	int decimalValue = std::stoul(this->memoryValue, nullptr, 16);

	ss << "0x" << std::setw(8) << std::setfill('0')
		<< std::hex << std::uppercase
		<< decimalValue;

	return ss.str();
}

int CommandProcessor::getSize() {
	return this->size;
}

