#include "gmock/gmock.h"
#include "ssd_write.h"

using namespace testing;

TEST(SSDREADWRITE, WRITEPASS_1) {
	SsdWrite ssdwrite;
	std::string address = "0";
	std::string value = "0xabc";

	EXPECT_NO_THROW(ssdwrite.write(address, value));
}

TEST(SSDREADWRITE, WRITEPASS_2) {
	SsdWrite ssdwrite;
	std::string address = "1";
	std::string value = "0xc";

	EXPECT_NO_THROW(ssdwrite.write(address, value));
}

TEST(SSDREADWRITE, WRITEPASS_3) {
	SsdWrite ssdwrite;
	std::string address = "2";
	std::string value = "0xa123bc";

	EXPECT_NO_THROW(ssdwrite.write(address, value));
}

TEST(SSDREADWRITE, WRITEPASS_4) {
	SsdWrite ssdwrite;
	std::string address = "3";
	std::string value = "0xaabbccdd";

	EXPECT_NO_THROW(ssdwrite.write(address, value));
}

TEST(SSDREADWRITE, INVALID_MEMORY_VALUE_1) {
	SsdWrite ssdwrite;
	std::string address = "34";
	std::string value = "124";

	EXPECT_THROW(ssdwrite.write(address, value), std::invalid_argument);
}

TEST(SSDREADWRITE, INVALID_ADDRESS_1) {
	SsdWrite ssdwrite;
	std::string address = "-1";
	std::string value = "0xabc";

	EXPECT_THROW(ssdwrite.write(address, value), std::invalid_argument);
}

TEST(SSDREADWRITE, INVALID_ADDRESS_2) {
	SsdWrite ssdwrite;
	std::string address = "- 1 2";
	std::string value = "0xabc";

	EXPECT_THROW(ssdwrite.write(address, value), std::invalid_argument);
}

TEST(SSDREADWRITE, INVALID_MEMORY_VALUE_2) {
	SsdWrite ssdwrite;
	std::string address = "34";
	std::string value = "test";

	EXPECT_THROW(ssdwrite.write(address, value), std::invalid_argument);
}

TEST(SSDREADWRITE, INVALID_MEMORY_VALUE_3) {
	SsdWrite ssdwrite;
	std::string address = "34";
	std::string value = "";

	EXPECT_THROW(ssdwrite.write(address, value), std::invalid_argument);
}