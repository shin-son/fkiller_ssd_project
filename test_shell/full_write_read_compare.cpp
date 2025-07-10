#include "full_write_read_compare.h"

NEXT_TEST FullWriteReadCompareCommand::process(std::istringstream& iss)
{
    int j = 1;
    bool failFlag = false;
    for (int i = 0; i < 20; i++) {
        auto testString = intToHexString(i);
        for (int j = 0; j < 5; j++) {
            auto ret = cmdRequester->write(5 * i + j, testString);
            if (ret.compare("") != 0) {
                failFlag = true;
                break;
            }
        }
        // TODO: fix to if (isFail()) break;
        if (failFlag == true) break;

        for (int j = 0; j < 5; j++) {
            auto ret = cmdRequester->read(5 * i + j);
            if (testString.compare(ret) != 0) {
                failFlag = true;
                break;
            }
        }
        if (failFlag == true) break;
    }
    if (failFlag == true) {
        printLog(getErrorHeader());
    }
    else {
        printLog(getDoneMessage());
    }
	return NEXT_KEEP_GOING;
}

void FullWriteReadCompareCommand::printHelp()
{
	std::cout << " Test script - 1 FullWriteAndReadCompare \n" <<
		"\t write and read Test all indices in 5-unit\n" <<
		"\t usage - 1_FullWriteAndReadCompare(or 1_)" << std::endl;
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