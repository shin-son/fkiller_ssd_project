#pragma once
#include "ICommand.h"

using std::string;

class WriteCommand : public ICommand
{
public:
	WriteCommand() { CLASS_NAME = "Write"; }
	NEXT_TEST process(std::istringstream& iss);
	void printHelp() override;

private:
	bool prepare(std::istringstream& iss) override;
	bool execute() override;
	void wrapUp(bool noError) override;

	int lba = 0;
	string data = "";
};