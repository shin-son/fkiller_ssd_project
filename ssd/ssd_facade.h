#pragma once
#include "ssd_read.h"
#include "ssd_write.h"
#include "ssd_erase.h"
#include <string>
#include "command_process.h"

class SsdFacade
{
public:
	static SsdFacade& getInstance();

	void readSsdIndex(CommandProcessor cmd);
	void writeSsdIndex(CommandProcessor cmd);
	void eraseSsdIndexToSize(CommandProcessor cmd);

private:
	SsdFacade() = default;

	SsdRead ssdRead;
	SsdWrite ssdWrite;
	SsdErase ssdErase;
};

