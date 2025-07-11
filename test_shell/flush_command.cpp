#include "flush_command.h"

void FlushCommand::printHelp()
{
	std::cout << " FLUSH - flush all buffers \n" <<
		"\t usage - flush (ex. flush)" << std::endl;
}

bool FlushCommand::prepare(std::istringstream& iss)
{
	return true;
}

bool FlushCommand::execute()
{
	string result = cmdRequester->flush();
	return true;
}

void FlushCommand::wrapUp(bool noError)
{
	printLog(logMessage);
}