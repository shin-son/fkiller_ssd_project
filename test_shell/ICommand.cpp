#include "ICommand.h"

SSDInterface* ICommand::cmdRequester = nullptr;

void ICommand::setSsdAdapter(SSDInterface* _adapter)
{
	cmdRequester = _adapter;
}

void ICommand::printLog(const string& message)
{
	LOG_PRINT(message);
	std::cout << message << std::endl;
}

string ICommand::getCommandName()
{
	return ("[" + CLASS_NAME + "] ");
}

string ICommand::getDoneMessage()
{
	return (getCommandName() + DONE);
}

string ICommand::getErrorHeader()
{
	return (getCommandName() + ERROR + " ");
}