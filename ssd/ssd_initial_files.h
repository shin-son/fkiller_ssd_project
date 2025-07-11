#pragma once
#include <string>

class SsdInitialFiles {
public:
    void initialize(const std::string& bufferDirectory);

private:
    void createInitBufferFile(const std::string& bufferDirectory);
};