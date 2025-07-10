#include <regex>
#include <sstream>
#include "test_shell.h"

void TestShell::setSsdAdapter(SSDInterface* adapter)
{
	ssdAdapter = adapter;
	ICommand::setSsdAdapter(adapter);
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
	std::istringstream iss(command);
	std::string cmd;
	iss >> cmd;

	if (false == isSSDCommand(cmd))	return processTestShellCommand(cmd);
	return processSsdTest(command);
}

bool TestShell::isSSDCommand(const std::string& cmdType) {
	if ((cmdType == HELP_COMMAND_NAME) || (cmdType == EXIT_COMMAND_NAME)) return false;
	return true;
}

NEXT_TEST TestShell::processTestShellCommand(const std::string& cmdType)
{
	if (cmdType == EXIT_COMMAND_NAME) {
		std::cout << "PROGRAM EXIT" << std::endl;
		return NEXT_EXIT;
	}
	else if (cmdType == "help") {
		printHelp();
		return NEXT_KEEP_GOING;
	}

	return NEXT_KEEP_GOING;
}

NEXT_TEST TestShell::processSsdTest(const std::string& command)
{
	std::istringstream iss(command);
	std::string cmd;
	iss >> cmd;

	std::unique_ptr<ICommand> cmdPtr = cmdCreator.createCommand(cmd);
	if (cmdPtr == nullptr)
	{
		std::cout << INVALID_COMMAND_MSG << std::endl;
		return NEXT_KEEP_GOING;
	}

	int posParam = command.find(cmd) + cmd.size();
	int sizeParam = command.find(cmd) - cmd.size();
	string cmdparam = command.substr(posParam, sizeParam);
	cmdPtr->process(cmdparam, iss);

	return NEXT_KEEP_GOING;
}

void TestShell::printHelp()
{
	LOG_PRINT("called");
	std::cout << "--------------------------------- HELP "
		<< "---------------------------------" << std::endl;

	for (std::string cmdType : cmdCreator.getAllCommandType())
	{
		std::unique_ptr<ICommand> cmdPtr = cmdCreator.createCommand(cmdType);
		cmdPtr->printHelp();
	}

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
		readWithPrintScreen(LBA);
	}
}

void TestShell::fullWriteAndReadCompare()
{
	LOG_PRINT("called");
	int j = 1;
	bool failFlag = false;
	for (int i = 0; i < 20; i++) {
		auto testString = intToHexString(i);
		for (int j = 0; j < 5; j++) {
			auto ret = ssdAdapter->write(5 * i + j, testString);
			if (ret.compare("") != 0) {
				failFlag = true;
				break;
			}
		}
		// TODO: fix to if (isFail()) break;
		if (failFlag == true) break;

		for (int j = 0; j < 5; j++) {
			auto ret = ssdAdapter->read(5 * i + j);
			if (testString.compare(ret) != 0) {
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

string TestShell::readWithPrintScreen(const int LBA)
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
		if (WRITE_DONE_RETURN != write(lba, data))
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
		if (readReturn != readWithPrintScreen(lba))
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