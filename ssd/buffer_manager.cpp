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
		createInitBufferFile();
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
	for (int innerIndex = index + 1; innerIndex < bufferEntries.size(); innerIndex++) {
		BufferEntry& oldBuffer = bufferEntries[innerIndex - 1];
		BufferEntry& newBuffer = bufferEntries[innerIndex];
		std::string newFileName = std::to_string(oldBuffer.index) + newBuffer.originalFilename.substr(1);
		renameWithFileName(oldBuffer.originalFilename, newFileName);
		updateBufferInfo(oldBuffer, newBuffer);
	}
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
		else if (entry.type == CommandType::EMPTY)
			break;
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

void BufferManager::createInitBufferFile() {
	for (int idx = 1; idx < 6; idx++) {
		std::filesystem::path filePath = bufferDirectory + "/" + std::to_string(idx) + "_empty";
		std::ofstream file(filePath);
		if (!file.is_open()) {
			std::cout << "failed to create files" << std::endl;
		}
		file.close();
	}
}