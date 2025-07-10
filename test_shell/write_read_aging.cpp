#include "write_read_aging.h"


NEXT_TEST WriteReadAgingCommand::process(std::istringstream& iss)
{
	LOG_PRINT("called");
	bool allMatch = true;

	for (int i = 0; i < 200; ++i) {
		std::stringstream ss;
		ss << "0x" << std::uppercase << std::hex << (rand() & 0xFFFFFFFF);
		std::string randData = ss.str();

		cmdRequester->write(0, randData);
		cmdRequester->write(99, randData);

		std::string result0 = cmdRequester->read(0);
		std::string result99 = cmdRequester->read(99);

		if (result0 != result99) {
			string errorMessage = "[WriteReadAging] ERROR mismatch value LBA[0] : " + result0 + " LBA[99] : " + result99;
			printLog(errorMessage);
			allMatch = false;
		}
	}

	if (allMatch) {
		printLog(getDoneMessage());
	}

	return NEXT_KEEP_GOING;
}

void WriteReadAgingCommand::printHelp()
{
	std::cout << " Test script - 3 WriteReadAging\n"
		"\trepeats following steps 200 times \n" <<
		"\t  step1) write the data to lba 0\n" <<
		"\t  step2) write the data to lba 99\n" <<
		"\t  step3) read the data to lba 0\n" <<
		"\t  step4) read the data to lba 99\n" <<
		"\t  step6) Check if data of LBA 0 and 99 is the same\n" <<
		"\t usage - 3_WriteReadAging(or 3_)" << std::endl;
}
