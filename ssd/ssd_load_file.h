#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "ssd_file_io.h"

class SsdLoadFile
{
public:
	SsdLoadFile(std::string readFileName) {
		ssdFileIoRead.setFileName(readFileName);
	}

	bool loadSsdNandFile();
	void saveSsdNandFile();
	std::vector<std::string>& getSsdNandData();

private:
	SsdFileIo ssdFileIoRead;

	std::vector<std::string> ssdNandData;
};

