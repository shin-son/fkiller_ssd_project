#pragma once
#include "ICommand.h"

class ReadCommand : public ICommand
{
public:
	ReadCommand() { CLASS_NAME = "Read"; }
	NEXT_TEST process(std::istringstream& iss);
	void printHelp() override;

private:
	bool prepare(std::istringstream& iss) override;
	bool execute() override;
	void wrapUp(bool noError) override;

	int lba = 0;
};