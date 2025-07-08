#include "ssd_write.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <regex>

bool SsdWrite::readTheNandMemory(const std::string& memoryFile) {
	std::ifstream memoryfileread(memoryFile);
	if (!memoryfileread) return false;

	std::string line;
	while (std::getline(memoryfileread, line)) {
		memory.push_back(std::stoul(line, nullptr, 16));
	}
	memoryfileread.close();
	return true;
}

void SsdWrite::writeTheValueToMemory(int address, const std::string& value) {
	std::string memoryFile = "ssd_nand.txt";

	if (!readTheNandMemory(memoryFile)) {
		throw std::ios_base::failure("");
	}

	std::ofstream memoryfilewrite(memoryFile, std::ios::out | std::ios::trunc);
	if (!memoryfilewrite) return;

	memory[address] = std::stoul(value, nullptr, 16);
	for (const auto& val : memory) {
		memoryfilewrite << "0x"
			<< std::setw(8) << std::setfill('0')
			<< std::hex << std::uppercase
			<< val << "\n";
	}
}

void SsdWrite::writeOutputFile(const std::string& result) {
	std::string outputFile = "ssd_output.txt";
	std::ofstream outputfile(outputFile, std::ios::out | std::ios::trunc);
	if (outputfile) {
		outputfile << result;
	}
}

void SsdWrite::write(const std::string& address, const std::string& value) {
	std::stringstream memoryStream;
	std::string outputResult = "";
	int addressToInt;

	if (!isValidAddress(addressToInt, address) || !isValidMemoryValue(value)) {
		outputResult = "ERROR";
		writeOutputFile(outputResult);
		throw std::invalid_argument("INVALID Address");
	}

	writeOutputFile(outputResult);
	writeTheValueToMemory(addressToInt, value);
}

bool SsdWrite::isValidAddress(int& addressToInt, const std::string& address) {
	static const std::regex numericPattern("^[0-9]+$");
	if (std::regex_match(address, numericPattern) == false) return false;

	addressToInt = std::stoi(address);

	return (addressToInt >= 0 && addressToInt < 100);
}

bool SsdWrite::isValidMemoryValue(const std::string& value) {
	static const std::regex hexPattern("^0x[0-9A-Fa-f]{1,8}$");
	return std::regex_match(value, hexPattern);
}