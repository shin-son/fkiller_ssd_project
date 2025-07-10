#include "buffer_manager.h"
#include <filesystem>
#include <fstream>
#include "ssd_constants.h"
#include <sstream>
#include <algorithm>
#include "command_process.h"
#include "ssd_facade.h"

// debugging
#include <iostream>
using namespace std;
//

BufferManager::BufferManager(const std::string& bufferDir)
	: bufferDirectory(bufferDir) {
	if (!std::filesystem::exists(bufferDirectory)) {
		std::filesystem::create_directory(bufferDirectory);
	}
	loadAndParseBufferFiles();
}

void BufferManager::addWrite(int lba, const std::string& value) {
	int emptyIdx = findEmptyBuffer();
	if (emptyIdx == ALL_BUFFER_USED) {
		flushAndReset();
		emptyIdx = 0;
	}

	int idx = findWriteSameAddress(lba);
	if (idx != NOT_FOUND_ANY_BUFFER) {
		updateBufferPath(idx, lba, value);
		return;
	}

	if (checkWriteIdxIsEraseIdxBoundary(lba, emptyIdx, value)) return;

	updateBufferPath(emptyIdx, lba, value);
	return;
}

bool BufferManager::checkWriteIdxIsEraseIdxBoundary(int lba, int emptyIdx, const std::string& value)
{
	for (int bufferIdx = bufferEntries.size() - 1; bufferIdx >= 0; bufferIdx--) {
		if (bufferEntries[bufferIdx].type == CommandType::ERASE) {
			auto eraseStartIdx = bufferEntries[bufferIdx].lba;
			if (eraseStartIdx == lba) {
				auto newEraseStartIdx = eraseStartIdx + 1;
				auto newEraseSize = stoi(bufferEntries[bufferIdx].value) - 1;
				if (newEraseSize == 0) {
					removeBuffer(bufferIdx);
					updateBufferPath(emptyIdx - 1, lba, value);
					return true;
				}
				updateBufferPath(bufferIdx, newEraseStartIdx, newEraseSize);
				updateBufferPath(emptyIdx, lba, value);
				return true;
			}

			auto eraseEndIdx = eraseStartIdx + stoi(bufferEntries[bufferIdx].value) - 1;
			if (eraseEndIdx == lba) {
				auto newEraseSize = stoi(bufferEntries[bufferIdx].value) - 1;
				if (newEraseSize == 0) {
					removeBuffer(bufferIdx);
					updateBufferPath(emptyIdx - 1, lba, value);
					return true;
				}
				updateBufferPath(bufferIdx, eraseStartIdx, newEraseSize);
				updateBufferPath(emptyIdx, lba, value);
				return true;
			}
		}
	}
	return false;
}

void BufferManager::updateBufferPath(int bufferIdx, int lba, int size)
{
	std::string old_path = bufferDirectory + "/" + bufferEntries[bufferIdx].originalFilename;
	std::string new_path = formatEraseFileName(bufferEntries[bufferIdx].index, lba, size);
	std::filesystem::rename(old_path, new_path);
	reloadBufferFiles();
}

void BufferManager::updateBufferPath(int bufferIdx, int lba, const std::string& value)
{
	std::string old_path = bufferDirectory + "/" + bufferEntries[bufferIdx].originalFilename;
	std::string new_path = formatWriteFileName(bufferEntries[bufferIdx].index, lba, value);
	std::filesystem::rename(old_path, new_path);
	reloadBufferFiles();
}

void BufferManager::addErase(int lba, int size) {

	int emptyIdx = findEmptyBuffer();
	if (emptyIdx == ALL_BUFFER_USED)
		flushAndReset();

	emptyIdx = optimizeWriteBuffer(lba, size);

	std::string old_path = bufferDirectory + "/" + bufferEntries[emptyIdx].originalFilename;
	std::string new_path = formatEraseFileName(bufferEntries[emptyIdx].index, lba, size);
	std::filesystem::rename(old_path, new_path);
	reloadBufferFiles();

	optimizeEraseBuffer(emptyIdx);
}

int BufferManager::optimizeWriteBuffer(const int lba, const int size)
{
	int index = 0;
	while (index < bufferEntries.size()) {
		BufferEntry& buffer = bufferEntries[index];
		if (buffer.type == CommandType::EMPTY) break;
		if (isNeedWrite(buffer, lba, size)) {
			index++;
			continue;
		}

		removeBuffer(index);
	}
	return index;
}

bool BufferManager::isNeedWrite(const BufferEntry& buffer, const int lba, const int size)
{
	return !(buffer.type == CommandType::WRITE && buffer.lba >= lba && buffer.lba < lba + size);
}

void BufferManager::removeBuffer(const int index)
{
	if (isLastBuffer(index)) {
		std::string newFileNameNewBuffer = std::to_string(bufferEntries[index].index) + "_empty";
		renameWithFileName(bufferEntries[index].originalFilename, newFileNameNewBuffer);
		bufferEntries[index].type = CommandType::EMPTY;
		bufferEntries[index].originalFilename = newFileNameNewBuffer;
	}

	for (int innerIndex = index + 1; innerIndex < bufferEntries.size(); innerIndex++) {
		BufferEntry& oldBuffer = bufferEntries[innerIndex - 1];
		BufferEntry& newBuffer = bufferEntries[innerIndex];
		std::string newFileName = std::to_string(oldBuffer.index) + newBuffer.originalFilename.substr(1);
		renameWithFileName(oldBuffer.originalFilename, newFileName);
		updateBufferInfo(oldBuffer, newBuffer);
	}
}

bool BufferManager::isLastBuffer(const int newBufferIndex)
{
	return bufferEntries[newBufferIndex].index == bufferEntries.size();
}

void BufferManager::renameWithFileName(const std::string& oldName, const std::string& newName)
{
	std::string old_path = bufferDirectory + "/" + oldName;
	std::string new_path = bufferDirectory + "/" + newName;
	std::filesystem::rename(old_path, new_path);
}

void BufferManager::updateBufferInfo(BufferEntry& oldBuffer, const BufferEntry& newBuffer)
{
	oldBuffer.lba = newBuffer.lba;
	oldBuffer.originalFilename = std::to_string(oldBuffer.index) + newBuffer.originalFilename.substr(1);
	oldBuffer.type = newBuffer.type;
	oldBuffer.value = newBuffer.value;
}

void BufferManager::optimizeEraseBuffer(const int emptyIdx)
{
	int newBufferIndex = emptyIdx;
	for (int oldBufferIndex = emptyIdx - 1; oldBufferIndex >= 0; oldBufferIndex--) {
		BufferEntry& newBuffer = bufferEntries[newBufferIndex];
		BufferEntry& oldBuffer = bufferEntries[oldBufferIndex];

		if (isExistOverlapErase(oldBuffer, newBuffer) == false) continue;
		if (updateEraseRange(oldBuffer, newBuffer) == false) continue;

		updateOldEraseBufferName(oldBuffer);

		removeBuffer(newBufferIndex);

		newBufferIndex = oldBufferIndex;
	}
}

bool BufferManager::isExistOverlapErase(const BufferEntry& oldBuffer, const BufferEntry& newBuffer)
{
	int newStartIndex = newBuffer.lba;
	int newEndIndex = newBuffer.lba + std::stoi(newBuffer.value) - 1;
	int oldStartIndex = oldBuffer.lba;
	int oldEndIndex = oldBuffer.lba + std::stoi(oldBuffer.value) - 1;
	return oldBuffer.type == CommandType::ERASE
		&& ((newStartIndex <= oldEndIndex + 1)
			|| (newEndIndex <= oldStartIndex - 1));
}

bool BufferManager::updateEraseRange(BufferEntry& oldBuffer, const BufferEntry& newBuffer)
{
	int newStartIndex = newBuffer.lba;
	int newEndIndex = newBuffer.lba + std::stoi(newBuffer.value) - 1;
	int oldStartIndex = oldBuffer.lba;
	int oldEndIndex = oldBuffer.lba + std::stoi(oldBuffer.value) - 1;

	if (newStartIndex < oldStartIndex) {
		oldBuffer.lba = newStartIndex;
		int newValue = std::stoi(oldBuffer.value) + oldStartIndex - newStartIndex;
		if (newValue > 10) return false;
		oldBuffer.value = std::to_string(newValue);
	}
	if (newEndIndex > oldEndIndex) {
		int newValue = std::stoi(oldBuffer.value) + newEndIndex - oldEndIndex;
		if (newValue > 10) return false;
		oldBuffer.value = std::to_string(newValue);
	}
	return true;
}

void BufferManager::updateOldEraseBufferName(BufferEntry& oldBuffer)
{
	std::string newFileName = std::to_string(oldBuffer.index) + "_e_" + std::to_string(oldBuffer.lba) + "_" + oldBuffer.value;
	renameWithFileName(oldBuffer.originalFilename, newFileName);
	oldBuffer.originalFilename = newFileName;
}

std::string  BufferManager::addRead(int lba) {
	for(int bufferIdx = bufferEntries.size() - 1; bufferIdx >= 0; bufferIdx--) {
		if (bufferEntries[bufferIdx].type == CommandType::WRITE && bufferEntries[bufferIdx].lba == lba) {
			return bufferEntries[bufferIdx].value;
		}
		if (bufferEntries[bufferIdx].type == CommandType::ERASE) {
			int start = bufferEntries[bufferIdx].lba;
			int end = start + std::stoi(bufferEntries[bufferIdx].value) - 1;
			if (lba >= start && lba <= end) {
				return INIT_STRING;
			}
		}
	}
	return EMPTY_STRING;
}

void BufferManager::resetAllBuffer() {
	for (const auto& entry : bufferEntries) {
		std::string old_path = bufferDirectory + "/" + entry.originalFilename;
		std::string new_path = bufferDirectory + "/" + std::to_string(entry.index) + "_empty";
		std::filesystem::rename(old_path, new_path);
	}
	reloadBufferFiles();
}

std::vector<std::vector<std::string>> BufferManager::flushBuffer() {
	std::vector<std::vector<std::string>> result;
	for (const auto& entry : bufferEntries) {
		if (entry.type == CommandType::WRITE || entry.type == CommandType::ERASE) {
			result.push_back({ "ssd.exe",
							  (entry.type == CommandType::WRITE ? "w" : "e"),
							  std::to_string(entry.lba),
							  entry.value });
		}
	}
	return result;
}

void BufferManager::reloadBufferFiles() {
	loadAndParseBufferFiles();
}

void BufferManager::flushAndReset() {
	auto cmds = flushBuffer();
	for (auto& c : cmds) {
		CommandProcessor* flushProc = CommandProcessor::Builder()
			.setOperator(c[1])
			.setAddress(c[2])
			.setData(c[3])
			.patternCheck();

		if (flushProc->getResult() == SUCCESS) {
			SsdFacade& ssdFacade = SsdFacade::getInstance();

			switch (flushProc->getOperator()) {
			case WRITE_OPERATION:
				ssdFacade.writeSsdIndex(*flushProc);
				break;
			case READ_OPERATION:
				ssdFacade.readSsdIndex(*flushProc);
				break;
			case ERASE_OPERATION:
				ssdFacade.eraseSsdIndexToSize(*flushProc);
				break;
			default:
				break;
			}
		}
	}
	resetAllBuffer();
}

void BufferManager::loadAndParseBufferFiles() {
	bufferEntries.clear();
	for (const auto& entry : std::filesystem::directory_iterator(bufferDirectory)) {
		std::string filename = entry.path().filename().string();
		bufferEntries.push_back(parseFilename(filename));
	}
	std::sort(bufferEntries.begin(), bufferEntries.end(), [](const BufferEntry& a, const BufferEntry& b) {
		return a.index < b.index;
		});
}

BufferEntry BufferManager::parseFilename(const std::string& filename) {
	BufferEntry entry;
	entry.originalFilename = filename;

	std::stringstream ss(filename);
	std::string token;

	std::getline(ss, token, '_');
	entry.index = std::stoi(token);

	std::getline(ss, token, '_');
	if (token == "w") entry.type = CommandType::WRITE;
	else if (token == "e") entry.type = CommandType::ERASE;
	else if (token == "empty") entry.type = CommandType::EMPTY;
	else entry.type = CommandType::UNKNOWN;

	if (entry.type != CommandType::EMPTY && entry.type != CommandType::UNKNOWN) {
		std::getline(ss, token, '_');
		entry.lba = std::stoi(token);
		std::getline(ss, token, '_');
		entry.value = token;
	}

	return entry;
}

int BufferManager::findEmptyBuffer() {
	if (bufferEntries.size() < 5) return 0;

	for (size_t bufferIdx = 0; bufferIdx < bufferEntries.size(); ++bufferIdx) {
		if (bufferEntries[bufferIdx].type == CommandType::EMPTY)
			return bufferIdx;
	}
	return ALL_BUFFER_USED;
}

int BufferManager::findWriteSameAddress(int lba) {
	for (int bufferIdx = bufferEntries.size() - 1; bufferIdx >= 0; bufferIdx--) {
		if (bufferEntries[bufferIdx].type == CommandType::WRITE && bufferEntries[bufferIdx].lba == lba)
			return bufferIdx;
	}
	return ALL_BUFFER_USED;
}

std::string BufferManager::formatWriteFileName(int idx, int lba, const std::string& value) {
	return bufferDirectory + "/" + std::to_string(idx) + "_w_" + std::to_string(lba) + "_" + value;
}

std::string BufferManager::formatEraseFileName(int idx, int lba, int size) {
	return bufferDirectory + "/" + std::to_string(idx) + "_e_" + std::to_string(lba) + "_" + std::to_string(size);
}
