#pragma once
#include "ICommand.h"

class WriteCommand : public ICommand
{
public:
	WriteCommand() { CLASS_NAME = "Write"; }
	NEXT_TEST process(std::istringstream& iss) override;
	void printHelp() override;
};