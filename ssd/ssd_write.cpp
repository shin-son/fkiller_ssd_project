#include "ssd_write.h"
#include <string>
#include "ssd_constants.h"

using std::string;

void SsdWrite::loadSsdNandFile() {
	if (!ssdFileIoRead.openReadFileStream()) return;
	ssdNandData = ssdFileIoRead.loadSsdNandData();
	ssdFileIoRead.closeReadFileStream();
}

void SsdWrite::writeSsdNandData(int index, const std::string& value) {

	writeOutputFile(EMPTY_STRING);
	ssdNandData[index] = value;
}

void SsdWrite::saveSsdNandFile() {
	if (!ssdFileIoRead.openWriteFileStream()) return;
	ssdFileIoRead.saveSsdNandData(ssdNandData);
	ssdFileIoRead.closeWriteFileStream();
}

void SsdWrite::writeOutputFile(const string& targetString) {

	if (!ssdFileIoWrite.openWriteFileStream()) return;
	std::vector<string> targetVector{ targetString };
	ssdFileIoWrite.saveSsdNandData(targetVector);
	ssdFileIoWrite.closeWriteFileStream();
}
