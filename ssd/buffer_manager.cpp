#include "buffer_manager.h"
#include <filesystem>
#include <fstream>
#include "ssd_constants.h"
#include <sstream>
#include <algorithm>

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

bool BufferManager::addWrite(int lba, const std::string& value) {
	int idx = findWriteSameAddress(lba);
	if (idx != NOT_FOUND_ANY_BUFFER) {
		std::string old_path = bufferDirectory + "/" + bufferEntries[idx].originalFilename;
		std::string new_path = formatWriteFileName(bufferEntries[idx].index, lba, value);
		std::filesystem::rename(old_path, new_path);
		reloadBufferFiles();
		return true;
	}

	int emptyIdx = findEmptyBuffer();
	if (emptyIdx == ALL_BUFFER_USED) return false;

	std::string old_path = bufferDirectory + "/" + bufferEntries[emptyIdx].originalFilename;
	std::string new_path = formatWriteFileName(bufferEntries[emptyIdx].index, lba, value);
	std::filesystem::rename(old_path, new_path);
	reloadBufferFiles();
	return true;
}

bool BufferManager::addErase(int lba, int size) {
	int emptyIdx = findEmptyBuffer();
	if (emptyIdx == ALL_BUFFER_USED) return false;

	std::string old_path = bufferDirectory + "/" + bufferEntries[emptyIdx].originalFilename;
	std::string new_path = formatEraseFileName(bufferEntries[emptyIdx].index, lba, size);
	std::filesystem::rename(old_path, new_path);
	reloadBufferFiles();
	return true;
}

std::string BufferManager::addRead(int lba) {
	for (const auto& entry : bufferEntries) {
		if (entry.type == CommandType::WRITE && entry.lba == lba) {
			return entry.value;
		}
		if (entry.type == CommandType::ERASE) {
			int start = entry.lba;
			int end = start + std::stoi(entry.value) - 1;
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
	for (size_t bufferIdx = 0; bufferIdx < bufferEntries.size(); ++bufferIdx) {
		if (bufferEntries[bufferIdx].type == CommandType::EMPTY)
			return bufferIdx;
	}
	return ALL_BUFFER_USED;
}

int BufferManager::findWriteSameAddress(int lba) {
	for (size_t bufferIdx = 0; bufferIdx < bufferEntries.size(); ++bufferIdx) {
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
