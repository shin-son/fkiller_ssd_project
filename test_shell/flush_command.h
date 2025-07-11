#pragma once
#include "ICommand.h"

class FlushCommand : public ICommand
{
public:
	FlushCommand() { CLASS_NAME = "flush"; }
	NEXT_TEST process(std::istringstream& iss);
	void printHelp() override;

private:
	bool prepare(std::istringstream& iss) override;
	bool execute() override;
	void wrapUp(bool noError) override;

	int lba = 0;
};