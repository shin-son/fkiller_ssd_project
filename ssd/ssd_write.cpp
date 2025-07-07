#include "ssd_write.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstdint>


void writeTheValueToMemory(int address, uint32_t value) {
	std::string memoryFile = "ssd_nand.txt";
	std::ifstream memoryfileread(memoryFile, std::ios::in | std::ios::out);
	if (!memoryfileread.is_open()) {
		std::cerr << "파일 열기 실패!\n";
		return;
	}
	std::vector<uint32_t> memory(100);  // 0~99 주소

	std::string line;
	int addr = 0;
	while (std::getline(memoryfileread, line) && addr < 100) {
		memory[addr] = std::stoul(line, nullptr, 16);  // 16진수 파싱
		++addr;
	}
	memoryfileread.close();

	std::ofstream memoryfilewrite(memoryFile, std::ios::in | std::ios::out);
	if (!memoryfilewrite.is_open()) {
		std::cerr << "파일 열기 실패!\n";
		return;
	}

	memory[address] = value;
	for (const auto& val : memory) {
		memoryfilewrite << "0x"
			<< std::setw(8) << std::setfill('0')
			<< std::hex << std::uppercase
			<< val << "\n";
	}

	memoryfilewrite.close();
}

void SsdWrite::write(int address, uint32_t value) {
	std::string outputFile = "ssd_output.txt";
	std::stringstream memoryStream;
	std::string outputResult;

	std::ofstream outputfile(outputFile, std::ios::out | std::ios::trunc);
	if (address < 0 || address > 99) {
		std::cout << "INVALID ADDRESS\n";
		outputResult = "ERROR";
	}
	else {
		outputResult = "\"\"";
		writeTheValueToMemory(address, value);
	}
	outputfile << outputResult;

	outputfile.close();
}
