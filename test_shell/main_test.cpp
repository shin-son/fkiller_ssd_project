#ifdef _DEBUG
#include <cstdlib>
#include "mock_ssd_adapter.h"
#include "test_shell.h"

using namespace testing;

TEST(SampleTest, Addition) {
    EXPECT_EQ(2 + 2, 4);
}

TEST(TS, ReadPass) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell;
    testShell.setSsdAdapter(&mockSSDAdapter);

    EXPECT_CALL(mockSSDAdapter, read(10))
        .Times(1)
        .WillRepeatedly(Return("0xAAAABBBB"));

    string command = "read 10";
    EXPECT_EQ(3, testShell.runCommand(command));
}
#endif