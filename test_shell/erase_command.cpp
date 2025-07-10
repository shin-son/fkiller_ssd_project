#include "erase_command.h"

NEXT_TEST EraseCommand::process(const string& command, std::istringstream& iss)
{
	LOG_PRINT("called");
	int startLBA = 0;
	int size = 0;

	if (false == getEraseParameter(startLBA, size, iss)) return NEXT_KEEP_GOING;

	int endLBA = startLBA + size - 1;
	if (false == isVaiidEraseRange(startLBA, endLBA))
	{
		printLog(getErrorHeader() + ": invalid Range(startLBA, size)");
		return NEXT_KEEP_GOING;
	}

	if (false == eraseRange(startLBA, endLBA))
	{
		printLog(getErrorHeader() + ": Erase Fail");
		return NEXT_KEEP_GOING;
	}

	printLog(getDoneMessage());

	return NEXT_KEEP_GOING;
}

void EraseCommand::printHelp()
{
	std::cout << " ERASE - erase at LBA(Logical Block Addressing) Range \n" <<
		"\t usage - erase <start LBA> <size> (ex.erase 0 10)" << std::endl;
}

bool EraseCommand::getEraseParameter(
	int& startLBA, int& size, std::istringstream& iss)
{
	LOG_PRINT("called");
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

bool EraseCommand::isVaiidEraseRange(const int startLBA, const int endLBA)
{
	LOG_PRINT("called");
	if (startLBA < 0)  return false;
	else if (endLBA >= SSD_SIZE) return false;
	else if (startLBA >= endLBA) return false;

	return true;
}

bool EraseCommand::eraseRange(int startLBA, int endLBA)
{
	LOG_PRINT("called");
	int retSuccess = true;
	string eraseResult = "";
	int eraseCount = (endLBA - startLBA + 1) / ERASE_UNIT_LBA_COUNT;
	eraseCount += ((endLBA - startLBA + 1) % ERASE_UNIT_LBA_COUNT) ? 1 : 0;

	for (int loopCount = 0; loopCount < eraseCount; loopCount++)
	{
		if (endLBA < startLBA + ERASE_UNIT_LBA_COUNT)
		{
			eraseResult = adapter->erase(startLBA, endLBA - startLBA + 1);
			if ("" != eraseResult) return false;
			break;
		}
		else
		{
			eraseResult = adapter->erase(startLBA, ERASE_UNIT_LBA_COUNT);
			if ("" != eraseResult) return false;
			startLBA += ERASE_UNIT_LBA_COUNT;
		}
	}

	return true;
}