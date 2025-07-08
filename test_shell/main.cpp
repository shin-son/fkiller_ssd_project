#include "test_shell.h"

#ifndef _DEBUG
int main() {
    TestShell testShell;
    testShell.runShell();

    return 0;
}
#endif
