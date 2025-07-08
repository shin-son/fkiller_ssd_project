#pragma once
#include <cstdint>
#include <string>

class SsdWrite
{
public:
	void write(int address, uint32_t value);
private:
	void writeOutputFile(const std::string& result);
	void writeTheValueToMemory(int address, uint32_t value);
};

