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
	NEXT_TEST process(std::istringstream& iss);
	virtual void printHelp() = 0;
	static void setSsdAdapter(SSDInterface* _adapter);

protected:
	virtual bool prepare(std::istringstream& iss) = 0;
	virtual bool execute() = 0;
	virtual void wrapUp(bool noError) = 0;

	void printLog(const string& message);
	string getCommandName();
	string getDoneMessage();
	string getErrorHeader();

	string CLASS_NAME;
	static SSDInterface* cmdRequester;
	Logger& logger = Logger::getInstance();
	string logMessage = "";
};