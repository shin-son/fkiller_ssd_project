#pragma once
#include "ICommand.h"

class EraseCommand : public ICommand
{
public:
	EraseCommand() { CLASS_NAME = "Erase"; }
	NEXT_TEST process(std::istringstream& iss) override;
	void printHelp() override;

private:
	bool getEraseParameter(int& startLBA, int& size, std::istringstream& iss);
	bool isVaiidEraseRange(const int startLBA, const int endLBA);
	bool eraseRange(int startLBA, int endLBA);
};