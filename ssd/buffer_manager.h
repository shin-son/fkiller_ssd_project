#pragma once
#include<string>
#include<vector>

class BufferManager {
public:
	BufferManager(const std::string& bufferDir = "./buffer");
	bool addWrite(int lba, const std::string& value);
	bool addErase(int lba, int size);
	std::vector<std::vector<std::string>> flushBuffer();
	void resetAllBuffer();
	std::string addRead(int lba);

private:
	std::string bufferDirectory;
	std::string sameAddressBuffer;

	int findEmtpyBuffer();
	std::string formatWriteFileName(int slotIndex, int lba, const std::string& value);
	std::string formatEraseFileName(int bufferIdx, int lba, int size);
	int findWriteSameAddress(int lba);
	void setTheSameAddressBuffer(const std::string& filename);
	std::string getTheSameAddressBuffer();
	std::vector<std::string> splitByUnderscore(const std::string& input);
};