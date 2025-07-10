#pragma once
#include "ICommand.h"

class ReadCommand : public ICommand
{
public:
	NEXT_TEST process(const string& command) override;
	void printHelp() override;
};