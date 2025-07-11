#include "erase_range_command.h"

void EraseRangeCommand::printHelp()
{
	std::cout << " ERASE RANGE - erase at LBA(Logical Block Addressing) Range \n" <<
		"\t usage - erase_range <start LBA> <End LBA> (ex.erase_range 0 10)" << std::endl;
}

bool EraseRangeCommand::prepare(std::istringstream& iss)
{
	if (false == getEraseParameter(iss)) return false;

	if (false == isVaiidEraseRange())
	{
		printLog(getErrorHeader() + ": invalid Range(startLBA, size)");
		return false;
	}

	return true;
}

bool EraseRangeCommand::execute()
{
	if (false == eraseRange())
	{
		printLog(getErrorHeader() + ": Erase Fail");
		return false;
	}
	return true;
}

void EraseRangeCommand::wrapUp(bool noError)
{
	if (noError) logMessage = getDoneMessage();
	printLog(logMessage);
}

bool EraseRangeCommand::getEraseParameter(std::istringstream& iss)
{
	if (!(iss >> startLBA))
	{
		printLog(getErrorHeader() + ": missing startLBA");
		return false;
	}

	if (!(iss >> endLBA))
	{
		printLog(getErrorHeader() + ": missing endLBA");
		return false;
	}

	return true;
}

bool EraseRangeCommand::isVaiidEraseRange()
{
	LOG_PRINT("called");
	if (startLBA < 0)  return false;
	else if (endLBA >= SSD_SIZE) return false;
	else if (startLBA > endLBA) return false;

	return true;
}

bool EraseRangeCommand::eraseRange()
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

		eraseResult = cmdRequester->erase(changedStartLBA, eraseLbaCount);
		if ("" != eraseResult) return false;
		changedStartLBA += ERASE_UNIT_LBA_COUNT;
	}

	return true;
}

int EraseRangeCommand::getLastEraseLbaCount(int changedStartLBA)
{
	return endLBA - changedStartLBA + 1;
}