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

    EXPECT_CALL(mockSSD, write(5, "0xAAAABBBB"))
        .WillOnce(Return("ERROR"));

    std::string result = shell.write(5, "0xAAAABBBB");
    EXPECT_EQ(result, "[Write] ERROR");
}

TEST(TestShellTest, FullWrite_Pass) {
    MockSSDAdapter mockSSD;
    TestShell shell;
    shell.setSsdAdapter(&mockSSD);

    EXPECT_CALL(mockSSD, write(_, _))
        .Times(100)
        .WillRepeatedly(Return(""));

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

#endif