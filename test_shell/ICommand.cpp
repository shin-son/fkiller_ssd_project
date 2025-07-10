#include "ICommand.h"

SSDInterface* ICommand::cmdRequester = nullptr;

NEXT_TEST ICommand::process(std::istringstream& iss)
{
	bool noError = prepare(iss);
	if (noError) noError = execute();
	wrapUp(noError);

	return NEXT_KEEP_GOING;
}

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