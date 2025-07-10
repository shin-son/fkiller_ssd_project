#include "erase_range_command.h"

NEXT_TEST EraseRangeCommand::process(const string& command, std::istringstream& iss)
{
	LOG_PRINT("called");
	int startLBA = 0;
	int endLBA = 0;

	if (false == getEraseParameter(startLBA, endLBA, iss)) return NEXT_KEEP_GOING;

	if (false == isVaiidEraseRange(startLBA, endLBA))
	{
		LOG_PRINT("[Erase] ERROR: invalid Range(startLBA, size)");
		std::cout << "[EraseRange] ERROR: invalid Range(startLBA, size)\n";
		return NEXT_KEEP_GOING;
	}

	if (false == eraseRange(startLBA, endLBA))
	{
		LOG_PRINT("[Erase] ERROR: Erase Operation Fail");
		std::cout << "[EraseRange] ERROR: Erase Operation Fail\n";
		return NEXT_KEEP_GOING;
	}

	std::cout << ERASE_RANGE_DONE_RETURN << std::endl;

	return NEXT_KEEP_GOING;
}

void EraseRangeCommand::printHelp()
{
	std::cout << " ERASE - erase at LBA(Logical Block Addressing) Range \n" <<
		"\t usage - erase <start LBA> <size> (ex.erase 0 10)" << std::endl;
}

bool EraseRangeCommand::getEraseParameter(
	int& startLBA, int& size, std::istringstream& iss)
{
	LOG_PRINT("called");
	if (!(iss >> startLBA))
	{
		LOG_PRINT("[Erase] ERROR: missing startLBA");
		std::cout << "[EraseRange] ERROR: missing startLBA\n";
		return false;
	}

	if (!(iss >> size))
	{
		LOG_PRINT("[Erase] ERROR: missing size");
		std::cout << "[EraseRange] ERROR: missing size\n";
		return false;
	}

	return true;
}

bool EraseRangeCommand::isVaiidEraseRange(const int startLBA, const int endLBA)
{
	LOG_PRINT("called");
	if (startLBA < 0)  return false;
	else if (endLBA >= SSD_SIZE) return false;
	else if (startLBA >= endLBA) return false;

	return true;
}

bool EraseRangeCommand::eraseRange(int startLBA, int endLBA)
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