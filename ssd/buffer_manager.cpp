#include "buffer_manager.h"
#include <filesystem>
#include <fstream>
#include <string>
#include "ssd_constants.h"

// de
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
	int bufferIdx = findEmtpyBuffer();
	if (bufferIdx == -1) return false;

	// Only Empty case
	std::string empty_path = bufferDirectory + "/" + std::to_string(bufferIdx) + "_empty";
	std::string new_path = formatWriteFileName(bufferIdx, lba, value);

	fs::rename(empty_path, new_path);
	std::cout << "Renamed: " << empty_path << " ¡æ " << new_path << "\n";
	return true;
}

int BufferManager::findEmtpyBuffer() const {
	int i = 0;
	bool isEmpty = false;
	for (const auto& entry : fs::directory_iterator(bufferDirectory)) {
		++i;
		std::string filename = entry.path().filename().string();
		if (filename.rfind(std::to_string(i) + "_empty", 0) == 0) {
			isEmpty = true;
			break;
		}
	}
	if (isEmpty) return i;
	return -1;
}

std::string BufferManager::formatWriteFileName(int bufferIdx, int lba, const std::string& value) const {
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

std::string BufferManager::formatEraseFileName(int bufferIdx, int lba, int size) const {
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
