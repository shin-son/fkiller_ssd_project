#pragma once
#include<string>

class BufferManager {
public:
    BufferManager(const std::string& bufferDir = "./buffer");
    bool addWrite(int lba, const std::string& value);
    void resetAllBuffer();

private:
    std::string bufferDirectory;

    int findEmtpyBuffer() const;
    std::string formatWriteFileName(int slotIndex, int lba, const std::string& value) const;
};