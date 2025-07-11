#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "ssd_file_io.h"
#include "ssd_load_file.h"

class SsdRead : public SsdLoadFile
{
public:
	SsdRead(std::string readFileName = "ssd_nand.txt", std::string writeFileName = "ssd_output.txt")
		: SsdLoadFile(readFileName), ssdWriteFileName(writeFileName) {
		ssdFileIoWrite.setFileName(writeFileName);
	}

	std::string getSsdNandDataAt(int index);

	bool writeSsdNandDataToFile(const std::string &targetString);
	int getSsdNandDataSize();
	bool isSsdOutputFileCorrect(const std::string &targetString);

private:

	std::string ssdWriteFileName;
	SsdFileIo ssdFileIoWrite;
};

