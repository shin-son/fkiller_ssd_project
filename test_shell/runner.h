#pragma once
#include "test_shell.h"
#include "ssd_adapter.h"

class TestRunner {
public:
	explicit TestRunner(TestShell& testShell, SSDAdapter& ssdAdapter);
	void verifyAndRunScript(const std::string& scriptPath);

private:
	bool isInvalidScript(const std::string& scriptPath);
	bool cannotOpenScript(std::ifstream& script, const std::string& scriptPath);
	void runScript(std::ifstream& script, const std::string& scriptPath);

	TestShell& testShell;
	SSDAdapter& ssdAdapter;
};