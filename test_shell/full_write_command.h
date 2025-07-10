#pragma once
#include "ICommand.h"

class FullWriteCommand : public ICommand
{
public:
	FullWriteCommand() { CLASS_NAME = "fullWrite"; }
	NEXT_TEST process(std::istringstream& iss) override;
	void printHelp() override;
};