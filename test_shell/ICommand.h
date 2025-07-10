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
	virtual NEXT_TEST process(std::istringstream& iss) = 0;
	virtual void printHelp() = 0;
	static void setSsdAdapter(SSDInterface* _adapter);

protected:
	void printLog(const string& message);
	string getCommandName();
	string getDoneMessage();
	string getErrorHeader();

	string CLASS_NAME;
	static SSDInterface* cmdRequester;
	Logger& logger = Logger::getInstance();
};