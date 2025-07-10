#pragma once
#include "ICommand.h"

class EraseWriteAgingCommand : public ICommand
{
public:
	NEXT_TEST process(const string& command, std::istringstream& iss) override;
	void printHelp() override;

private:
	bool eraseWriteAgingOneCycle(int firstStartLBA, int lastStartLBA, int increment);
	string getRandomInput();
};