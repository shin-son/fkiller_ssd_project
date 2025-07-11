#include "ssd_read.h"
#include <fstream>
#include <string>
#include "ssd_constants.h"

using std::string;

string SsdRead::getSsdNandDataAt(int index) {
	string result = getSsdNandData()[index];
	return result;
}

bool SsdRead::writeSsdNandDataToFile(const string &targetString) {

	if (!ssdFileIoWrite.openWriteFileStream()) return false;
	std::vector<string> targetVector{ targetString };
	ssdFileIoWrite.saveSsdNandData(targetVector);
	ssdFileIoWrite.closeWriteFileStream();
	return true;
}

int SsdRead::getSsdNandDataSize() {
	return getSsdNandData().size();
}

bool SsdRead::isSsdOutputFileCorrect(const string& targetString) {

	if (!ssdFileIoWrite.openReadFileStream()) return false;

	std::vector<string> targetVector;
	targetVector = ssdFileIoWrite.loadSsdNandData();

	ssdFileIoWrite.closeReadFileStream();

	if (targetVector[0] != targetString) return false;

	return true;
}







