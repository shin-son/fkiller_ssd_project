#pragma once
#include "ssd_interface.h"

class MockSSDAdapter : public SSDInterface {
public:
    MOCK_METHOD(string, read, (const int LBA), (override));
    MOCK_METHOD(string, write, (const int LBA, const string& data), (override));
};