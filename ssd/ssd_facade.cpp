#include "ssd_facade.h"

using std::string;

SsdFacade& SsdFacade::getInstance() {
	static SsdFacade instance;
	return instance;
}

void SsdFacade::readSsdIndex(int index) {
	ssdRead.readSsdNandFile();
	string ret = ssdRead.getSsdNandDataAt(index);
	ssdRead.writeSsdNandDataToFile(ret);
}

void SsdFacade::writeSsdIndex(int index, const std::string& targetString)
{
	ssdWrite.write(index, targetString);
}

void SsdFacade::eraseSsdIndexToSize(int index, int size) {
	ssdErase.loadSsdNandFile();
	ssdErase.eraseSsdNandData(index, size);
	ssdErase.saveSsdNandFile();
}


