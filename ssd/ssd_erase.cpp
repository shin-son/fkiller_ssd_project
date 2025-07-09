#include "ssd_erase.h"

void SsdErase::loadSsdNandFile() {
	if (!ssdFileIo.openReadFileStream()) return;
	ssdNandData = ssdFileIo.loadSsdNandData();
	ssdFileIo.closeReadFileStream();
}

void SsdErase::eraseSsdNandData(int startIndex, int size) {

	for (int index = startIndex; index < startIndex + size; ++index) {
		ssdNandData[index] = "0x00000000";
	}
}

void SsdErase::saveSsdNandFile() {
	if (!ssdFileIo.openWriteFileStream()) return;
	ssdFileIo.saveSsdNandData(ssdNandData);
	ssdFileIo.closeWriteFileStream();
}