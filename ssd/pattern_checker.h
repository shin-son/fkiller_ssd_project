#pragma once
#include <string>

class PatternChecker {
public:
	bool isValidMemoryValue(const std::string& value);
	bool isValidAddress(const std::string& address);
};