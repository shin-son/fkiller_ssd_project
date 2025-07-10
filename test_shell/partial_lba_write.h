#pragma once
#include "ICommand.h"

using std::vector;

class PartialLbaWriteCommand : public ICommand
{
public:
	NEXT_TEST process(const string& command, std::istringstream& iss) override;
	void printHelp() override;
private:
	bool writeTheSequence(const std::string& data, const vector<int>& lbaSequence);
	bool verifyTheSequence(const std::string& data, const vector<int>& lbaSequence);
};