#pragma once
#include "ICommand.h"

class EraseWriteAgingCommand : public ICommand
{
public:
	EraseWriteAgingCommand() { CLASS_NAME = "EraseAndWriteAging"; }
	NEXT_TEST process(std::istringstream& iss) override;
	void printHelp() override;

private:
	bool eraseWriteAgingOneCycle(int firstStartLBA, int lastStartLBA, int increment);
	string getRandomInput();
};