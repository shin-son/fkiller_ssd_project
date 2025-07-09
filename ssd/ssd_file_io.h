#pragma once
#include <vector>
#include <string>
#include <fstream>

class SsdFileIo
{
public:
	SsdFileIo(std::string fileNameParam = "ssd_nand.txt")
		: fileName(fileNameParam) {
	}

	std::vector<std::string> loadSsdNandData();
	void saveSsdNandData(std::vector<std::string> targetData);

	bool openReadFileStream();
	void closeReadFileStream();
	bool openWriteFileStream();
	void closeWriteFileStream();

	void setFileName(const std::string & fileNameParam) {
		this->fileName = fileNameParam;
	}

private:
	std::vector<std::string> ssdNandData;

	std::string fileName;

	std::ifstream readFileStream;
	std::ofstream writeFileStream;
};

