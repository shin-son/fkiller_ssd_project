#pragma once
#include "ssd_read.h"
#include "ssd_write.h"
#include <string>

class SsdFacade
{
public:
	static SsdFacade& getInstance();

	void readSsdIndex(int index);
	void writeSsdIndex(int index, const std::string& targetString);

private:
	SsdFacade() = default;

	SsdRead ssdRead;
	SsdWrite ssdWrite;
};

