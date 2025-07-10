#ifdef _DEBUG
#include "test_case.h"

TEST_F(TestShellFixture, FullWriteAndReadComparePass) {
    for (int i = 0; i < 20; i++) {
        auto t = testShell.intToHexString(i);
        for (int j = 0; j < LBA_COUNT_FOR_FULL_WRC; j++) {
            EXPECT_CALL(mockSSDAdapter, write(LBA_COUNT_FOR_FULL_WRC * i + j, t)).Times(1)
                .WillOnce(Return(""));
        }
        for (int j = 0; j < LBA_COUNT_FOR_FULL_WRC; j++) {
            EXPECT_CALL(mockSSDAdapter, read(LBA_COUNT_FOR_FULL_WRC * i + j)).Times(1)
                .WillOnce(Return(t));
        }
    }
    testing::internal::CaptureStdout();
    //testShell.fullWriteAndReadCompare();
    testShell.runCommand(TEST_SCRIPT_1_FULL_COMMAND_NAME);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find(DONE), std::string::npos);
}

TEST_F(TestShellFixture, FullWriteAndReadCompareFail) {
    for (int i = 0; i < LOOP_COUNT_FOR_FULL_WRC / 2; i++) {
        auto t = testShell.intToHexString(i);
        for (int j = 0; j < LBA_COUNT_FOR_FULL_WRC; j++) {
            EXPECT_CALL(mockSSDAdapter, write(LBA_COUNT_FOR_FULL_WRC * i + j, t)).Times(1)
                .WillOnce(Return(""));
        }
        for (int j = 0; j < LBA_COUNT_FOR_FULL_WRC; j++) {
            EXPECT_CALL(mockSSDAdapter, read(LBA_COUNT_FOR_FULL_WRC * i + j)).Times(1)
                .WillOnce(Return(t));
        }
    }

    EXPECT_CALL(mockSSDAdapter, write(50, testShell.intToHexString(10))).Times(1)
        .WillOnce(Return(ERROR));

    testing::internal::CaptureStdout();
    //testShell.fullWriteAndReadCompare();
    testShell.runCommand(TEST_SCRIPT_1_FULL_COMMAND_NAME);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find(ERROR), std::string::npos);
}

TEST_F(TestShellFixture, PartialLBAWriteCountWriteTimesWithFullCommnad) {
    string cmdInput = TEST_SCRIPT_2_SHORT_COMMAND_NAME;
    string writeData = INPUT_DATA_FOR_PARTIAL_LBA_WRITE;

    {
        InSequence seq;

        for (int count = 0; count < LOOP_COUNT_FOR_PARTIAL_LBA_WRITE; count++)
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
        .WillRepeatedly(Return(writeData));

    testing::internal::CaptureStdout();
    testShell.runCommand(cmdInput);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find(TEST_SCRIPT_2_SUCCESS_MSG), std::string::npos);
}

TEST_F(TestShellFixture, PartialLBAWriteCountWriteTimesWithShortCommand) {
    string cmdInput = TEST_SCRIPT_2_SHORT_COMMAND_NAME;
    string writeData = INPUT_DATA_FOR_PARTIAL_LBA_WRITE;

    {
        InSequence seq;

        for (int count = 0; count < LOOP_COUNT_FOR_PARTIAL_LBA_WRITE; count++)
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
        .WillRepeatedly(Return(writeData));

    testing::internal::CaptureStdout();
    testShell.runCommand(cmdInput);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output.find(ERROR), std::string::npos);
    EXPECT_NE(output.find(TEST_SCRIPT_2_SUCCESS_MSG), std::string::npos);
}

TEST_F(TestShellFixture, PartialLBAWriteFullCommandPass) {
    string cmdInput = TEST_SCRIPT_2_FULL_COMMAND_NAME;
    string writeData = INPUT_DATA_FOR_PARTIAL_LBA_WRITE;

    {
        InSequence seq;

        for (int count = 0; count < LOOP_COUNT_FOR_PARTIAL_LBA_WRITE; count++)
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
        .WillRepeatedly(Return(writeData));

    testing::internal::CaptureStdout();
    testShell.runCommand(cmdInput);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output.find(ERROR), std::string::npos);
    EXPECT_NE(output.find(TEST_SCRIPT_2_SUCCESS_MSG), std::string::npos);
}

TEST_F(TestShellFixture, PartialLBAWriteShortCommandPass) {
    string cmdInput = TEST_SCRIPT_2_SHORT_COMMAND_NAME;
    string writeData = INPUT_DATA_FOR_PARTIAL_LBA_WRITE;

    {
        InSequence seq;

        for (int count = 0; count < LOOP_COUNT_FOR_PARTIAL_LBA_WRITE; count++)
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

    testing::internal::CaptureStdout();
    testShell.runCommand(cmdInput);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output.find(ERROR), std::string::npos);
    EXPECT_NE(output.find(DONE), std::string::npos);
}

TEST_F(TestShellFixture, PartialLBAWriteWithDataPass) {
    string cmdInput = TEST_SCRIPT_2_FULL_COMMAND_NAME;
    string writeData = "0xDEADDEAD";

    {
        InSequence seq;

        for (int count = 0; count < LOOP_COUNT_FOR_PARTIAL_LBA_WRITE; count++)
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
        .WillRepeatedly(Return(writeData));

    testing::internal::CaptureStdout();
    testShell.partialLBAWrite(writeData);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output.find(ERROR), std::string::npos);
}

TEST_F(TestShellFixture, PartialLBAWriteWriteFail) {
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
        .WillRepeatedly(Return(ERROR));

    testing::internal::CaptureStdout();
    testShell.runCommand(cmdInput);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find(ERROR), std::string::npos);
}

TEST_F(TestShellFixture, PartialLBAWriteVerifyPass) {
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
        .WillRepeatedly(Return(writeData));

    testing::internal::CaptureStdout();
    testShell.runCommand(cmdInput);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output.find(ERROR), std::string::npos);
}

TEST_F(TestShellFixture, PartialLBAWriteVerifyFail) {
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
        .WillOnce(Return(writeData))
        .WillRepeatedly(Return("0x0000"));

    testing::internal::CaptureStdout();
    testShell.runCommand(cmdInput);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find(ERROR), std::string::npos);
}

TEST_F(TestShellFixture, WriteReadAgingPass) {
    std::string testData = "0xA1B2C3D4";

    EXPECT_CALL(mockSSDAdapter, write(0, _))
        .Times(200)
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, write(99, _))
        .Times(200)
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, read(0))
        .Times(200)
        .WillRepeatedly(Return(testData));
    EXPECT_CALL(mockSSDAdapter, read(99))
        .Times(200)
        .WillRepeatedly(Return(testData));

    testing::internal::CaptureStdout();
    testShell.runCommand(TEST_SCRIPT_3_FULL_COMMAND_NAME);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("[WriteReadAging] Done"), std::string::npos);
}

TEST_F(TestShellFixture, WriteReadAgingFail) {
    std::string testData = "0xA1B2C3D4";
    std::string wrongData = "0xA1B2C3DF";

    EXPECT_CALL(mockSSDAdapter, write(0, _))
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, write(99, _))
        .WillRepeatedly(Return(""));
    EXPECT_CALL(mockSSDAdapter, read(0))
        .WillRepeatedly(Return(testData));
    EXPECT_CALL(mockSSDAdapter, read(99))
        .WillOnce(Return(wrongData))
        .WillRepeatedly(Return(testData));

    testing::internal::CaptureStdout();
    testShell.runCommand(TEST_SCRIPT_3_FULL_COMMAND_NAME);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("[WriteReadAging] ERROR mismatch value LBA[0] : 0xA1B2C3D4 LBA[99] : 0xA1B2C3DF"), std::string::npos);
}

TEST_F(TestShellFixture, EraseAndWriteAging_Pass) {

    EXPECT_CALL(mockSSDAdapter, write(_, _))
        .Times(30 * 2 * 49)
        .WillRepeatedly(Return(""));

    EXPECT_CALL(mockSSDAdapter, erase(_, _))
        .Times(1 + 30 * 49)
        .WillRepeatedly(Return(""));


    testing::internal::CaptureStdout();
    testShell.runCommand(TEST_SCRIPT_4_FULL_COMMAND_NAME);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output.find(ERROR), std::string::npos);
}

TEST_F(TestShellFixture, EraseAndWriteAging_WriteFail) {

    EXPECT_CALL(mockSSDAdapter, write(_, _))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillOnce(Return(ERROR))
        .WillRepeatedly(Return(""));

    EXPECT_CALL(mockSSDAdapter, erase(_, _))
        .WillRepeatedly(Return(""));

    testing::internal::CaptureStdout();
    testShell.runCommand(TEST_SCRIPT_4_FULL_COMMAND_NAME);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find(ERROR), std::string::npos);
}

TEST_F(TestShellFixture, EraseAndWriteAging_EraseFail) {

    EXPECT_CALL(mockSSDAdapter, write(_, _))
        .WillRepeatedly(Return(""));

    EXPECT_CALL(mockSSDAdapter, erase(_, _))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillRepeatedly(Return(ERROR));

    testing::internal::CaptureStdout();
    testShell.runCommand(TEST_SCRIPT_4_FULL_COMMAND_NAME);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find(ERROR), std::string::npos);
}
#endif