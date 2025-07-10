#include "ssd_initial_files.h"
#include <fstream>
#include <filesystem>
#include <iomanip>

void SsdInitialFiles::initialize() {
    namespace fs = std::filesystem;
    const std::string nandFile = "ssd_nand.txt";
    const std::string outputFile = "ssd_output.txt";
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
}