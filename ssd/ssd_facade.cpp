#include "ssd_facade.h"

using std::string;

SsdFacade& SsdFacade::getInstance() {
	static SsdFacade instance;
	return instance;
}

void SsdFacade::readSsdIndex(CommandProcessor cmd) {
	ssdRead.readSsdNandFile();
	string ret = ssdRead.getSsdNandDataAt(cmd.getAddress());
	ssdRead.writeSsdNandDataToFile(ret);
}

void SsdFacade::writeSsdIndex(CommandProcessor cmd)
{
	ssdWrite.loadSsdNandFile();
	ssdWrite.writeSsdNandData(cmd.getAddress(), cmd.getInputValue());
	ssdWrite.saveSsdNandFile();
}

void SsdFacade::eraseSsdIndexToSize(CommandProcessor cmd) {
	ssdErase.loadSsdNandFile();
	ssdErase.eraseSsdNandData(cmd.getAddress(), cmd.getSize());
	ssdErase.saveSsdNandFile();
}


