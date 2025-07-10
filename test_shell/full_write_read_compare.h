#pragma once
#include "ICommand.h"

class FullWriteReadCompareCommand : public ICommand
{
public:
	NEXT_TEST process(const string& command) override
	{
		return NEXT_KEEP_GOING;
	}

	void printHelp() override
	{
		std::cout << " Test script - 1 FullWriteAndReadCompare \n" <<
			"\t write and read Test all indices in 5-unit\n" <<
			"\t usage - 1_FullWriteAndReadCompare(or 1_)" << std::endl;
	}
};