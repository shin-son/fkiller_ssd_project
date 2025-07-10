#pragma once
#include "ICommand.h"

class EraseRangeCommand : public ICommand
{
public:
	EraseRangeCommand() { CLASS_NAME = "EraseRange"; }
	void printHelp() override;

private:
	bool prepare(std::istringstream& iss) override;
	bool execute() override;
	void wrapUp(bool noError) override;

	bool getEraseParameter(std::istringstream& iss);
	bool isVaiidEraseRange();
	bool eraseRange();
	int getLastEraseLbaCount(int changedStartLBA);

	int startLBA = 0;;
	int endLBA = 0;
};