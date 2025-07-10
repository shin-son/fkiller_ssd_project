#pragma once
#include "ICommand.h"

class EraseCommand : public ICommand
{
public:
	EraseCommand() { CLASS_NAME = "Erase"; }
	void printHelp() override;

private:
	bool prepare(std::istringstream& iss) override;
	bool execute() override;
	void wrapUp(bool noError) override;

	bool getEraseParameter(std::istringstream& iss);
	bool isVaiidEraseRange();
	bool eraseRange();

	void setEndLBA();
	int getLastEraseLbaCount(int changedStartLBA);

	int startLBA = 0;
	int size = 0;
	int endLBA = 0;
};