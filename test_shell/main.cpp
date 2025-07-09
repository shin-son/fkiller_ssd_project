#include "test_shell.h"
#include "ssd_adapter.h"
#include "runner.h"

#ifdef _DEBUG
#include "mock_ssd_adapter.h"

using namespace testing;

int main(int argc, char** argv) {
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#else
int main(int argc, char** argv) {
    std::string command;
    TestShell testShell;
    SSDAdapter ssdAdapter;

    if (argc == CONVERSATION_MODE) {
        testShell.setSsdAdapter(&ssdAdapter);
        testShell.runShell();
    }
    else if (argc == TEST_SCRIPT_MODE) {
        TestRunner runner(testShell, ssdAdapter);
        runner.verifyAndRunScript(argv[1]);
    }

    return 0;
}
#endif
