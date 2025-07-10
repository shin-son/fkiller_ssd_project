#pragma once
#ifdef _DEBUG
#include "test_case.h"

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

    internal::CaptureStdout();
    testShell.runCommand("read 10");
    string output = internal::GetCapturedStdout();

    EXPECT_NE(output.find("[Read] LBA"), std::string::npos);
}

TEST_F(TestShellFixture, ReadFailWrongLBA) {
    EXPECT_CALL(mockSSDAdapter, read(100))
        .Times(1)
        .WillOnce(Return(ERROR));

    internal::CaptureStdout();
    testShell.runCommand("read 100");
    string output = internal::GetCapturedStdout();

    EXPECT_NE(output.find("[Read] ERROR"), std::string::npos);
}

TEST_F(TestShellFixture, FullReadPass) {
    EXPECT_CALL(mockSSDAdapter, read(_))
        .Times(100)
        .WillRepeatedly(Return("0x00ABCDEF"));

    string expected = "[fullRead]\n";
    for (int LBA = 0; LBA < 100; LBA++) {
        expected += "LBA " + std::to_string(LBA) + " : " + "0x00ABCDEF\n";
    }
    internal::CaptureStdout();
    testShell.runCommand("fullread");
    string output = internal::GetCapturedStdout();

    EXPECT_EQ(expected, output);
}

TEST_F(TestShellFixture, FullReadFail) {
    EXPECT_CALL(mockSSDAdapter, read(_))
        .Times(100)
        .WillOnce(Return("0x00ABCDEF"))
        .WillRepeatedly(Return(ERROR));

    string expected = "[fullRead]\nLBA 0 : 0x00ABCDEF\n";
    for (int LBA = 1; LBA < 100; LBA++) {
        expected += ("LBA " + std::to_string(LBA) + " : " + ERROR + "\n");
    }
    internal::CaptureStdout();
    testShell.runCommand("fullread");
    string output = internal::GetCapturedStdout();

    EXPECT_EQ(expected, output);
}

TEST_F(TestShellFixture, WritePass) {
    EXPECT_CALL(mockSSDAdapter, write(5, "0xAAAABBBB"))
        .Times(1)
        .WillOnce(Return(""));

    testing::internal::CaptureStdout();
    testShell.runCommand("write 5 0xAAAABBBB");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("[Write] Done"), std::string::npos);
}

TEST_F(TestShellFixture, WriteFail) {
    EXPECT_CALL(mockSSDAdapter, write(5, "0xAAAAABBBBB"))
        .WillOnce(Return(ERROR));

    testing::internal::CaptureStdout();
    testShell.runCommand("write 5 0xAAAAABBBBB");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("[Write] ERROR"), std::string::npos);
}

TEST_F(TestShellFixture, FullWritePass) {
    
    for (int i = 0; i < 100; ++i) {
        EXPECT_CALL(mockSSDAdapter, write(i, "0xAAAABBBB"))
            .Times(1)
            .WillRepeatedly(Return(""));
    }

    internal::CaptureStdout();
    testShell.runCommand("fullwrite 0xAAAABBBB");
    std::string output = internal::GetCapturedStdout();

    EXPECT_NE(output.find("[fullWrite] Done"), std::string::npos);
}

TEST_F(TestShellFixture, FullWriteFail) {
    EXPECT_CALL(mockSSDAdapter, write(_, _))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillOnce(Return("[Write] ERROR"));

    internal::CaptureStdout();
    testShell.runCommand("fullwrite 0xAAAABBBB");
    std::string output = internal::GetCapturedStdout();

    EXPECT_NE(output.find("[fullWrite] Failed at LBA 3"), std::string::npos);
}

TEST_F(TestShellFixture, EraseWithSize) {

    EXPECT_CALL(mockSSDAdapter, erase(_, _))
        .Times(1)
        .WillRepeatedly(Return(""));

    testing::internal::CaptureStdout();
    testShell.runCommand("erase 0 10");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("[Erase] Done"), std::string::npos);
}

TEST_F(TestShellFixture, EraseWithInvalidSize) {

    testing::internal::CaptureStdout();
    testShell.runCommand("erase 0 101");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output.find("[Erase] Done"), std::string::npos);
    EXPECT_NE(output.find("[Erase] ERROR"), std::string::npos);
}

TEST_F(TestShellFixture, EraseWithSize_CheckEraseCount) {

    EXPECT_CALL(mockSSDAdapter, erase(_, _))
        .Times(1)
        .WillRepeatedly(Return(""));

    testing::internal::CaptureStdout();
    testShell.runCommand("erase 0 10");
    std::string output = testing::internal::GetCapturedStdout();

    //

    EXPECT_CALL(mockSSDAdapter, erase(_, _))
        .Times(2)
        .WillRepeatedly(Return(""));

    testing::internal::CaptureStdout();
    testShell.runCommand("erase 0 11");
    output = testing::internal::GetCapturedStdout();

    //

    EXPECT_CALL(mockSSDAdapter, erase(_, _))
        .Times(5)
        .WillRepeatedly(Return(""));

    testing::internal::CaptureStdout();
    testShell.runCommand("erase 1 50");
    output = testing::internal::GetCapturedStdout();

    //

    EXPECT_CALL(mockSSDAdapter, erase(_, _))
        .Times(6)
        .WillRepeatedly(Return(""));

    testing::internal::CaptureStdout();
    testShell.runCommand("erase 2 53");
    output = testing::internal::GetCapturedStdout();
}

TEST_F(TestShellFixture, EraseWithSize_EraseFailCase) {

    EXPECT_CALL(mockSSDAdapter, erase(_, _))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillOnce(Return(ERROR))
        .WillRepeatedly(Return(""));


    testing::internal::CaptureStdout();
    testShell.runCommand("erase 0 100");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("[Erase] ERROR"), std::string::npos);
}

TEST_F(TestShellFixture, EraseRange_Pass) {

    EXPECT_CALL(mockSSDAdapter, erase(_, _))
        .Times(1)
        .WillRepeatedly(Return(""));

    testing::internal::CaptureStdout();
    testShell.runCommand("erase_range 0 9");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find(ERASE_RANGE_DONE_RETURN), std::string::npos);
}

TEST_F(TestShellFixture, EraseRangeWithValidEndLBA_EdgeCase) {

    testing::internal::CaptureStdout();
    testShell.runCommand("erase_range 0 99");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find(ERASE_RANGE_DONE_RETURN), std::string::npos);
    EXPECT_EQ(output.find(ERASE_RANGE_FAIL_RETURN), std::string::npos);
}

TEST_F(TestShellFixture, EraseRangeWithInvalidEndLBA) {

    testing::internal::CaptureStdout();
    testShell.runCommand("erase_range 0 100");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output.find(ERASE_RANGE_DONE_RETURN), std::string::npos);
    EXPECT_NE(output.find(ERASE_RANGE_FAIL_RETURN), std::string::npos);
}

TEST_F(TestShellFixture, EraseRange_CheckEraseCount) {

    // expect calling erase one time
    EXPECT_CALL(mockSSDAdapter, erase(_, _))
        .Times(1)
        .WillRepeatedly(Return(""));

    testing::internal::CaptureStdout();
    testShell.runCommand("erase_range 0 9");
    std::string output = testing::internal::GetCapturedStdout();

    // expect calling erase two times
    EXPECT_CALL(mockSSDAdapter, erase(_, _))
        .Times(2)
        .WillRepeatedly(Return(""));

    testing::internal::CaptureStdout();
    testShell.runCommand("erase_range 0 10");
    output = testing::internal::GetCapturedStdout();

    // expect calling erase 5 times
    EXPECT_CALL(mockSSDAdapter, erase(_, _))
        .Times(5)
        .WillRepeatedly(Return(""));

    testing::internal::CaptureStdout();
    testShell.runCommand("erase_range 2 51");
    output = testing::internal::GetCapturedStdout();

    // expect calling erase six times
    EXPECT_CALL(mockSSDAdapter, erase(_, _))
        .Times(6)
        .WillRepeatedly(Return(""));

    testing::internal::CaptureStdout();
    testShell.runCommand("erase_range 2 53");
    output = testing::internal::GetCapturedStdout();
}

TEST_F(TestShellFixture, EraseRange_EraseFailCase) {

    EXPECT_CALL(mockSSDAdapter, erase(_, _))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillOnce(Return(""))
        .WillOnce(Return(ERROR))
        .WillRepeatedly(Return(""));


    testing::internal::CaptureStdout();
    testShell.runCommand("erase_range 0 55");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find(ERASE_RANGE_FAIL_RETURN), std::string::npos);
}
#endif