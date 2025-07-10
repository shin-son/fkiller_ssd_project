#pragma once
#include "ICommand.h"

class EraseRangeCommand : public ICommand
{
public:
	NEXT_TEST process(const string& command) override
	{
		return NEXT_KEEP_GOING;
	}

	void printHelp() override
	{
		std::cout << " ERASE RANGE - erase at LBA(Logical Block Addressing) Range \n" <<
			"\t usage - erase_range <start LBA> <end LBA> (ex.erase_range 0 10)" << std::endl;
	}
};