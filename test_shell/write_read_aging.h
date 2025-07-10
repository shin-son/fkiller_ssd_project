#pragma once
#include "ICommand.h"

class WriteReadAgingCommand : public ICommand
{
public:
	WriteReadAgingCommand() { CLASS_NAME = "WriteReadAging"; }
	NEXT_TEST process(std::istringstream& iss);
	void printHelp() override;

private:
	bool prepare(std::istringstream& iss) override;
	bool execute() override;
	void wrapUp(bool noError) override;

	string getRandomData();
};