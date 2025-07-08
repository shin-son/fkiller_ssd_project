#pragma once
#include <cstdint>
#include <string>

class SsdWrite
{
public:
	void write(const std::string& address, const std::string& value);
private:
	void writeOutputFile(const std::string& result);
	void writeTheValueToMemory(int address, const std::string& value);
	bool isValidMemoryValue(const std::string& value);
	bool isValidAddress(int& addressToInt, const std::string& address);
};