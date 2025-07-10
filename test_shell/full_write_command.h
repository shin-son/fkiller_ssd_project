#pragma once
#include "ICommand.h"

class FullWriteCommand : public ICommand
{
public:
	NEXT_TEST process(const string& command, std::istringstream& iss) override;
	void printHelp() override;
};