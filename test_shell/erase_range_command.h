#pragma once
#include "ICommand.h"

class EraseRangeCommand : public ICommand
{
public:
	NEXT_TEST process(const string& command, std::istringstream& iss) override;
	void printHelp() override;

private:
	bool getEraseParameter(int& startLBA, int& endLBA, std::istringstream& iss);
	bool isVaiidEraseRange(const int startLBA, const int endLBA);
	bool eraseRange(int startLBA, int endLBA);
};