#pragma once
#include <iostream>
#include <string>
#include "constants.h"

using std::string;

class ICommand
{
public:
	virtual TEST_NEXT process(const string& command) = 0;
	virtual void printHelp() = 0;

private:
	//virtual bool prepare() = 0;
	//virtual bool execute() = 0;
	//virtual bool wrapUp() = 0;
};