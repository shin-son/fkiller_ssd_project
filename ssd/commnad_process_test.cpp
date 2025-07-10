#include "gmock/gmock.h"
#include "command_process.h"
#include "ssd_constants.h"

using namespace testing;

TEST(PATTERNCHECKER, WRITE_VALID_ARG_1) {
	const char* argv[] = { "ssd.exe", "w", "10", "0xDEADBEEF" };
	int argc = sizeof(argv) / sizeof(argv[0]);
	
	CommandProcessor* cp = CommandProcessor::Builder()
		.setParam(argc, const_cast<char**>(argv))
		.setOperator()
		.setAddress()
		.setData()
		.patternCheck();

	int result = cp->getResult();

	EXPECT_EQ(result, SUCCESS);
}

TEST(PATTERNCHECKER, WRITE_VALID_ARG_2) {
	const char* argv[] = { "ssd.exe", "w", "12", "0xABCE" };
	int argc = sizeof(argv) / sizeof(argv[0]);

	CommandProcessor* cp = CommandProcessor::Builder()
		.setParam(argc, const_cast<char**>(argv))
		.setOperator()
		.setAddress()
		.setData()
		.patternCheck();

	int result = cp->getResult();

	EXPECT_EQ(result, SUCCESS);
}

TEST(PATTERNCHECKER, INVALID_READ_ADDRESS_1) {
	const char* argv[] = { "ssd.exe", "r", "-10"};
	int argc = sizeof(argv) / sizeof(argv[0]);
	
	CommandProcessor* cp = CommandProcessor::Builder()
		.setParam(argc, const_cast<char**>(argv))
		.setOperator()
		.setAddress()
		.setData()
		.patternCheck();

	int result = cp->getResult();

	EXPECT_EQ(result, INVALID_ARGUMENT);
}

TEST(PATTERNCHECKER, INVALID_READ_ADDRESS_2) {

	const char* argv[] = { "ssd.exe", "r", "10000" };
	int argc = sizeof(argv) / sizeof(argv[0]);
	
	CommandProcessor* cp = CommandProcessor::Builder()
		.setParam(argc, const_cast<char**>(argv))
		.setOperator()
		.setAddress()
		.setData()
		.patternCheck();

	int result = cp->getResult();

	EXPECT_EQ(result, INVALID_ARGUMENT);
}

TEST(PATTERNCHECKER, INVALID_WRITE_ARGUMENT_1) {
	
	const char* argv[] = { "ssd.exe", "w", "10" };
	int argc = sizeof(argv) / sizeof(argv[0]);
	
	CommandProcessor* cp = CommandProcessor::Builder()
		.setParam(argc, const_cast<char**>(argv))
		.setOperator()
		.setAddress()
		.setData()
		.patternCheck();

	int result = cp->getResult();

	EXPECT_EQ(result, INVALID_ARGUMENT);
}

TEST(PATTERNCHECKER, INVALID_WRITE_ARGUMENT_2) {
	
	const char* argv[] = { "ssd.exe", "w", "0xa10" };
	int argc = sizeof(argv) / sizeof(argv[0]);
	
	CommandProcessor* cp = CommandProcessor::Builder()
		.setParam(argc, const_cast<char**>(argv))
		.setOperator()
		.setAddress()
		.setData()
		.patternCheck();

	int result = cp->getResult();

	EXPECT_EQ(result, INVALID_ARGUMENT);
}

TEST(PATTERNCHECKER, INVALID_WRITE_ADDRESS_1) {
	
	const char* argv[] = { "ssd.exe", "w", "-1", "0x1234"};
	int argc = sizeof(argv) / sizeof(argv[0]);
	
	CommandProcessor* cp = CommandProcessor::Builder()
		.setParam(argc, const_cast<char**>(argv))
		.setOperator()
		.setAddress()
		.setData()
		.patternCheck();

	int result = cp->getResult();

	EXPECT_EQ(result, INVALID_ARGUMENT);
}

TEST(PATTERNCHECKER, INVALID_WRITE_ADDRESS_2) {
	
	const char* argv[] = { "ssd.exe", "w", "100", "0x1234" };
	int argc = sizeof(argv) / sizeof(argv[0]);
	
	CommandProcessor* cp = CommandProcessor::Builder()
		.setParam(argc, const_cast<char**>(argv))
		.setOperator()
		.setAddress()
		.setData()
		.patternCheck();

	int result = cp->getResult();

	EXPECT_EQ(result, INVALID_ARGUMENT);
}

TEST(PATTERNCHECKER, INVALID_WRITE_MEMORYVALUE_1) {
	
	const char* argv[] = { "ssd.exe", "w", "10", "0x1234eeeee" };
	int argc = sizeof(argv) / sizeof(argv[0]);
	
	CommandProcessor* cp = CommandProcessor::Builder()
		.setParam(argc, const_cast<char**>(argv))
		.setOperator()
		.setAddress()
		.setData()
		.patternCheck();

	int result = cp->getResult();

	EXPECT_EQ(result, INVALID_ARGUMENT);
}

TEST(PATTERNCHECKER, INVALID_WRITE_MEMORYVALUE_2) {
	
	const char* argv[] = { "ssd.exe", "w", "10", "helloworld" };
	int argc = sizeof(argv) / sizeof(argv[0]);
	
	CommandProcessor* cp = CommandProcessor::Builder()
		.setParam(argc, const_cast<char**>(argv))
		.setOperator()
		.setAddress()
		.setData()
		.patternCheck();

	int result = cp->getResult();

	EXPECT_EQ(result, INVALID_ARGUMENT);
}