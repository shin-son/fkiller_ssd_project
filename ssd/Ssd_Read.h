#pragma once
#include <vector>
#include <string>

class Ssd_Read
{
public:
	Ssd_Read(std::string readFileName = "ssd_nand.txt", std::string writeFileName = "ssd_output.txt")
		: SSD_READ_FILE(readFileName), SSD_WRITE_FILE(writeFileName) {}
	
	bool readSsdNandFile();
	bool writeSsdNandDataToFile(std::string targetString);

	int getSsdNandDataSize();

	std::string getSsdNandDataAt(int index);

	bool isSsdOutputFileCorrect(std::string targetString);

private:
	std::string SSD_READ_FILE;
	std::string SSD_WRITE_FILE;
	std::vector<std::string> ssdNandData;
};

