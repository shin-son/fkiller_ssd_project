#include "erase_write_aging.h"

NEXT_TEST EraseWriteAgingCommand::process(const string& command, std::istringstream& iss)
{
	bool AgingPass = false;
	const int firstEraseLBA = 0;
	const int firstEraseSize = 3;

	const int firstStartLBAForAging = 2;
	const int lastStartLBAForAging = 98;
	const int incrementNextLBA = 2;

	if (ERROR == adapter->erase(firstEraseLBA, firstEraseSize))
	{
		printLog(getErrorHeader() + ": 1st Erase Fail");
		return NEXT_KEEP_GOING;
	}

	for (int count = 0; count < LOOP_COUNT_FOR_AGING; ++count) {
		if (false == eraseWriteAgingOneCycle(firstStartLBAForAging, lastStartLBAForAging, incrementNextLBA))
		{
			printLog(getErrorHeader() + ": " + "Fail while" + std::to_string(count + 1) + " cycle");
			return NEXT_KEEP_GOING;
		}
	}

	printLog(getDoneMessage());
	return NEXT_KEEP_GOING;
}

void EraseWriteAgingCommand::printHelp()
{
	std::cout << " Test script - 4 EraseAndWriteAging\n"
		"\t  erase lba 0~2\n" <<
		"\trepeats following steps 30 times) \n" <<
		"\t  step1) write lba 2 two times with different random values\n" <<
		"\t  step2) erase lba 2~4with another random value\n" <<
		"\t  step3) write lba 4 two times with different random values\n" <<
		"\t  step4) erase lba 4~6with another random value\n" <<
		"\t ...\n" <<
		"\t  step49) write lba 98 two times with different random values\n" <<
		"\t  step50) erase lba 98~99with another random value" << std::endl;
}

bool EraseWriteAgingCommand::eraseWriteAgingOneCycle(int firstStartLBA, int lastStartLBA, int increment)
{
	std::string randData;
	const int SSD_LAST_LBA = SSD_SIZE - 1;
	const int ERASE_LBA_COUUNT = 3;
	int actualEraseCount = ERASE_LBA_COUUNT;
	int eraseLastLBA = 0;

	for (int startLBA = firstStartLBA; startLBA <= lastStartLBA; startLBA += increment)
	{
		randData = getRandomInput();
		if (ERROR == adapter->write(startLBA, randData)) return false;

		randData = getRandomInput();
		if (ERROR == adapter->write(startLBA, randData)) return false;

		eraseLastLBA = startLBA + ERASE_LBA_COUUNT - 1;

		if (eraseLastLBA > SSD_LAST_LBA)
		{
			// for last erase case
			actualEraseCount = SSD_SIZE - startLBA;
		}

		if (ERROR == adapter->erase(startLBA, actualEraseCount))
		{
			return false;
		}
	}

	return true;
}

string EraseWriteAgingCommand::getRandomInput()
{
#ifdef _DEBUG
	srand(time(0));
#endif

	std::stringstream ss;
	ss << "0x" << std::uppercase << std::hex << (rand() & 0xFFFFFFFF);
	std::string randData = ss.str();
	ss.str("");
	ss.clear();

	return randData;
}