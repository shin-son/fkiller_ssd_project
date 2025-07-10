#pragma once
#include "ICommand.h"

class EraseWriteAgingCommand : public ICommand
{
public:
	EraseWriteAgingCommand() { CLASS_NAME = "EraseAndWriteAging"; }
	void printHelp() override;

private:
	bool prepare(std::istringstream& iss) override;
	bool execute() override;
	void wrapUp(bool noError) override;

	bool eraseWriteAgingOneCycle(int firstStartLBA, int lastStartLBA, int increment);
	string getRandomInput();

	static const int firstEraseLBA = 0;
	static const int firstEraseSize = 3;

	static const int firstStartLBAForAging = 2;
	static const int lastStartLBAForAging = 98;
	static const int incrementNextLBA = 2;
};