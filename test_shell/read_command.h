#pragma once
#include "ICommand.h"

class ReadCommand : public ICommand
{
public:
	TEST_NEXT process(const string& command) override
	{
		return NEXT_KEEP_GOING;
	}

	void printHelp() override
	{
		std::cout << " READ - read one LBA (Logical Block Addressing) \n" <<
			"\t usage - read <LBA>(ex.read 0)" << std::endl;
	}
};