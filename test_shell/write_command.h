#pragma once
#include "ICommand.h"

class WriteCommand : public ICommand
{
public:
	WriteCommand() { CLASS_NAME = "Write"; }
	NEXT_TEST process(const string& command, std::istringstream& iss) override;
	void printHelp() override;
};