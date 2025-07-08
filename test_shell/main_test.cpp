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

TEST(TestShellTest, Write_Pass) {
    MockSSDAdapter mockSSD;
    TestShell testShell;
    testShell.setSsdAdapter(&mockSSD);

    EXPECT_CALL(mockSSD, write(5, "0xAAAABBBB"))
        .WillOnce(Return(""));

    std::string result = testShell.write(5, "0xAAAABBBB");
    EXPECT_EQ(result, "[Write] Done");
}

TEST(TestShellTest, Write_Fail) {
    MockSSDAdapter mockSSD;
    TestShell shell;
    shell.setSsdAdapter(&mockSSD);

    EXPECT_CALL(mockSSD, write(5, "0xAAAAABBBBB"))
        .WillOnce(Return("ERROR"));

    std::string result = shell.write(5, "0xAAAAABBBBB");
    EXPECT_EQ(result, "[Write] ERROR");
}

TEST(TestShellTest, FullWrite_Pass) {
    MockSSDAdapter mockSSD;
    TestShell shell;
    shell.setSsdAdapter(&mockSSD);

    for (int i = 0; i < 100; ++i) {
        EXPECT_CALL(mockSSD, write(i, "0xAAAABBBB"))
        .Times(1)
        .WillRepeatedly(Return(""));
    }

    testing::internal::CaptureStdout();
    shell.fullWrite("0xAAAABBBB");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("[fullWrite] Done"), std::string::npos);
}

TEST(TestShellTest, FullWrite_Fail) {
    MockSSDAdapter mockSSD;
    TestShell shell;
    shell.setSsdAdapter(&mockSSD);

    EXPECT_CALL(mockSSD, write(_, _))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillOnce(Return("[Write] ERROR"));

    testing::internal::CaptureStdout();
    shell.fullWrite("0xAAAABBBB");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("[fullWrite] Failed at LBA 3"), std::string::npos);
}

TEST(TestShellTest, partialLBAWrite_CountWriteTimesWithFullCommnad) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell(&mockSSDAdapter);
    string cmdInput = TEST_SCRIPT_2_FULL_COMMAND_NAME;
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

    EXPECT_CALL(mockSSDAdapter, read(_))
        .WillRepeatedly(Return(string(INPUT_DATA_FOR_PARTIAL_LBA_WRITE)));

    testShell.runCommand(cmdInput);
}

TEST(TestShellTest, partialLBAWrite_CountWriteTimesWithShortCommand) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell(&mockSSDAdapter);
    string cmdInput = TEST_SCRIPT_2_SHORT_COMMAND_NAME;
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

    EXPECT_CALL(mockSSDAdapter, read(_))
        .WillRepeatedly(Return(string(INPUT_DATA_FOR_PARTIAL_LBA_WRITE)));

    testShell.runCommand(cmdInput);
}

TEST(TestShellTest, partialLBAWrite_WriteFail) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell(&mockSSDAdapter);
    string cmdInput = TEST_SCRIPT_2_FULL_COMMAND_NAME;
    string writeData = INPUT_DATA_FOR_PARTIAL_LBA_WRITE;

    EXPECT_CALL(mockSSDAdapter, write(4, writeData))
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, write(0, writeData))
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, write(3, writeData))
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, write(1, writeData))
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, write(2, writeData))
        .WillRepeatedly(Return("Error"));

    EXPECT_EQ(testShell.runCommand(cmdInput), 2);
}

TEST(TestShellTest, partialLBAWrite_VerifySuccess) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell(&mockSSDAdapter);
    string cmdInput = TEST_SCRIPT_2_FULL_COMMAND_NAME;
    string writeData = INPUT_DATA_FOR_PARTIAL_LBA_WRITE;

    EXPECT_CALL(mockSSDAdapter, write(4, writeData))
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, write(0, writeData))
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, write(3, writeData))
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, write(1, writeData))
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, write(2, writeData))
        .WillRepeatedly(Return(""));

    EXPECT_CALL(mockSSDAdapter, read(_))
        .WillRepeatedly(Return(string(INPUT_DATA_FOR_PARTIAL_LBA_WRITE)));

    EXPECT_EQ(testShell.runCommand(cmdInput), 3);
}

TEST(TestShellTest, partialLBAWrite_VerifyFail) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell(&mockSSDAdapter);
    string cmdInput = TEST_SCRIPT_2_FULL_COMMAND_NAME;
    string writeData = INPUT_DATA_FOR_PARTIAL_LBA_WRITE;

    EXPECT_CALL(mockSSDAdapter, write(4, writeData))
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, write(0, writeData))
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, write(3, writeData))
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, write(1, writeData))
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, write(2, writeData))
        .WillRepeatedly(Return(""));

    EXPECT_CALL(mockSSDAdapter, read(_))
        .WillOnce(Return(string(INPUT_DATA_FOR_PARTIAL_LBA_WRITE)))
        .WillOnce(Return(string("0x0000")))
        .WillRepeatedly(Return(string(INPUT_DATA_FOR_PARTIAL_LBA_WRITE)));

    EXPECT_EQ(testShell.runCommand(cmdInput), 2);
}

#endif