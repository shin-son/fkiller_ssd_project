#include <iostream>
#include <string>
#include <cstdlib>
#include "mock_ssd_adapter.h"
#include "test_shell.h"

#ifdef _DEBUG
#include "gmock/gmock.h"

using namespace testing;

TEST(SampleTest, Addition) {
    EXPECT_EQ(2 + 2, 4);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#else
int main() {
    TestShell testShell;
    testShell.runShell();

    return 0;
}
#endif
