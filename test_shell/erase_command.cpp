#include "erase_command.h"

void EraseCommand::printHelp()
{
	std::cout << " ERASE - erase at LBA(Logical Block Addressing) Range \n" <<
		"\t usage - erase <start LBA> <size> (ex.erase 0 10)" << std::endl;
}

bool EraseCommand::prepare(std::istringstream& iss)
{
	if (false == getEraseParameter(iss)) return false;

	setEndLBA();
	if (false == isVaiidEraseRange())
	{
		printLog(getErrorHeader() + ": invalid Range(startLBA, size)");
		return false;
	}

	return true;
}

bool EraseCommand::execute()
{
	if (false == eraseRange())
	{
		printLog(getErrorHeader() + ": Erase Fail");
		return false;
	}
	return true;
}

void EraseCommand::wrapUp(bool noError)
{
	if (noError) logMessage = getDoneMessage();
	printLog(logMessage);
}

bool EraseCommand::getEraseParameter(std::istringstream& iss)
{
	if (!(iss >> startLBA))
	{
		printLog(getErrorHeader() + ": missing startLBA");
		return false;
	}

	if (!(iss >> size))
	{
		printLog(getErrorHeader() + ": missing size");
		return false;
	}

	return true;
}

bool EraseCommand::isVaiidEraseRange()
{
	if (startLBA < 0)  return false;
	else if (endLBA >= SSD_SIZE) return false;
	else if (startLBA >= endLBA) return false;

	return true;
}

bool EraseCommand::eraseRange()
{
	int changedStartLBA = startLBA;
	int retSuccess = true;
	string eraseResult = "";
	int eraseCount = (endLBA - changedStartLBA + 1) / ERASE_UNIT_LBA_COUNT;
	eraseCount += ((endLBA - changedStartLBA + 1) % ERASE_UNIT_LBA_COUNT) ? 1 : 0;

	for (int loopCount = 0; loopCount < eraseCount; loopCount++)
	{
		int eraseLbaCount = ERASE_UNIT_LBA_COUNT;
		if (endLBA < changedStartLBA + ERASE_UNIT_LBA_COUNT) eraseLbaCount = getLastEraseLbaCount(changedStartLBA);

		eraseResult = cmdRequester->erase(changedStartLBA, ERASE_UNIT_LBA_COUNT);
		if ("" != eraseResult) return false;
		changedStartLBA += ERASE_UNIT_LBA_COUNT;
	}
	return true;
}

void EraseCommand::setEndLBA()
{
	endLBA = startLBA + size - 1;
}

int EraseCommand::getLastEraseLbaCount(int changedStartLBA)
{
	return endLBA - changedStartLBA + 1;
}
