#include "test_shell.h"

#ifdef _DEBUG
#include "mock_ssd_adapter.h"

using namespace testing;

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
