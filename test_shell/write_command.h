#pragma once
#include "ICommand.h"

class WriteCommand : public ICommand
{
public:
	TEST_NEXT process(const string& command) override
	{
		return NEXT_KEEP_GOING;
	}

	void printHelp() override
	{
		std::cout << " WRITE - write value to LBA(Logical Block Addressing) \n" <<
			"\t usage - write <LBA> <value> (ex.write 3 0xAAAABBBB)" << std::endl;
	}
};