#include "ssd_Read.h"
#include <fstream>
#include <string>
#include "ssd_constants.h"

using std::string;

bool SsdRead::readSsdNandFile() {

	if (!openReadFileStream(ssdReadFileName)) return false;
	loadSsdNandData();
	closeReadFileStream();
	return true;
}

string SsdRead::getSsdNandDataAt(int index) {
	preConditionCheck(index);
	string result = ssdNandData[index];
	return result;
}

bool SsdRead::writeSsdNandDataToFile(const string &targetString) {

	if (!openWriteFileStream()) return false;
	saveSsdResultData(targetString);
	closeWriteFileStream();
	return true;
}

int SsdRead::getSsdNandDataSize() {
	return ssdNandData.size();
}

bool SsdRead::isSsdOutputFileCorrect(const string& targetString) {

	if (!openReadFileStream(ssdWriteFileName)) return false;

	string writeString;
	std::getline(readFileStream, writeString);

	closeReadFileStream();

	if (writeString != targetString) return false;

	return true;
}

void SsdRead::loadSsdNandData()
{
	string line;
	while (std::getline(readFileStream, line)) {
		ssdNandData.push_back(line);
	}
}

void SsdRead::saveSsdResultData(const string& targetString)
{
	writeFileStream << targetString << std::endl;
}

void SsdRead::preConditionCheck(int index)
{
	if (index < 0 || index >= ssdNandData.size()) {
		writeSsdNandDataToFile(ERROR_STRING);
		throw std::out_of_range("Index out of range");
	}
}

bool SsdRead::openReadFileStream(string fileName)
{
	readFileStream.open(fileName);
	if (!readFileStream.is_open()) {
		return false;
	}
	return true;
}

void SsdRead::closeReadFileStream() {
	if (readFileStream.is_open()) {
		readFileStream.close();
	}
}

bool SsdRead::openWriteFileStream()
{
	writeFileStream.open(ssdWriteFileName);
	if (!writeFileStream.is_open()) {
		return false;
	}
	return true;
}

void SsdRead::closeWriteFileStream() {
	if (writeFileStream.is_open()) {
		writeFileStream.close();
	}
}





