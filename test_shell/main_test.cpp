#ifdef _DEBUG
#include <cstdlib>
#include "constants.h"
#include "mock_ssd_adapter.h"
#include "test_shell.h"

using namespace testing;

TEST(SampleTest, Addition) {
    EXPECT_EQ(2 + 2, 4);
}

TEST(TestShellTest, ReadPass) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell;
    testShell.setSsdAdapter(&mockSSDAdapter);

    EXPECT_CALL(mockSSDAdapter, read(10))
        .Times(1)
        .WillRepeatedly(Return("0xAAAABBBB"));

    EXPECT_EQ("[Read] LBA 10 : 0xAAAABBBB", testShell.read(10));
}

TEST(TestShellTest, ReadFailWrongLBA) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell;
    testShell.setSsdAdapter(&mockSSDAdapter);

    EXPECT_CALL(mockSSDAdapter, read(100))
        .Times(1)
        .WillOnce(Return("ERROR"));

    EXPECT_EQ("[Read] ERROR", testShell.read(100));
}

TEST(TestShellTest, FullReadPass) {
    testing::internal::CaptureStdout();
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell;
    testShell.setSsdAdapter(&mockSSDAdapter);

    EXPECT_CALL(mockSSDAdapter, read(_))
        .Times(100)
        .WillRepeatedly(Return("0x00ABCDEF"));

    string expected;
    for (int LBA = 0; LBA < 100; LBA++) {
        expected += "[Read] LBA " + std::to_string(LBA) + " : " + "0x00ABCDEF\n";
    }

    testShell.fullRead();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(expected, output);
}

TEST(TestShellTest, FullReadFail) {
    testing::internal::CaptureStdout();
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell;
    testShell.setSsdAdapter(&mockSSDAdapter);

    EXPECT_CALL(mockSSDAdapter, read(_))
        .Times(100)
        .WillOnce(Return("0x00ABCDEF"))
        .WillRepeatedly(Return("ERROR"));

    string expected = "[Read] LBA 0 : 0x00ABCDEF\n";
    for (int LBA = 1; LBA < 100; LBA++) {
        expected += "[Read] ERROR\n";
    }

    testShell.fullRead();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(expected, output);
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

TEST(TestShellTest, PartialLBAWrite_FullCommand_Pass) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell(&mockSSDAdapter);
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

TEST(TestShellTest, PartialLBAWrite_ShortCommand_Pass) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell(&mockSSDAdapter);
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

TEST(TestShellTest, PartialLBAWrite_WithData_Pass) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell(&mockSSDAdapter);
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

TEST(TestShellTest, PartialLBAWrite_Write_Fail) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell(&mockSSDAdapter);
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

TEST(TestShellTest, PartialLBAWrite_VerifyPass) {
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
        .WillRepeatedly(Return(writeData));

    testing::internal::CaptureStdout();
    testShell.runCommand(cmdInput);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output.find("Fail"), std::string::npos);
}

TEST(TestShellTest, PartialLBAWrite_VerifyFail) {
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
        .WillOnce(Return(string(writeData)))
        .WillOnce(Return(string("0x0000")))
        .WillRepeatedly(Return(string(writeData)));

    testing::internal::CaptureStdout();
    testShell.runCommand(cmdInput);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Fail"), std::string::npos);
}

TEST(TestShellTest, FullWriteAndReadCompare_Pass) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell;
    testShell.setSsdAdapter(&mockSSDAdapter);

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

TEST(TestShellTest, FullWriteAndReadCompare_Fail) {
    MockSSDAdapter mockSSDAdapter;
    TestShell testShell;
    testShell.setSsdAdapter(&mockSSDAdapter);

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
#endif