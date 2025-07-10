#pragma once
#include "ICommand.h"

class ReadCommand : public ICommand
{
public:
	ReadCommand() { CLASS_NAME = "Read"; }
	NEXT_TEST process(std::istringstream& iss) override;
	void printHelp() override;
};