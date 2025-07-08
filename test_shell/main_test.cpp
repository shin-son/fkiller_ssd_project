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

TEST(TS, ReadFailNoLBA) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell;
    testShell.setSsdAdapter(&mockSSDAdapter);

    EXPECT_CALL(mockSSDAdapter, read(10))
        .Times(0);

    string command = "read";
    EXPECT_EQ(1, testShell.runCommand(command));
}

TEST(TS, ReadFailWrongLBA) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell;
    testShell.setSsdAdapter(&mockSSDAdapter);

    EXPECT_CALL(mockSSDAdapter, read(100))
        .Times(1)
        .WillOnce(Return("ERROR"));

    string command = "read 100";
    EXPECT_EQ(1, testShell.runCommand(command));
}

TEST(TS, FullReadPASS) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell;
    testShell.setSsdAdapter(&mockSSDAdapter);

    EXPECT_CALL(mockSSDAdapter, read(_))
        .Times(100)
        .WillRepeatedly(Return("0x00ABCDEF"));

    string command = "fullread";
    EXPECT_EQ(3, testShell.runCommand(command));
}

TEST(TS, FullReadFAIL) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell;
    testShell.setSsdAdapter(&mockSSDAdapter);

    EXPECT_CALL(mockSSDAdapter, read(_))
        .Times(100)
        .WillOnce(Return("0x00ABCDEF"))
        .WillRepeatedly(Return("ERROR"));

    string command = "fullread";
    EXPECT_EQ(1, testShell.runCommand(command));
}
#endif