#pragma once
#include "ICommand.h"

class EraseCommand : public ICommand
{
public:
	NEXT_TEST process(const string& command) override
	{
		return NEXT_KEEP_GOING;
	}

	void printHelp() override
	{
		std::cout << " ERASE - erase at LBA(Logical Block Addressing) Range \n" <<
			"\t usage - erase <start LBA> <size> (ex.erase 0 10)" << std::endl;
	}
};