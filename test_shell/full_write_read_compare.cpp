#include "full_write_read_compare.h"

void FullWriteReadCompareCommand::printHelp()
{
	std::cout << " Test script - 1 FullWriteAndReadCompare \n" <<
		"\t write and read Test all indices in 5-unit\n" <<
		"\t usage - 1_FullWriteAndReadCompare(or 1_)" << std::endl;
}

bool FullWriteReadCompareCommand::prepare(std::istringstream& iss)
{
	return true;
}

bool FullWriteReadCompareCommand::execute()
{
	for (int loopCount = 0; loopCount < LOOP_COUNT_FOR_FULL_WRC; loopCount++) {
		string testData = intToHexString(loopCount);

		if (false == writeTest(loopCount, testData))
		{
			logMessage = getErrorHeader() + ": Write Fail";
			return false;
		}

		if (false == readTest(loopCount, testData))
		{
			logMessage = getErrorHeader() + ": Read Fail";
			return false;
		}
	}
	return true;
}

void FullWriteReadCompareCommand::wrapUp(bool noError)
{
	if (noError) logMessage = getDoneMessage();
	printLog(logMessage);
}

bool FullWriteReadCompareCommand::writeTest(int sequenceNumber, const string& testData)
{
	for (int lbaOffset = 0; lbaOffset < LBA_COUNT_FOR_FULL_WRC; lbaOffset++) {
		auto ret = cmdRequester->write(LBA_COUNT_FOR_FULL_WRC * sequenceNumber + lbaOffset, testData);
		if (ret.compare("") != 0) {
			return false;
		}
	}
	return true;
}

bool FullWriteReadCompareCommand::readTest(int sequenceNumber, const string& testData)
{
	for (int lbaOffset = 0; lbaOffset < LBA_COUNT_FOR_FULL_WRC; lbaOffset++) {
		auto ret = cmdRequester->read(LBA_COUNT_FOR_FULL_WRC * sequenceNumber + lbaOffset);
		if (testData.compare(ret) != 0) {
			return false;
		}
	}
	return true;
}


string FullWriteReadCompareCommand::intToHexString(int value) {
	LOG_PRINT("value: " + std::to_string(value));
	std::stringstream ss;
	ss << "0x"
		<< std::setfill('0') << std::setw(8)
		<< std::hex << std::uppercase
		<< value;
	return ss.str();
}