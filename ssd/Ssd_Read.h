#pragma once
#include <vector>
#include <string>
#include <fstream>

class SsdRead
{
public:
	SsdRead(std::string readFileName = "ssd_nand.txt", std::string writeFileName = "ssd_output.txt")
		: ssdReadFileName(readFileName), ssdWriteFileName(writeFileName) {
	}

	bool readSsdNandFile();
	bool writeSsdNandDataToFile(std::string targetString);
	std::string getSsdNandDataAt(int index);
	int getSsdNandDataSize();
	bool isSsdOutputFileCorrect(std::string targetString);

private:

	void loadSsdNandData();
	void saveSsdResultData(std::string& targetString);
	void preConditionCheck(int index);

	bool openReadFileStream(std::string fileName);
	void closeReadFileStream();
	bool openWriteFileStream();
	void closeWriteFileStream();

	std::string ssdReadFileName;
	std::string ssdWriteFileName;
	std::vector<std::string> ssdNandData;

	std::ifstream readFileStream;
	std::ofstream writeFileStream;

	const std::string ERROR_STRING = "ERROR";
};

