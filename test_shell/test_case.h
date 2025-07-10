#pragma once
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