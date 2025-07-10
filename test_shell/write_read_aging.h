#pragma once
#include "ICommand.h"

class WriteReadAgingCommand : public ICommand
{
public:
	WriteReadAgingCommand() { CLASS_NAME = "WriteReadAging"; }
	NEXT_TEST process(std::istringstream& iss) override;
	void printHelp() override;
};