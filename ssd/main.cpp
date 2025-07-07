#include "gmock/gmock.h"
#include "ssd_write.h"

TEST(SSDREADWRITE, WRITEPASS) {
	SsdWrite ssdwrite;
	int address = 0;
	uint32_t value = 0xabc;

	ssdwrite.write(address, value);
}

TEST(SSDREADWRITE, WRITEFAILED_INVALIDADDRESS) {
	SsdWrite ssdwrite;
	int address = -1;
	uint32_t value = 0xabc;

	ssdwrite.write(address, value);
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}