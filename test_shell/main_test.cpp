#ifdef _DEBUG
#include <cstdlib>
#include "constants.h"
#include "mock_ssd_adapter.h"
#include "test_shell.h""

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

TEST(TestShellTest, partialLBAWrite_CountWriteTimesWithFullCommnad) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell(&mockSSDAdapter);
    string cmdInput = "2_";
    string writeData = INPUT_DATA_FOR_PARTIAL_LBA_WRITE;

    {
        InSequence seq;

        for (int count = 0; count < LOOP_COUT_FOR_PARTIAL_LBA_WRITE; count++)
        {
            EXPECT_CALL(mockSSDAdapter, write(4, writeData))
                .Times(1);
            EXPECT_CALL(mockSSDAdapter, write(0, writeData))
                .Times(1);
            EXPECT_CALL(mockSSDAdapter, write(3, writeData))
                .Times(1);
            EXPECT_CALL(mockSSDAdapter, write(1, writeData))
                .Times(1);
            EXPECT_CALL(mockSSDAdapter, write(2, writeData))
                .Times(1);
        }
    }

    testShell.runCommand(cmdInput);
}

TEST(TestShellTest, partialLBAWrite_CountWriteTimesWithShortCommand) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell(&mockSSDAdapter);
    string cmdInput = "2_";
    string writeData = INPUT_DATA_FOR_PARTIAL_LBA_WRITE;

    {
        InSequence seq;

        for (int count = 0; count < LOOP_COUT_FOR_PARTIAL_LBA_WRITE; count++)
        {
            EXPECT_CALL(mockSSDAdapter, write(4, writeData))
                .Times(1);
            EXPECT_CALL(mockSSDAdapter, write(0, writeData))
                .Times(1);
            EXPECT_CALL(mockSSDAdapter, write(3, writeData))
                .Times(1);
            EXPECT_CALL(mockSSDAdapter, write(1, writeData))
                .Times(1);
            EXPECT_CALL(mockSSDAdapter, write(2, writeData))
                .Times(1);
        }
    }

    testShell.runCommand(cmdInput);
}
#endif