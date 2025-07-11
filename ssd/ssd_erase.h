#pragma once
#include "ssd_file_io.h"
#include <string>
#include "ssd_load_file.h"

class SsdErase : public SsdLoadFile
{
public:
	SsdErase()
		: SsdLoadFile("ssd_nand.txt") {
	}

	void eraseSsdNandData(int startIndex, int size);

private:
};

