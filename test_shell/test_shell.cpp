#include <regex>
#include <sstream>
#include "test_shell.h"

void TestShell::setSsdAdapter(SSDInterface* adapter)
{
	ssdAdapter = adapter;
	ICommand::setSsdAdapter(adapter);
}

void TestShell::runShell() {
	std::string command;

	while (true) {
		std::cout << " SHELL> ";
		if (!std::getline(std::cin, command)) break;
		if (runCommand(command) == NEXT_EXIT) break;
	}
}

int TestShell::runCommand(const std::string& command)
{
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
		LOG_PRINT("PROGRAM EXIT");
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
		LOG_PRINT(INVALID_COMMAND_MSG);
		std::cout << INVALID_COMMAND_MSG << std::endl;
		return NEXT_KEEP_GOING;
	}
	return cmdPtr->process(iss);
}

void TestShell::printHelp()
{
	LOG_PRINT("called");
	
	std::cout << "=========================-TEAM F KILLER "
		<< "SSD PROJECT=======================" << std::endl;

	std::cout << "-----------------------------------------\n";
	std::cout << "              Contributors        \n";
	std::cout << "-----------------------------------------\n";
	std::cout << " ��  Shin Son           (@shin-son)\n";
	std::cout << " ��  Jongwon Park         (@jwon96park)\n";
	std::cout << " ��  Dongguen Lim       (@Limdongguen)\n";
	std::cout << " ��  Hyungju Song       (@HyungjuSong5)\n";
	std::cout << " ��  Sungmin Kim        (@Kim-Sung-Min87)\n";
	std::cout << " ��  Jaewon Song        (@jw.song)\n";
	std::cout << "-----------------------------------------\n";

	for (std::string cmdType : cmdCreator.getAllCommandType())
	{
		std::unique_ptr<ICommand> cmdPtr = cmdCreator.createCommand(cmdType);
		cmdPtr->printHelp();
	}

	std::cout << "======================================="
		<< "=================================" << std::endl;
}

#ifdef _DEBUG
string TestShell::intToHexString(int value) {
	std::stringstream ss;
	ss << "0x"
		<< std::setfill('0') << std::setw(8)
		<< std::hex << std::uppercase
		<< value;
	return ss.str();
}
#endif