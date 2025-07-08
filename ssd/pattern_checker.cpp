#include "pattern_checker.h"
#include <regex>

bool PatternChecker::isValidAddress(const std::string& address) {
	static const std::regex numericPattern("^[0-9]+$");
	if (std::regex_match(address, numericPattern) == false) return false;
	int addressToInt = std::stoi(address);

	if (addressToInt < 0 || addressToInt > 99) return false;

	return true;
}

bool PatternChecker::isValidMemoryValue(const std::string& value) {
	static const std::regex hexPattern("^0x[0-9A-Fa-f]{1,8}$");
	return std::regex_match(value, hexPattern);
}