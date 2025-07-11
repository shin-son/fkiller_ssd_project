#include "ssd_load_file.h"

using std::string;
using std::vector;

bool SsdLoadFile::loadSsdNandFile() {

	if (!ssdFileIoRead.openReadFileStream()) return false;
	ssdNandData = ssdFileIoRead.loadSsdNandData();
	ssdFileIoRead.closeReadFileStream();
	return true;
}

void SsdLoadFile::saveSsdNandFile() {
	if (!ssdFileIoRead.openWriteFileStream()) return;
	ssdFileIoRead.saveSsdNandData(ssdNandData);
	ssdFileIoRead.closeWriteFileStream();
}

vector<string>& SsdLoadFile::getSsdNandData()
{
	return ssdNandData;
}
