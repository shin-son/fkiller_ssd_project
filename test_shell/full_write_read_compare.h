#pragma once
#include "ICommand.h"

class FullWriteReadCompareCommand : public ICommand
{
public:
	TEST_NEXT process(const string& command) override
	{
		return NEXT_KEEP_GOING;
	}

	void printHelp() override
	{
		return;
	}
};