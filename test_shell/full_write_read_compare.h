#pragma once
#include "ICommand.h"

class FullWriteReadCompareCommand : public ICommand
{
public:
	NEXT_TEST process(const string& command, std::istringstream& iss) override;
	void printHelp() override;
private:
	string intToHexString(int value);
};