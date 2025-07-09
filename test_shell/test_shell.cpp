#include <regex>
#include <sstream>
#include "test_shell.h"

void TestShell::setSsdAdapter(SSDInterface* adapter)
{
	ssdAdapter = adapter;
}

void TestShell::runShell() {
	LOG_PRINT("called");
	std::string command;

	while (true) {
		std::cout << " SHELL> ";
		if (!std::getline(std::cin, command)) break;

		int retFlag = runCommand(command);
		if (retFlag == NEXT_EXIT) break;
	}
}

int TestShell::runCommand(const std::string& command)
{
	LOG_PRINT("called");
	int retFlag = NEXT_KEEP_GOING;
	std::istringstream iss(command);
	std::string cmd;
	iss >> cmd;

	if (cmd == "exit") {
		std::cout << "PROGRAM EXIT" << std::endl;
		return NEXT_EXIT;
	}

	if (cmd == "help") {
		printHelp();
		return NEXT_KEEP_GOING;
	}

	if (cmd == "1_" || cmd == "1_FullWriteAndReadCompare") {
		fullWriteAndReadCompare();
		return NEXT_KEEP_GOING;
	}

	if ((TEST_SCRIPT_2_FULL_COMMAND_NAME == command)
		|| (TEST_SCRIPT_2_SHORT_COMMAND_NAME == command))
	{
		partialLBAWrite();
		return NEXT_KEEP_GOING;
	}

	if (("3_WriteReadAging" == command) || ("3_" == command)) {
		writeReadAging();
		return NEXT_KEEP_GOING;
	}

	if ((TEST_SCRIPT_4_FULL_COMMAND_NAME == command)
		|| (TEST_SCRIPT_4_SHORT_COMMAND_NAME == command)) {
		eraseWriteAging();
		return NEXT_KEEP_GOING;
	}

	if (cmd == "write") {
		int lba;
		std::string data;
		std::string extra;
		if (!(iss >> lba)) {
			std::cout << "[Write] ERROR: Missing lba" << std::endl;
			return NEXT_KEEP_GOING;
		}

		if (!(iss >> data)) {
			std::cout << "[Write] ERROR: Missing data" << std::endl;
			return NEXT_KEEP_GOING;
		}

		if (iss >> extra) {
			std::cout << "[Write] ERROR: Too many arguments" << std::endl;
			return NEXT_KEEP_GOING;
		}

		std::string result = write(lba, data);
		std::cout << result << std::endl;
		return NEXT_KEEP_GOING;
	}

	if (cmd == "read") {
		int LBA;

		if (!(iss >> LBA)) {
			std::cout << "[Read] ERROR: Missing LBA" << std::endl;
			return NEXT_KEEP_GOING;
		}

		string result = read(LBA);

		if (result == "[Read] ERROR") return 1;
		return NEXT_KEEP_GOING;
	}

	if (cmd == "fullread") {
		fullRead();
		return NEXT_KEEP_GOING;
	}

	if (cmd == "erase")
	{
		eraseWithSize(iss);
		return NEXT_KEEP_GOING;
	}

	if (cmd == "erase_range")
	{
		eraseWithEndLBA(iss);
		return NEXT_KEEP_GOING;
	}

	if (cmd == "fullwrite") {
		std::string data;

		if (!(iss >> data)) {
			std::cout << "[Write] ERROR: Missing data" << std::endl;
			return NEXT_KEEP_GOING;
		}

		fullWrite(data);
		return NEXT_KEEP_GOING;
	}

	// Reaching here means that the command is invalid.
	std::cout << INVALID_COMMAND_MSG << std::endl;

	return retFlag;
}

void TestShell::printHelp()
{
	LOG_PRINT("called");
	std::cout << "--------------------------------- HELP "
		<< "---------------------------------" << std::endl;
	std::cout << " READ - read one LBA (Logical Block Addressing) \n" <<
		"\t usage - read <LBA>(ex.read 0)" << std::endl;
	std::cout << " WRITE - write value to LBA(Logical Block Addressing) \n" <<
		"\t usage - write <LBA> <value> (ex.write 3 0xAAAABBBB)" << std::endl;
	std::cout << " ERASE - erase at LBA(Logical Block Addressing) Range \n" <<
		"\t usage1 - erase <start LBA> <size> (ex.erase 0 10)\n" <<
		"\t usage2 - erase_range <start LBA> <end LBA> (ex.erase_range 0 10)" << std::endl;
	std::cout << " Test script - 1 FullWriteAndReadCompare \n" <<
		"\t write and read Test all indices in 5-unit\n" <<
		"\t usage - 1_FullWriteAndReadCompare(or 1_)" << std::endl;
	std::cout << " Test script - 2 PartialLBAWrite\n" <<
		"\trepeats following steps 30 times \n" <<
		"\t  step1) write the data to lba 4\n" <<
		"\t  step2) write the data to lba 0\n" <<
		"\t  step3) write the data to lba 3\n" <<
		"\t  step4) write the data to lba 1\n" <<
		"\t  step5) write the data to lba 2\n" <<
		"\t  step6) Check if data of all LBA 0 to 4 are the same\n" <<
		"\t usage - 2_PartialLBAWrite(or 2_)" << std::endl;
	std::cout << " Test script - 3 WriteReadAging\n"
		"\trepeats following steps 200 times \n" <<
		"\t  step1) write the data to lba 0\n" <<
		"\t  step2) write the data to lba 99\n" <<
		"\t  step3) read the data to lba 0\n" <<
		"\t  step4) read the data to lba 99\n" <<
		"\t  step6) Check if data of LBA 0 and 99 is the same\n" <<
		"\t usage - 3_WriteReadAging(or 3_)" << std::endl;
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
	std::cout << "---------------------------------------"
		<< "---------------------------------" << std::endl;
}

void TestShell::fullWrite(const string& data) {
	LOG_PRINT("called");
	for (int i = 0; i < 100; ++i) {
		std::string result = write(i, data);
		if (result == "[Write] ERROR") {
			LOG_PRINT("Failed at LBA " + std::to_string(i));
			std::cout << "[fullWrite] Failed at LBA " << i << std::endl;
			return;
		}
	}
	std::cout << "[fullWrite] Done" << std::endl;
}

void TestShell::fullRead()
{
	LOG_PRINT("called");
	for (int LBA = 0; LBA < SSD_SIZE; LBA++) {
		read(LBA);
	}
}

void TestShell::fullWriteAndReadCompare()
{
	LOG_PRINT("called");
	int j = 1;
	bool failFlag = false;
	for (int i = 0; i < 20; i++) {
		auto test_string = intToHexString(i);
		for (int j = 0; j < 5; j++) {
			auto ret = ssdAdapter->write(5 * i + j, test_string);
			if (ret != "") {
				LOG_PRINT("FAIL: ssdAdapter->write " + std::to_string(i) + " " + std::to_string(j));
				failFlag = true;
				break;
			}
		}
		// TODO: fix to if (isFail()) break;
		if (failFlag == true) break;

		for (int j = 0; j < 5; j++) {
			if (test_string.compare(ssdAdapter->read(5 * i + j)) == 0) {
				failFlag = true;
				break;
			}
		}
		if (failFlag == true) break;
	}
	if (failFlag == true) {
		std::cout << "FAIL" << std::endl;
	}
	else {
		std::cout << "PASS" << std::endl;
	}
}

void TestShell::partialLBAWrite(const string& data)
{
	LOG_PRINT("called");
	vector<int> lbaSequence = INPUT_LBA_SEQUENCE;

	for (int count = 0; count < LOOP_COUNT_FOR_PARTIAL_LBA_WRITE; count++)
	{
		if (false == writeTheSequence(lbaSequence, data)) return;

		if (false == verifyTheSequence(data, lbaSequence)) return;
	}

	LOG_PRINT(TEST_SCRIPT_2_SUCCESS_MSG);
	std::cout << TEST_SCRIPT_2_SUCCESS_MSG << std::endl;
}

void TestShell::writeReadAging() {
	LOG_PRINT("called");
	bool allMatch = true;

	for (int i = 0; i < 200; ++i) {
		std::stringstream ss;
		ss << "0x" << std::uppercase << std::hex << (rand() & 0xFFFFFFFF);
		std::string randData = ss.str();

		ssdAdapter->write(0, randData);
		ssdAdapter->write(99, randData);

		std::string result0 = ssdAdapter->read(0);
		std::string result99 = ssdAdapter->read(99);

		if (result0 != result99) {
			string errorMessage = "[Aging] ERROR mismatch value LBA[0] : " + result0 + " LBA[99] : " + result99;
			LOG_PRINT(errorMessage);
			std::cout << "[Aging] ERROR mismatch value LBA[0] : " << result0 << " LBA[99] : " << result99 << std::endl;
			allMatch = false;
		}
	}

	if (allMatch) {
		LOG_PRINT("[Aging] PASS");
		std::cout << "[Aging] PASS" << std::endl;
	}
}

string TestShell::write(const int LBA, const string& data) {
	LOG_PRINT("LBA(" + std::to_string(LBA) + ") data(" + data + ")");
	if (!ssdAdapter) return WRITE_ERROR_RETURN;

	std::string result = ssdAdapter->write(LBA, data);
	if (result == "") return WRITE_DONE_RETURN;
	else return WRITE_ERROR_RETURN;
}

string TestShell::read(const int LBA)
{
	LOG_PRINT("LBA(" + std::to_string(LBA) + ")");
	string result = ssdAdapter->read(LBA);
	if (result == "ERROR") result = "[Read] ERROR";
	else result = "[Read] LBA " + std::to_string(LBA) + " : " + result;
	std::cout << result << std::endl;

	return result;
}

string TestShell::erase(const int LBA, const int size)
{
	LOG_PRINT("LBA(" + std::to_string(LBA) + ") size(" + std::to_string(size) + ")");
	string result = ssdAdapter->erase(LBA, size);
	if (result == "ERROR") result = ERASE_FAIL_RETURN;
	else result = ERASE_DONE_RETURN;
	std::cout << result << std::endl;

	return result;
}

string TestShell::intToHexString(int value) {
	LOG_PRINT("value: " + std::to_string(value));
	std::stringstream ss;
	ss << "0x"
		<< std::setfill('0') << std::setw(8)
		<< std::hex << std::uppercase
		<< value;
	return ss.str();
}

bool TestShell::writeTheSequence(const std::vector<int>& lbaSequence, const std::string& data)
{
	LOG_PRINT("called");
	for (int lba : lbaSequence)
	{
		if (RETURN_WRITE_DONE != write(lba, data))
		{
			std::cout << TEST_SCRIPT_2_WRITE_FAIL_MSG << std::endl;
			return false;
		}
	}
	return true;
}

bool TestShell::verifyTheSequence(
	const std::string& data, const vector<int>& lbaSequence)
{
	LOG_PRINT("called");
	for (int lba : lbaSequence)
	{
		string readReturn = "[Read] LBA " + std::to_string(lba) + " : " + data;
		if (readReturn != read(lba))
		{
			LOG_PRINT(TEST_SCRIPT_2_VERIFY_FAIL_MSG);
			std::cout << TEST_SCRIPT_2_VERIFY_FAIL_MSG << std::endl;
			return false;
		}
	}
	return true;
}

void TestShell::eraseWithSize(std::istringstream& iss)
{
	LOG_PRINT("called");
	int startLBA = 0;
	int size = 0;

	if (false == getEraseParameter(startLBA, size, iss)) return;

	int endLBA = startLBA + size - 1;
	if (false == isVaiidEraseRange(startLBA, endLBA))
	{
		LOG_PRINT("[Erase] Error: invalid Range(startLBA, size)");
		std::cout << "[Erase] Error: invalid Range(startLBA, size)\n";
		return;
	}

	if (false == eraseRange(startLBA, endLBA))
	{
		LOG_PRINT("[Erase] Error: Erase Operation Fail");
		std::cout << "[Erase] Error: Erase Operation Fail\n";
		return;
	}

	std::cout << ERASE_DONE_RETURN << std::endl;
	return;
}

void TestShell::eraseWithEndLBA(std::istringstream& iss)
{
	LOG_PRINT("called");
	int startLBA = 0;
	int endLBA = 0;

	if (false == getEraseParameter(startLBA, endLBA, iss)) return;

	if (false == isVaiidEraseRange(startLBA, endLBA))
	{
		LOG_PRINT("[Erase_Range] Error: invalid Range(startLBA, endLBA)");
		std::cout << "[Erase_Range] Error: invalid Range(startLBA, endLBA)\n";
		return;
	}

	if (false == eraseRange(startLBA, endLBA))
	{
		LOG_PRINT("[Erase_Range] Error: Erase Operation Fail");
		std::cout << "[Erase_Range] Error: Erase Operation Fail\n";
		return;
	}

	std::cout << ERASE_RANGE_DONE_RETURN << std::endl;
	return;
}

bool TestShell::getEraseParameter(int& startLBA, int& size, std::istringstream& iss)
{
	LOG_PRINT("called");
	if (!(iss >> startLBA))
	{
		LOG_PRINT("[Erase] Error: missing startLBA");
		std::cout << "[Erase] Error: missing startLBA\n";
		return false;
	}

	if (!(iss >> size))
	{
		LOG_PRINT("[Erase] Error: missing size");
		std::cout << "[Erase] Error: missing size\n";
		return false;
	}

	return true;
}

bool TestShell::isVaiidEraseRange(const int startLBA, const int endLBA)
{
	LOG_PRINT("called");
	if (startLBA < 0)  return false;
	else if (endLBA >= SSD_SIZE) return false;
	else if (startLBA >= endLBA) return false;

	return true;
}

bool TestShell::eraseRange(int startLBA, int endLBA)
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
			eraseResult = erase(startLBA, endLBA - startLBA + 1);
			if ("[Erase] Done" != eraseResult) return false;
			break;
		}
		else
		{
			eraseResult = erase(startLBA, ERASE_UNIT_LBA_COUNT);
			if ("[Erase] Done" != eraseResult) return false;
			startLBA += ERASE_UNIT_LBA_COUNT;
		}
	}

	return true;
}

void TestShell::eraseWriteAging() {
	LOG_PRINT("called");
	bool AgingPass = false;
	const int firstEraseLBA = 0;
	const int firstEraseSize = 3;

	const int firstStartLBAForAging = 2;
	const int lastStartLBAForAging = 98;
	const int incrementNextLBA = 2;

	if (ERASE_FAIL_RETURN == erase(firstEraseLBA, firstEraseSize))
	{
		LOG_PRINT("[EraseAndWriteAging] 1st Erase Fail");
		std::cout << "[EraseAndWriteAging] 1st Erase Fail" << std::endl;
		return;
	}

	for (int count = 0; count < LOOP_COUNT_FOR_AGING; ++count) {
		if (false == eraseWriteAgingOneCycle(firstStartLBAForAging, lastStartLBAForAging, incrementNextLBA))
		{
			LOG_PRINT("[EraseAndWriteAging] Fail");
			std::cout << "[EraseAndWriteAging] Fail" << std::endl;
			return;
		}
	}

	LOG_PRINT("[EraseAndWriteAging] Done");
	std::cout << "[EraseAndWriteAging] Done" << std::endl;
}

bool TestShell::eraseWriteAgingOneCycle(int firstStartLBA, int lastStartLBA, int increment)
{
	std::string randData;
	const int SSD_LAST_LBA = SSD_SIZE - 1;
	const int ERASE_LBA_COUUNT = 3;
	int actualEraseCount = ERASE_LBA_COUUNT;
	int eraseLastLBA = 0;
#ifdef _DEBUG
	srand(time(0));
#endif

	for (int startLBA = firstStartLBA; startLBA <= lastStartLBA; startLBA += increment)
	{
		randData = getRandomInput();
		if (WRITE_ERROR_RETURN == write(startLBA, randData)) return false;

		randData = getRandomInput();
		if (WRITE_ERROR_RETURN == write(startLBA, randData)) return false;

		eraseLastLBA = startLBA + ERASE_LBA_COUUNT - 1;

		if (eraseLastLBA > SSD_LAST_LBA)
		{
			// for last erase case
			actualEraseCount = SSD_SIZE - startLBA;
		}

		if (ERASE_FAIL_RETURN == erase(startLBA, actualEraseCount))
		{
			return false;
		}
	}

	return true;
}

string TestShell::getRandomInput()
{
	std::stringstream ss;
	ss << "0x" << std::uppercase << std::hex << (rand() & 0xFFFFFFFF);
	std::string randData = ss.str();
	ss.str("");
	ss.clear();

	return randData;
}