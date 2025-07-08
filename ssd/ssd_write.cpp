#include "ssd_write.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include "error_code.h"


int writeTheValueToMemory(int address, uint32_t value) {
	std::string memoryFile = "ssd_nand.txt";

	std::ifstream memoryfileread(memoryFile, std::ios::in | std::ios::out);
	if (!memoryfileread.is_open()) {
		return FAILOPENFAIL;
	}

	std::vector<uint32_t> memory(100);
	std::string line;
	int addr = 0;
	while (std::getline(memoryfileread, line) && addr < 100) {
		memory[addr] = std::stoul(line, nullptr, 16);
		++addr;
	}
	memoryfileread.close();

	std::ofstream memoryfilewrite(memoryFile, std::ios::in | std::ios::out);
	if (!memoryfilewrite.is_open()) {
		return FAILOPENFAIL;
	}

	memory[address] = value;
	for (const auto& val : memory) {
		memoryfilewrite << "0x"
			<< std::setw(8) << std::setfill('0')
			<< std::hex << std::uppercase
			<< val << "\n";
	}

	memoryfilewrite.close();

	return SUCCESS;
}

int SsdWrite::write(int address, uint32_t value) {
	std::string outputFile = "ssd_output.txt";
	std::stringstream memoryStream;
	std::string outputResult = "";
	int result = 0;

	std::ofstream outputfile(outputFile, std::ios::out | std::ios::trunc);
	if (!outputfile.is_open()) {
		return FAILOPENFAIL;
	}
	if (address < 0 || address > 99) {
		outputResult = "ERROR";
		result = INVALIDADDRESS;
	}
	else {
		writeTheValueToMemory(address, value);
	}
	outputfile << outputResult;

	outputfile.close();
	return result;
}
