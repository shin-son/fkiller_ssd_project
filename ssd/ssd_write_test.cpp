#include "gmock/gmock.h"
#include "ssd_write.h"

using namespace testing;

TEST(SSDREADWRITE, WRITEPASS) {
	SsdWrite ssdwrite;
	std::string address = "0";
	std::string value = "0xabc";

	EXPECT_NO_THROW(ssdwrite.write(address, value));
}

TEST(SSDREADWRITE, WRITEFAILED_INVALID_ADDRESS) {
	SsdWrite ssdwrite;
	std::string address = "-1";
	std::string value = "0xabc";

	EXPECT_THROW(ssdwrite.write(address, value), std::out_of_range);
}

TEST(SSDREADWRITE, WRITEFAILED_INVALID_ARGUMENT_ADDRESS) {
	SsdWrite ssdwrite;
	std::string address = "- 1 2";
	std::string value = "0xabc";

	EXPECT_THROW(ssdwrite.write(address, value), std::invalid_argument);
}

TEST(SSDREADWRITE, INVALID_MEMORY_VALUE) {
	SsdWrite ssdwrite;
	std::string address = "34";
	std::string value = "test";

	EXPECT_THROW(ssdwrite.write(address, value), std::invalid_argument);
}