#pragma once
#include "ICommand.h"

using std::vector;

class PartialLbaWriteCommand : public ICommand
{
public:
	PartialLbaWriteCommand() { CLASS_NAME = "PartialLBAWrite"; }
	NEXT_TEST process(std::istringstream& iss);
	void printHelp() override;

private:
	bool prepare(std::istringstream& iss) override;
	bool execute() override;
	void wrapUp(bool noError) override;

	bool writeTheSequence(const std::string& data, const vector<int>& lbaSequence);
	bool verifyTheSequence(const std::string& data, const vector<int>& lbaSequence);
};