#pragma once
#include<string>

class BufferManager {
public:
	BufferManager(const std::string& bufferDir = "./buffer");
	bool addWrite(int lba, const std::string& value);
	bool addErase(int lba, int size);
	void resetAllBuffer();

private:
	std::string bufferDirectory;
	std::string sameAddressBuffer;

	int findEmtpyBuffer();
	std::string formatWriteFileName(int slotIndex, int lba, const std::string& value);
	std::string formatEraseFileName(int bufferIdx, int lba, int size);
	int findSameAddress(int lba);
	void setTheSameAddressBuffer(const std::string& filename);
	std::string getTheSameAddressBuffer();
};