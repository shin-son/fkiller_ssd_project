#pragma once
#include "ICommand.h"

class FullWriteReadCompareCommand : public ICommand
{
public:
	FullWriteReadCompareCommand() { CLASS_NAME = "FullWriteReadCompare"; }
	void printHelp() override;

private:
	bool prepare(std::istringstream& iss) override;
	bool execute() override;
	void wrapUp(bool noError) override;

	bool writeTest(int sequenceNumber, const string& testString);
	bool readTest(int sequenceNumber, const string& testString);

	string intToHexString(int value);
};