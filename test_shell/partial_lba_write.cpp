#include "partial_lba_write.h"

NEXT_TEST PartialLbaWriteCommand::process(std::istringstream& iss)
{

	printLog(getDoneMessage());

	return NEXT_KEEP_GOING;
}

void PartialLbaWriteCommand::printHelp()
{
	std::cout << " Test script - 2 PartialLBAWrite\n" <<
		"\trepeats following steps 30 times \n" <<
		"\t  step1) write the data to lba 4\n" <<
		"\t  step2) write the data to lba 0\n" <<
		"\t  step3) write the data to lba 3\n" <<
		"\t  step4) write the data to lba 1\n" <<
		"\t  step5) write the data to lba 2\n" <<
		"\t  step6) Check if data of all LBA 0 to 4 are the same\n" <<
		"\t usage - 2_PartialLBAWrite(or 2_)" << std::endl;
}

bool PartialLbaWriteCommand::prepare(std::istringstream& iss)
{
	return true;
}

bool PartialLbaWriteCommand::execute()
{
	vector<int> lbaSequence = INPUT_LBA_SEQUENCE;

	for (int count = 0; count < LOOP_COUNT_FOR_PARTIAL_LBA_WRITE; count++)
	{
		if (false == writeTheSequence(INPUT_DATA_FOR_PARTIAL_LBA_WRITE, lbaSequence))
		{
			logMessage = getErrorHeader() + ": Write Fail";
			return false;
		}
		
		if (false == verifyTheSequence(INPUT_DATA_FOR_PARTIAL_LBA_WRITE, lbaSequence))
		{
			logMessage = getErrorHeader() + ": Verify Fail";
			return false;
		}
	}
	return true;
}

void PartialLbaWriteCommand::wrapUp(bool noError)
{	
	if (noError) logMessage = getDoneMessage();
	printLog(logMessage);
}

bool PartialLbaWriteCommand::writeTheSequence(
	const std::string& data, const vector<int>& lbaSequence)
{
	for (int lba : lbaSequence)
	{
		if ("" != cmdRequester->write(lba, data))
		{
			std::cout << TEST_SCRIPT_2_WRITE_FAIL_MSG << std::endl;
			return false;
		}
	}
	return true;
}

bool PartialLbaWriteCommand::verifyTheSequence(
	const std::string& data, const vector<int>& lbaSequence)
{
	for (int lba : lbaSequence)
	{
		string readReturn = "LBA " + std::to_string(lba) + " : " + data;
		if ((data != cmdRequester->read(lba) || (ERROR == cmdRequester->read(lba))))
		{
			LOG_PRINT(TEST_SCRIPT_2_VERIFY_FAIL_MSG);
			std::cout << TEST_SCRIPT_2_VERIFY_FAIL_MSG << std::endl;
			return false;
		}
	}
	return true;
}