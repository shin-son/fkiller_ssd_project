#pragma once
#include "ssd_interface.h"
#include "gmock/gmock.h"

class MockSSDAdapter : public SSDInterface {
public:
    MOCK_METHOD(string, read, (const int LBA), (override));
    MOCK_METHOD(string, write, (const int LBA, const string& data), (override));
    MOCK_METHOD(string, erase, (const int LBA, const int size), (override));
    MOCK_METHOD(string, flush, (), (override));
};