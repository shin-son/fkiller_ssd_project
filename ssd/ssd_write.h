#pragma once
#include <cstdint>
#include <string>
#include <vector>

class SsdWrite
{
public:
	void write(const std::string& address, const std::string& value);
	void writeOutputFile(const std::string& result);
private:
	void writeTheValueToMemory(int address, const std::string& value);
	bool readTheNandMemory(const std::string& memoryFileName);
	std::vector<uint32_t> memory;
};