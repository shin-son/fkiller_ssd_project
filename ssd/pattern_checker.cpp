#include "pattern_checker.h"
#include <regex>

bool PatternChecker::isValidAddress(const std::string& address) {
	static const std::regex numericPattern("^[0-9]+$");
	if (std::regex_match(address, numericPattern) == false) return false;
	int addressToInt = std::stoi(address);

	if (addressToInt < 0 || addressToInt > 99) return false;

	return true;
}

bool PatternChecker::isValidSize(const std::string& value) {
	static const std::regex numericPattern("^[0-9]+$");
	if (std::regex_match(value, numericPattern) == false) return false;
	int size = std::stoi(value);

	if (size < 1 || size > 10) return false;

	return true;
}

bool PatternChecker::isValidMemoryValue(const std::string& value) {
	static const std::regex hexPattern("^0x[0-9A-Fa-f]{1,8}$");
	return std::regex_match(value, hexPattern);
}

bool PatternChecker::isOutofRange(const std::string& address, const std::string& value) {
	int addressInt = std::stoi(address);
	int size = std::stoi(value);

	if (addressInt + size > 99) return false;

	return true;
}