#include "gmock/gmock.h"
#include "ssd_read.h"
#include <stdexcept>

using namespace testing;
using std::string;

class SsdReadTestFixture : public Test {
protected:
	SsdRead ssdRead{ "..\\ssd_nand.txt", "..\\ssd_output.txt" };

	void SetUp() override {
		ssdRead.loadSsdNandFile();
	}
};

TEST_F(SsdReadTestFixture, ReadFileStream) {

	EXPECT_TRUE(ssdRead.loadSsdNandFile());
}

TEST_F(SsdReadTestFixture, WrongReadFileStream) {

	SsdRead ssdReadWrong("hdd_read_test.cpp");

	EXPECT_FALSE(ssdReadWrong.loadSsdNandFile());
}

TEST_F(SsdReadTestFixture, WriteFileStream) {

	EXPECT_TRUE(ssdRead.writeSsdNandDataToFile("test"));
}

TEST_F(SsdReadTestFixture, AllDataRead) {

	int expectedSize = 100;

	EXPECT_EQ(expectedSize, ssdRead.getSsdNandDataSize());
}

TEST_F(SsdReadTestFixture, SpecificReadData) {

	string expectedString = "0x00000000";

	EXPECT_EQ(expectedString, ssdRead.getSsdNandDataAt(2));
}

TEST_F(SsdReadTestFixture, DISABLED_OutOfRangeReadData) {

	EXPECT_THROW(ssdRead.getSsdNandDataAt(100), std::out_of_range);
}

TEST_F(SsdReadTestFixture, ReadDataWriteToOutputFile) {

	string expectedString = ssdRead.getSsdNandDataAt(2);
	ssdRead.writeSsdNandDataToFile(expectedString);

	EXPECT_TRUE(ssdRead.isSsdOutputFileCorrect(expectedString));
}

TEST_F(SsdReadTestFixture, DISABLED_WrongReadDataWriteToOutputFile) {

	string expectedString = "ERROR";

	EXPECT_THROW(ssdRead.getSsdNandDataAt(100), std::out_of_range);
	EXPECT_TRUE(ssdRead.isSsdOutputFileCorrect(expectedString));
}

