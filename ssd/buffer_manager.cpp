#include "buffer_manager.h"
#include <filesystem>
#include <fstream>
#include <string>
#include "ssd_constants.h"
#include <sstream>

// debugging
#include <iostream>
using namespace std;
//

namespace fs = std::filesystem;

BufferManager::BufferManager(const std::string& bufferDir)
	: bufferDirectory(bufferDir) {
	if (!fs::exists(bufferDirectory)) {
		fs::create_directory(bufferDirectory);
	}
}

bool BufferManager::addWrite(int lba, const std::string& value) {
	int bufferIdx = 0;

	bufferIdx = findWriteSameAddress(lba);
	if (bufferIdx != NOT_FOUND_ANY_BUFFER) {
		std::string new_path = formatWriteFileName(bufferIdx, lba, value);
		std::string old_path = bufferDirectory + "/" + getTheSameAddressBuffer();
		fs::rename(old_path, new_path);
		std::cout << "Renamed: " << old_path << " ¡æ " << new_path << "\n";
		return true;
	}

	bufferIdx = findEmtpyBuffer();
	if (bufferIdx == ALL_BUFFER_USED) return false;

	// Only Empty case
	std::string empty_path = bufferDirectory + "/" + std::to_string(bufferIdx) + "_empty";
	std::string new_path = formatWriteFileName(bufferIdx, lba, value);

	fs::rename(empty_path, new_path);
	std::cout << "Renamed: " << empty_path << " ¡æ " << new_path << "\n";
	return true;
}

std::string BufferManager::getTheSameAddressBuffer() {
	return sameAddressBuffer;
}

int BufferManager::findEmtpyBuffer() {
	int bufferIdx = 0;
	bool isEmpty = false;
	for (const auto& entry : fs::directory_iterator(bufferDirectory)) {
		++bufferIdx;
		std::string filename = entry.path().filename().string();
		if (filename.rfind(std::to_string(bufferIdx) + "_empty", 0) == 0) {
			isEmpty = true;
			break;
		}
	}
	if (isEmpty) return bufferIdx;
	return ALL_BUFFER_USED;
}

int BufferManager::findWriteSameAddress(int lba) {
	int bufferIdx = 0;
	bool isSameAddress = false;
	std::string fileName;
	for (const auto& entry : fs::directory_iterator(bufferDirectory)) {
		++bufferIdx;
		fileName = entry.path().filename().string();
		if (fileName.rfind(std::to_string(bufferIdx) + "_w_" + std::to_string(lba) + "_", 0) == 0) {
			isSameAddress = true;
			break;
		}
	}

	if (isSameAddress) {
		setTheSameAddressBuffer(fileName);
		return bufferIdx;
	}

	return NOT_FOUND_ANY_BUFFER;

}

void BufferManager::setTheSameAddressBuffer(const std::string& filename) {
	sameAddressBuffer = filename;
}


std::string BufferManager::formatWriteFileName(int bufferIdx, int lba, const std::string& value) {
	return bufferDirectory + "/" + std::to_string(bufferIdx) + "_w_" +
		std::to_string(lba) + "_" + value;
}

bool BufferManager::addErase(int lba, int size) {
	int bufferIdx = findEmtpyBuffer();
	if (bufferIdx == -1) return false;

	// Only Empty case
	std::string empty_path = bufferDirectory + "/" + std::to_string(bufferIdx) + "_empty";
	std::string new_path = formatEraseFileName(bufferIdx, lba, size);

	fs::rename(empty_path, new_path);
	std::cout << "Renamed: " << empty_path << " ¡æ " << new_path << "\n";
	return true;
}

std::string BufferManager::formatEraseFileName(int bufferIdx, int lba, int size) {
	return bufferDirectory + "/" + std::to_string(bufferIdx) + "_e_" +
		std::to_string(lba) + "_" + std::to_string(size);

}
void BufferManager::resetAllBuffer() {
	int bufferIdx = 0;
	for (const auto& entry : fs::directory_iterator(bufferDirectory)) {
		std::string nowFile = bufferDirectory + "/" + entry.path().filename().string();
		std::string empty_path = bufferDirectory + "/" + std::to_string(++bufferIdx) + "_empty";
		fs::rename(nowFile, empty_path);
	}
}

std::string BufferManager::addRead(int lba) {
	int bufferIdx = 0;
	std::string fileName;
	std::string bufferIdxToString;
	std::string operatorString;
	std::string lbaString;
	std::string memoryValue;

	for (const auto& entry : fs::directory_iterator(bufferDirectory)) {
		fileName = entry.path().filename().string();
		std::stringstream ss(fileName);
		std::getline(ss, bufferIdxToString, '_'); // bufferIdx
		std::getline(ss, operatorString, '_'); // e or w
		std::getline(ss, lbaString, '_'); // startId
		std::getline(ss, memoryValue, '_'); // memoryVaue or size

		if (operatorString == "w") {
			if (std::stoi(lbaString) == lba)
				return memoryValue;
		}
		else if (operatorString == "e") {
			int start_idx =  std::stoi(lbaString);
			int end_idx = std::stoi(memoryValue) + start_idx - 1;
			if (start_idx <= lba && end_idx >= lba) return INIT_STRING;
			else
				return EMPTY_STRING;
		}
	}

	return EMPTY_STRING;
}

std::vector<std::vector<std::string>> BufferManager::flushBuffer() {
	std::vector<std::vector<std::string>> commands;
	std::cout << bufferDirectory  << std::endl;
	for (const auto& entry : fs::directory_iterator(bufferDirectory)) {
		std::string filename = entry.path().filename().string();
		auto tokens = splitByUnderscore(filename);
		for (auto& t : tokens) std::cout << t << " ";
		if (tokens.size() < 4) continue;

		std::vector<std::string> cmd;
		cmd.insert(cmd.end(), tokens.begin(), tokens.end());
		commands.push_back(cmd);
	}
	return commands;

}
std::vector<std::string> BufferManager::splitByUnderscore(const std::string& input) {
	std::vector<std::string> tokens;
	std::stringstream ss(input);
	std::string item;

	std::getline(ss, item, '_');

	tokens.push_back("ssd.exe");
	while (std::getline(ss, item, '_')) {
		tokens.push_back(item);
	}

	return tokens;
}