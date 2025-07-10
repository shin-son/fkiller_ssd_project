#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "constants.h"
#include "ssd_interface.h"
#include "logger.h"

using std::string;

class ICommand
{
public:
	virtual NEXT_TEST process(const string& command, std::istringstream& iss) = 0;
	virtual void printHelp() = 0;
	static void setSsdAdapter(SSDInterface* adapter_)
	{
		adapter = adapter_;
	}

protected:
	//virtual bool prepare() = 0;
	//virtual bool execute() = 0;
	//virtual bool wrapUp() = 0;

	void printLog(const string& message)
	{
		LOG_PRINT(message);
		std::cout << message << std::endl;
	}

	string getCommandName()
	{
		return ("[" + CLASS_NAME + "] ");
	}

	string getDoneMessage()
	{
		return (getCommandName() + DONE);
	}

	string getErrorHeader()
	{
		return (getCommandName() + ERROR + " ");
	}

	string CLASS_NAME;
	static SSDInterface* adapter;
	Logger& logger = Logger::getInstance();
};