#pragma once
#include "ICommand.h"

class FullWriteReadCompareCommand : public ICommand
{
public:
	FullWriteReadCompareCommand() { CLASS_NAME = "FullWriteReadCompare"; }
	NEXT_TEST process(const string& command, std::istringstream& iss) override;
	void printHelp() override;
private:
	string intToHexString(int value);
};