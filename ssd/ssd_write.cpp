#include "ssd_write.h"
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <regex>

void SsdWrite::writeTheValueToMemory(int address, const std::string& value) {
	std::string memoryFile = "ssd_nand.txt";

	std::ifstream memoryfileread(memoryFile);
	if (!memoryfileread) return;

	std::vector<uint32_t> memory(100);
	std::string line;
	int addr = 0;
	while (std::getline(memoryfileread, line) && addr < 100) {
		memory[addr] = std::stoul(line, nullptr, 16);
		++addr;
	}
	memoryfileread.close();

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

	if (isValidAddress(addressToInt, address) == false) {
		outputResult = "ERROR";
		writeOutputFile(outputResult);
		throw std::invalid_argument("INVALID Address");
	}

	if (isValidMemoryValue(value) == false) {
		outputResult = "ERROR";
		writeOutputFile(outputResult);
		throw std::out_of_range("INVALID Memory Value");
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