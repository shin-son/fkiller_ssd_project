#include "gmock/gmock.h"
#include "buffer_manager.h"
#include <filesystem>

namespace fs = std::filesystem;

TEST(BufferManagerTest, WriteCommand_First_Buffer) {
    const std::string testDir = "./test_buffer_write";

    if (fs::exists(testDir) && fs::is_directory(testDir))
        fs::create_directory(testDir);

    BufferManager mgr(testDir);
    mgr.addWrite(3, "0xABCD1234");

    bool found = false;
    for (const auto& entry : fs::directory_iterator(testDir)) {
        std::string filename = entry.path().filename().string();
        if (filename.find("_w_3_0xABCD1234") != std::string::npos) {
            found = true;
            break;
        }
    }

    EXPECT_TRUE(found) << "Expected write command file not found in buffer";
}

TEST(BufferManagerTest, WriteCommand_full_buffer) {
    const std::string testDir = "./test_buffer_write";

    if (fs::exists(testDir) && fs::is_directory(testDir))
        fs::create_directory(testDir);

    BufferManager mgr(testDir);
    mgr.addWrite(5, "0x3214");
    mgr.addWrite(7, "0x3214");
    mgr.addWrite(9, "0xaaaa");
    mgr.addWrite(10, "0xbbbb");
}