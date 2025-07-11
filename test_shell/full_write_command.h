#pragma once
#include "ICommand.h"

class FullWriteCommand : public ICommand
{
public:
	FullWriteCommand() { CLASS_NAME = "fullWrite"; }
	NEXT_TEST process(std::istringstream& iss);
	void printHelp() override;

private:
	bool prepare(std::istringstream& iss) override;
	bool execute() override;
	void wrapUp(bool noError) override;

	string data = "";
};