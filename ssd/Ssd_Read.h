#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "ssd_file_io.h"

class SsdRead
{
public:
	SsdRead(std::string readFileName = "ssd_nand.txt", std::string writeFileName = "ssd_output.txt")
		: ssdReadFileName(readFileName), ssdWriteFileName(writeFileName) {
		ssdFileIoRead.setFileName(readFileName);
		ssdFileIoWrite.setFileName(writeFileName);
	}

	bool readSsdNandFile();
	std::string getSsdNandDataAt(int index);

	bool writeSsdNandDataToFile(const std::string &targetString);
	int getSsdNandDataSize();
	bool isSsdOutputFileCorrect(const std::string &targetString);

private:

	std::string ssdReadFileName;
	std::string ssdWriteFileName;

	std::vector<std::string> ssdNandData;

	SsdFileIo ssdFileIoRead;
	SsdFileIo ssdFileIoWrite;
};

