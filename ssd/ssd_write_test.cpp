#include "gmock/gmock.h"
#include "ssd_write.h"

using namespace testing;

TEST(SSDREADWRITE, WRITEPASS_1) {
	SsdWrite ssdwrite;

	ssdwrite.loadSsdNandFile();

	int address = 2;
	std::string value = "0xabcabcab";

	EXPECT_NO_THROW(ssdwrite.writeSsdNandData(address, value));
}

TEST(SSDREADWRITE, DISABLED_WRITEPASS_2) {
	SsdWrite ssdwrite;

	ssdwrite.loadSsdNandFile();

	int address = 1;
	std::string value = "0xc0000000";

	EXPECT_NO_THROW(ssdwrite.writeSsdNandData(address, value));
}

TEST(SSDREADWRITE, DISABLED_WRITEPASS_3) {
	SsdWrite ssdwrite;
	int address = 2;
	std::string value = "0xa123bc";

	EXPECT_NO_THROW(ssdwrite.writeSsdNandData(address, value));
}

TEST(SSDREADWRITE, DISABLED_WRITEPASS_4) {
	SsdWrite ssdwrite;
	int address = 3;
	std::string value = "0xaabbccdd";

	EXPECT_NO_THROW(ssdwrite.writeSsdNandData(address, value));
}

TEST(SSDREADWRITE, DISABLED_INVALID_MEMORY_VALUE_1) {
	SsdWrite ssdwrite;
	int address = 4;
	std::string value = "124";

	EXPECT_THROW(ssdwrite.writeSsdNandData(address, value), std::invalid_argument);
}

TEST(SSDREADWRITE, DISABLED_INVALID_ADDRESS_1) {
	SsdWrite ssdwrite;
	int address = -1;
	std::string value = "0xabc";

	EXPECT_THROW(ssdwrite.writeSsdNandData(address, value), std::invalid_argument);
}

TEST(SSDREADWRITE, DISABLED_INVALID_ADDRESS_2) {
	SsdWrite ssdwrite;
	int address = -12;
	std::string value = "0xabc";

	EXPECT_THROW(ssdwrite.writeSsdNandData(address, value), std::invalid_argument);
}

TEST(SSDREADWRITE, DISABLED_INVALID_MEMORY_VALUE_2) {
	SsdWrite ssdwrite;
	int address = 34;
	std::string value = "test";

	EXPECT_THROW(ssdwrite.writeSsdNandData(address, value), std::invalid_argument);
}

TEST(SSDREADWRITE, DISABLED_INVALID_MEMORY_VALUE_3) {
	SsdWrite ssdwrite;
	int address = 34;
	std::string value = "";

	EXPECT_THROW(ssdwrite.writeSsdNandData(address, value), std::invalid_argument);
}