#pragma once
#include "ICommand.h"

class WriteReadAgingCommand : public ICommand
{
public:
	TEST_NEXT process(const string& command) override
	{
		return NEXT_KEEP_GOING;
	}

	void printHelp() override
	{
		std::cout << " Test script - 3 WriteReadAging\n"
			"\trepeats following steps 200 times \n" <<
			"\t  step1) write the data to lba 0\n" <<
			"\t  step2) write the data to lba 99\n" <<
			"\t  step3) read the data to lba 0\n" <<
			"\t  step4) read the data to lba 99\n" <<
			"\t  step6) Check if data of LBA 0 and 99 is the same\n" <<
			"\t usage - 3_WriteReadAging(or 3_)" << std::endl;
	}
};