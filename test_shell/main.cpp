#include "test_shell.h"
#include "ssd_adapter.h"

#ifdef _DEBUG
#include "mock_ssd_adapter.h"

using namespace testing;

int main(int argc, char** argv) {
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#else
int main() {
    TestShell testShell;
    SSDAdapter ssdAdapter;
    testShell.setSsdAdapter(&ssdAdapter);
    testShell.runShell();

    return 0;
}
#endif
