#pragma once
#include "ICommand.h"

class FullReadCommand : public ICommand
{
public:
	FullReadCommand() { CLASS_NAME = "fullRead"; }
	NEXT_TEST process(const string& command, std::istringstream& iss) override;
	void printHelp() override;
};