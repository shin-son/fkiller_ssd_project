#pragma once
#include<string>
#include<vector>

enum class CommandType {
	EMPTY,
	WRITE,
	ERASE,
	UNKNOWN
};

struct BufferEntry {
	int index = -1;
	CommandType type = CommandType::UNKNOWN;
	int lba = -1;
	std::string value = "";
	std::string originalFilename = "";
};

class BufferManager {
public:
	BufferManager(const std::string& bufferDir = "..\\buffer");
	void resetAllBuffer();
	void addWrite(int lba, const std::string& value);
	void addErase(int lba, int size);
	std::string addRead(int lba);
	void reloadBufferFiles();
	void flushAndReset();

private:
	std::string bufferDirectory;
	std::string sameAddressBuffer;

	int findEmptyBuffer();
	std::string formatWriteFileName(int slotIndex, int lba, const std::string& value);
	std::string formatEraseFileName(int bufferIdx, int lba, int size);
	int findWriteSameAddress(int lba);
	void setTheSameAddressBuffer(const std::string& filename);
	std::string getTheSameAddressBuffer();
	std::vector<std::string> splitByUnderscore(const std::string& input);

	int optimizeWriteBuffer(const int lba, const int size);
	bool isNeedWrite(const BufferEntry& buffer, const int lba, const int size);
	void removeBuffer(const int index);
	bool isLastBuffer(const int newBufferIndex);
	void renameWithFileName(const std::string& oldName, const std::string& newName);
	void updateBufferInfo(BufferEntry& oldBuffer, const BufferEntry& newBuffer);

	void optimizeEraseBuffer(const int emptyIdx);
	bool isExistOverlapErase(const BufferEntry& oldBuffer, const BufferEntry& newBuffer);
	bool updateEraseRange(BufferEntry& oldBuffer, const BufferEntry& newBuffer);
	void updateOldEraseBufferName(BufferEntry& oldBuffer);

	std::vector<BufferEntry> bufferEntries;
	void loadAndParseBufferFiles();
	BufferEntry parseFilename(const std::string& filename);

	void updateBufferPath(int idx, int lba, const std::string& value);
	void updateBufferPath(int bufferIdx, int idx, int size);
	bool checkWriteIdxIsEraseIdxBoundary(int lba, int emptyIdx, const std::string& value);

	std::vector<std::vector<std::string>> flushBuffer();
};