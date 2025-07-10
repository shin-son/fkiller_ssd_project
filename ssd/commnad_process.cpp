#include "command_process.h"
#include "ssd_write.h"
#include "ssd_Read.h"
#include "ssd_constants.h"
#include <iostream>
#include <sstream>
#include <iomanip>

int CommandProcessor::getOperator() {
	return this->ssdOperator;
}

int CommandProcessor::getAddress() {
	return stoi(this->address);
}

std::string CommandProcessor::getInputValue() {

	std::stringstream ss;
	int decimalValue = std::stoul(this->data, nullptr, 16);

	ss << "0x" << std::setw(8) << std::setfill('0')
		<< std::hex << std::uppercase
		<< decimalValue;

	return ss.str();
}

int CommandProcessor::getSize() {
	return stoi(this->data);
}

int CommandProcessor::getResult() {
	return result;
}

void CommandProcessor::printWriteToOutput(std::string value) {
	SsdWrite writer;
	writer.writeOutputFile(value);
}

