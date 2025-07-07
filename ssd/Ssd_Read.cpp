#include "Ssd_Read.h"
#include <fstream>
#include <string>

using std::string;

bool Ssd_Read::readSsdNandFile() {

	std::ifstream file(SSD_READ_FILE);
	if (!file.is_open()) {
		return false;
	}

	string line;
	while (std::getline(file, line)) {
		ssdNandData.push_back(line);
	}
	file.close();
	return true;
}

bool Ssd_Read::writeSsdNandDataToFile(string targetString) {
	std::ofstream file(SSD_WRITE_FILE);
	if (!file.is_open()) {
		return false;
	}

	file << targetString << std::endl;

	file.close();
	return true;
}

int Ssd_Read::getSsdNandDataSize() { 
	return ssdNandData.size(); 
}

string Ssd_Read::getSsdNandDataAt(int index) {
	if (index < 0 || index >= ssdNandData.size()) {
		writeSsdNandDataToFile("ERROR");
		throw std::out_of_range("Index out of range");
	}
	string result = ssdNandData[index];
	writeSsdNandDataToFile(result);
	return result;
}

bool Ssd_Read::isSsdOutputFileCorrect(string targetString) {

	std::ifstream file(SSD_WRITE_FILE);
	if (!file.is_open()) {
		return false;
	}

	string line;
	std::getline(file, line);

	file.close();

	if (line != targetString) return false;

	return true;
}





