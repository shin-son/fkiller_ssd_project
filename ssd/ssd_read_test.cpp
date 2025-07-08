#include "gmock/gmock.h"
#include "ssd_Read.h"
#include <stdexcept>

using namespace testing;
using std::string;

TEST(SsdReadTest, ReadFileStream) {
	
	Ssd_Read ssdRead("ssd_nand.txt");

	EXPECT_TRUE(ssdRead.readSsdNandFile());
}

TEST(SsdReadTest, WrongReadFileStream) {
	
	Ssd_Read ssdRead("hdd_read_test.cpp");

	EXPECT_FALSE(ssdRead.readSsdNandFile());
}

TEST(SsdReadTest, WriteFileStream) {
	
	Ssd_Read ssdRead("ssd_nand.txt", "ssd_output.txt");

	EXPECT_TRUE(ssdRead.writeSsdNandDataToFile("test"));
}

TEST(SsdReadTest, AllDataRead) {
	
	Ssd_Read ssdRead;

	int expectedSize = 100;

	EXPECT_TRUE(ssdRead.readSsdNandFile());
	EXPECT_EQ(expectedSize, ssdRead.getSsdNandDataSize());
}

TEST(SsdReadTest, SpecificReadData) {

	Ssd_Read ssdRead;

	string expectedString = "0x00000000";

	EXPECT_TRUE(ssdRead.readSsdNandFile());
	EXPECT_EQ(expectedString, ssdRead.getSsdNandDataAt(2));
}

TEST(SsdReadTest, OutOfRangeReadData) {

	Ssd_Read ssdRead;

	EXPECT_TRUE(ssdRead.readSsdNandFile());
	EXPECT_THROW(ssdRead.getSsdNandDataAt(100), std::out_of_range);
}

TEST(SsdReadTest, ReadDataWriteToOutputFile) {
	
	Ssd_Read ssdRead;

	EXPECT_TRUE(ssdRead.readSsdNandFile());
	
	string expectedString = ssdRead.getSsdNandDataAt(2);

	EXPECT_TRUE(ssdRead.isSsdOutputFileCorrect(expectedString));
}

TEST(SsdReadTest, WrongReadDataWriteToOutputFile) {
	
	Ssd_Read ssdRead;

	EXPECT_TRUE(ssdRead.readSsdNandFile());
	
	string expectedString = "ERROR";

	EXPECT_THROW(ssdRead.getSsdNandDataAt(100), std::out_of_range);
	EXPECT_TRUE(ssdRead.isSsdOutputFileCorrect(expectedString));
}

