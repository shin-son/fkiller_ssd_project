#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "ssd_file_io.h"
#include "ssd_load_file.h"

class SsdWrite : public SsdLoadFile
{
public:
	SsdWrite() : SsdLoadFile("ssd_nand.txt") {
		ssdFileIoWrite.setFileName("ssd_output.txt");
	}

	void writeSsdNandData(int index, const std::string& value);
	void writeOutputFile(const std::string& targetString);

private:

	SsdFileIo ssdFileIoWrite;
};