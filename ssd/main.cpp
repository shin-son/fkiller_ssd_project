#include "gmock/gmock.h"
#include "ssd_write.h"
#include "error_code.h"

TEST(SSDREADWRITE, WRITEPASS) {
	SsdWrite ssdwrite;
	int address = 0;
	uint32_t value = 0xabc;

	EXPECT_EQ(ssdwrite.write(address, value), SUCCESS);
}

TEST(SSDREADWRITE, WRITEFAILED_INVALIDADDRESS) {
	SsdWrite ssdwrite;
	int address = -1;
	uint32_t value = 0xabc;

	EXPECT_EQ(ssdwrite.write(address, value), INVALIDADDRESS);
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}