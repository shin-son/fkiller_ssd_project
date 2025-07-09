#ifdef _DEBUG
#include <cstdlib>
#include "constants.h"
#include "mock_ssd_adapter.h"
#include "test_shell.h"

using namespace testing;

class TestShellFixture : public Test {
protected:
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell;
    unsigned int SSD_SIZE = 100;

    void SetUp() override {
        testShell.setSsdAdapter(&mockSSDAdapter);
    }
};

TEST_F(TestShellFixture, ExitCMD) {
    string cmdInput = EXIT_COMMAND_NAME;
    testing::internal::CaptureStdout();
    testShell.runCommand(cmdInput);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("PROGRAM EXIT"), std::string::npos);
}

TEST_F(TestShellFixture, InvalidCMD) {
    string cmdInput = TEST_SCRIPT_2_FULL_COMMAND_NAME;
    testing::internal::CaptureStdout();
    testShell.runCommand(cmdInput);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output.find(INVALID_COMMAND_MSG), std::string::npos);

    cmdInput = "XXXX";
    testing::internal::CaptureStdout();
    testShell.runCommand(cmdInput);
    output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find(INVALID_COMMAND_MSG), std::string::npos);
}

TEST_F(TestShellFixture, ReadPass) {
    EXPECT_CALL(mockSSDAdapter, read(10))
        .Times(1)
        .WillRepeatedly(Return("0xAAAABBBB"));

    EXPECT_EQ("[Read] LBA 10 : 0xAAAABBBB", testShell.read(10));
}

TEST_F(TestShellFixture, ReadFailWrongLBA) {
    EXPECT_CALL(mockSSDAdapter, read(100))
        .Times(1)
        .WillOnce(Return("ERROR"));

    EXPECT_EQ("[Read] ERROR", testShell.read(100));
}

TEST_F(TestShellFixture, FullReadPass) {
    internal::CaptureStdout();
    EXPECT_CALL(mockSSDAdapter, read(_))
        .Times(100)
        .WillRepeatedly(Return("0x00ABCDEF"));

    string expected;
    for (int LBA = 0; LBA < 100; LBA++) {
        expected += "[Read] LBA " + std::to_string(LBA) + " : " + "0x00ABCDEF\n";
    }

    testShell.fullRead();
    string output = internal::GetCapturedStdout();

    EXPECT_EQ(expected, output);
}

TEST_F(TestShellFixture, FullReadFail) {
    internal::CaptureStdout();
    EXPECT_CALL(mockSSDAdapter, read(_))
        .Times(100)
        .WillOnce(Return("0x00ABCDEF"))
        .WillRepeatedly(Return("ERROR"));

    string expected = "[Read] LBA 0 : 0x00ABCDEF\n";
    for (int LBA = 1; LBA < 100; LBA++) {
        expected += "[Read] ERROR\n";
    }

    testShell.fullRead();
    string output = internal::GetCapturedStdout();

    EXPECT_EQ(expected, output);
}

TEST_F(TestShellFixture, WritePass) {
    EXPECT_CALL(mockSSDAdapter, write(5, "0xAAAABBBB"))
        .WillOnce(Return(""));

    std::string result = testShell.write(5, "0xAAAABBBB");
    EXPECT_EQ(result, "[Write] Done");
}

TEST_F(TestShellFixture, WriteFail) {
    EXPECT_CALL(mockSSDAdapter, write(5, "0xAAAAABBBBB"))
        .WillOnce(Return("ERROR"));

    std::string result = testShell.write(5, "0xAAAAABBBBB");
    EXPECT_EQ(result, "[Write] ERROR");
}

TEST_F(TestShellFixture, FullWritePass) {
    internal::CaptureStdout();
    for (int i = 0; i < 100; ++i) {
        EXPECT_CALL(mockSSDAdapter, write(i, "0xAAAABBBB"))
            .Times(1)
            .WillRepeatedly(Return(""));
    }

    testShell.fullWrite("0xAAAABBBB");
    std::string output = internal::GetCapturedStdout();

    EXPECT_NE(output.find("[fullWrite] Done"), std::string::npos);
}

TEST_F(TestShellFixture, FullWriteFail) {
    internal::CaptureStdout();
    EXPECT_CALL(mockSSDAdapter, write(_, _))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillOnce(Return("[Write] ERROR"));

    testShell.fullWrite("0xAAAABBBB");
    std::string output = internal::GetCapturedStdout();

    EXPECT_NE(output.find("[fullWrite] Failed at LBA 3"), std::string::npos);
}

TEST_F(TestShellFixture, FullWriteAndReadComparePass) {
    for (int i = 0; i < 20; i++) {
        auto t = testShell.intToHexString(i);
        for (int j = 0; j < 5; j++) {
            EXPECT_CALL(mockSSDAdapter, write(5 * i + j, t)).Times(1)
                .WillOnce(Return(""));
        }
        for (int j = 0; j < 5; j++) {
            EXPECT_CALL(mockSSDAdapter, read(5 * i + j)).Times(1)
                .WillOnce(Return(t));
        }
    }
    testing::internal::CaptureStdout();
    testShell.fullWriteAndReadCompare();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("PASS"), std::string::npos);
}

TEST_F(TestShellFixture, FullWriteAndReadCompareFail) {
    for (int i = 0; i < 10; i++) {
        auto t = testShell.intToHexString(i);
        for (int j = 0; j < 5; j++) {
            EXPECT_CALL(mockSSDAdapter, write(5 * i + j, t)).Times(1)
                .WillOnce(Return(""));
        }
        for (int j = 0; j < 5; j++) {
            EXPECT_CALL(mockSSDAdapter, read(5 * i + j)).Times(1)
                .WillOnce(Return(t));
        }
    }
    EXPECT_CALL(mockSSDAdapter, write(50, testShell.intToHexString(10))).Times(1)
        .WillOnce(Return("ERROR"));

    testing::internal::CaptureStdout();
    testShell.fullWriteAndReadCompare();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("FAIL"), std::string::npos);
}

TEST_F(TestShellFixture, PartialLBAWriteCountWriteTimesWithFullCommnad) {
    string cmdInput = "2_";
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

    testShell.runCommand(cmdInput);
}

TEST_F(TestShellFixture, PartialLBAWriteCountWriteTimesWithShortCommand) {
    string cmdInput = "2_";
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

    testShell.runCommand(cmdInput);
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

    EXPECT_EQ(output.find("Fail"), std::string::npos);
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

    EXPECT_EQ(output.find("Fail"), std::string::npos);
}

TEST_F(TestShellFixture, PartialLBAWriteWithDataPass) {
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

    EXPECT_EQ(output.find("Fail"), std::string::npos);
}

TEST_F(TestShellFixture, PartialLBAWriteWriteFail) {
    string writeData = "0xDEAFDEAF";

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
        //.WillRepeatedly(Return(""));

    testing::internal::CaptureStdout();
    testShell.partialLBAWrite(writeData);
    std::string output = testing::internal::GetCapturedStdout();

    //EXPECT_EQ(string(TEST_SCRIPT_2_WRITE_FAIL_MSG), output);
    EXPECT_NE(output.find("Fail"), std::string::npos);
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

    EXPECT_EQ(output.find("Fail"), std::string::npos);
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
        .WillOnce(Return(string(writeData)))
        .WillOnce(Return(string("0x0000")))
        .WillRepeatedly(Return(string(writeData)));

    testing::internal::CaptureStdout();
    testShell.runCommand(cmdInput);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Fail"), std::string::npos);
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
    testShell.writeReadAging();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("[Aging] PASS"), std::string::npos);
}

TEST_F(TestShellFixture, WriteReadAgingFail) {
    std::string testData = "0xA1B2C3D4";
    std::string wrongData = "0xA1B2C3DF";

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
        .WillOnce(Return(wrongData))
        .WillRepeatedly(Return(testData));

    testing::internal::CaptureStdout();
    testShell.writeReadAging();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("[Aging] ERROR mismatch value LBA[0] : 0xA1B2C3D4 LBA[99] : 0xA1B2C3DF"), std::string::npos);
}

#endif