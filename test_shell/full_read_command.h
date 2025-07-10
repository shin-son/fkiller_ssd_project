#pragma once
#include "ICommand.h"

class FullReadCommand : public ICommand
{
public:
	FullReadCommand() { CLASS_NAME = "fullRead"; }
	NEXT_TEST process(std::istringstream& iss);
	void printHelp() override;

private:
	bool prepare(std::istringstream& iss) override;
	bool execute() override;
	void wrapUp(bool noError) override;
};