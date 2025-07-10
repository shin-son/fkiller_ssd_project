#pragma once
#include "ICommand.h"

class EraseWriteAgingCommand : public ICommand
{
public:
	NEXT_TEST process(const string& command, std::istringstream& iss) override
	{
		return NEXT_KEEP_GOING;
	}

	void printHelp() override
	{
		std::cout << " Test script - 4 EraseAndWriteAging\n"
			"\t  erase lba 0~2\n" <<
			"\trepeats following steps 30 times) \n" <<
			"\t  step1) write lba 2 two times with different random values\n" <<
			"\t  step2) erase lba 2~4with another random value\n" <<
			"\t  step3) write lba 4 two times with different random values\n" <<
			"\t  step4) erase lba 4~6with another random value\n" <<
			"\t ...\n" <<
			"\t  step49) write lba 98 two times with different random values\n" <<
			"\t  step50) erase lba 98~99with another random value" << std::endl;
	}
};