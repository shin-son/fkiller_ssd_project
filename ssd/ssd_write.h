#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "ssd_file_io.h"

class SsdWrite
{
public:
	SsdWrite() {
		ssdFileIoRead.setFileName("ssd_nand.txt");
		ssdFileIoWrite.setFileName("ssd_output.txt");
	}

	void loadSsdNandFile();
	void writeSsdNandData(int index, const std::string& value);
	void saveSsdNandFile();
	void writeOutputFile(const std::string& targetString);

private:

	std::vector<std::string> ssdNandData;

	SsdFileIo ssdFileIoRead;
	SsdFileIo ssdFileIoWrite;
};