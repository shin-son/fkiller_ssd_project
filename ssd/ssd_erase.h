#pragma once
#include "ssd_file_io.h"
#include <string>

class SsdErase
{
public:
	SsdErase(std::string fileName = "ssd_nand.txt") {
		ssdFileIo.setFileName(fileName);
	}

	void loadSsdNandFile();
	void eraseSsdNandData(int startIndex, int size);
	void saveSsdNandFile();

private:
	SsdFileIo ssdFileIo;

	std::vector<std::string> ssdNandData;
};

