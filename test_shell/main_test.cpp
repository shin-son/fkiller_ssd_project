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

    EXPECT_CALL(mockSSDAdapter, read(10))
        .WillRepeatedly(Return("0xAAAABBBB"));
}

TEST(SampleTest, partialLBAWrite_CountWriteTimesWithFullCommnad) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell(&mockSSDAdapter);
    string cmdInput = "2_";
    string writeData = INPUT_DATA_FOR_PARTIAL_LBA_WRITE;

    EXPECT_CALL(mockSSDAdapter, write(_, writeData))
        .Times(150);

    testShell.runCommand(cmdInput);
}

TEST(SampleTest, partialLBAWrite_CountWriteTimesWittShortCommand) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell(&mockSSDAdapter);
    string cmdInput = "2_";
    string writeData = INPUT_DATA_FOR_PARTIAL_LBA_WRITE;

    EXPECT_CALL(mockSSDAdapter, write(_, writeData))
        .Times(150);

    testShell.runCommand(cmdInput);
}
#endif