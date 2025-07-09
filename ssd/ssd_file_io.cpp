#include "ssd_file_io.h"
#include <string>

using std::string;

std::vector<std::string>  SsdFileIo::loadSsdNandData()
{
	ssdNandData.clear();
	string line;
	while (std::getline(readFileStream, line)) {
		ssdNandData.push_back(line);
	}
	return ssdNandData;
}

void SsdFileIo::saveSsdNandData(std::vector<std::string> targetData) {

	for (string targetStr : targetData) {
		writeFileStream << targetStr << std::endl;
	}
}

bool SsdFileIo::openReadFileStream() {
	readFileStream.open(fileName);
	if (!readFileStream.is_open()) {
		return false;
	}
	return true;
}

void SsdFileIo::closeReadFileStream() {
	if (readFileStream.is_open()) {
		readFileStream.close();
	}
}

bool SsdFileIo::openWriteFileStream() {
	writeFileStream.open(fileName);
	if (!writeFileStream.is_open()) {
		return false;
	}
	return true;
}

void SsdFileIo::closeWriteFileStream() {
	if (writeFileStream.is_open()) {
		writeFileStream.close();
	}
}

