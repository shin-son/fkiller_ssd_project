#include "ssd_initial_files.h"
#include <fstream>
#include <filesystem>
#include <iomanip>
#include "ssd_constants.h"
#include <iostream>

void SsdInitialFiles::initialize(const std::string& bufferDirectory) {
    namespace fs = std::filesystem;
    const std::string nandFile = "..\\ssd_nand.txt";
    const std::string outputFile = "..\\ssd_output.txt";
    if (!fs::exists(nandFile)) {
        std::ofstream nand(nandFile);
        for (int i = 0; i < 100; ++i) {
            nand << "0x" << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << 0 << "\n";
        }
        nand.close();
    }

    if (!fs::exists(outputFile)) {
        std::ofstream output(outputFile);
        output.close();
    }
    createInitBufferFile(bufferDirectory);
}

void SsdInitialFiles::createInitBufferFile(const std::string& bufferDirectory) {
    if (!std::filesystem::exists(bufferDirectory)) {
        std::filesystem::create_directory(bufferDirectory);
        for (int idx = 1; idx <= BUFFER_SIZE; idx++) {
            std::filesystem::path filePath = bufferDirectory + "/" + std::to_string(idx) + "_empty";
            std::ofstream file(filePath);
            if (!file.is_open()) {
                std::cout << "failed to create files" << std::endl;
            }
            file.close();
        }
    }
}