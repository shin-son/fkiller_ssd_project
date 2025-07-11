#include "ssd_write.h"
#include <string>
#include "ssd_constants.h"

using std::string;

void SsdWrite::writeSsdNandData(int index, const string& value) {

	writeOutputFile(EMPTY_STRING);
	getSsdNandData()[index] = value;
}

void SsdWrite::writeOutputFile(const string& targetString) {

	if (!ssdFileIoWrite.openWriteFileStream()) return;
	std::vector<string> targetVector{ targetString };
	ssdFileIoWrite.saveSsdNandData(targetVector);
	ssdFileIoWrite.closeWriteFileStream();
}
