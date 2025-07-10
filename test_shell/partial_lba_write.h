#pragma once
#include "ICommand.h"

class PartialLbaWriteCommand : public ICommand
{
public:
	NEXT_TEST process(const string& command, std::istringstream& iss) override
	{
		return NEXT_KEEP_GOING;
	}

	void printHelp() override
	{
		std::cout << " Test script - 2 PartialLBAWrite\n" <<
			"\trepeats following steps 30 times \n" <<
			"\t  step1) write the data to lba 4\n" <<
			"\t  step2) write the data to lba 0\n" <<
			"\t  step3) write the data to lba 3\n" <<
			"\t  step4) write the data to lba 1\n" <<
			"\t  step5) write the data to lba 2\n" <<
			"\t  step6) Check if data of all LBA 0 to 4 are the same\n" <<
			"\t usage - 2_PartialLBAWrite(or 2_)" << std::endl;
	}
};