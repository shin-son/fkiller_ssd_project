#include "gmock/gmock.h"
#include "buffer_manager.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

TEST(BufferManagerTest, WriteCommand_First_Buffer) {
	const std::string testDir = "./test_buffer_write";

	if (fs::exists(testDir) && fs::is_directory(testDir))
		fs::create_directory(testDir);

	BufferManager mgr(testDir);
	// process ssd.exe w 3 0x234234
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

	BufferManager mgr(testDir);
	mgr.addWrite(5, "0x3214");
	mgr.addWrite(7, "0x3214");
	mgr.addWrite(9, "0xaaaa");
	mgr.addWrite(10, "0xbbbb");
}

TEST(BufferManagerTest, Make_All_empty_buffer) {
	const std::string testDir = "./test_buffer_write";

	BufferManager mgr(testDir);
	mgr.resetAllBuffer();
}

TEST(BufferManagerTest, EraseCommand_First_Buffer) {
	const std::string testDir = "./test_buffer_write";

	BufferManager mgr(testDir);
	// process ssd.exe e 3 1
	mgr.addErase(3, 1);

	bool found = false;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		std::string filename = entry.path().filename().string();
		if (filename.find("_e_3_1") != std::string::npos) {
			found = true;
			break;
		}
	}

	EXPECT_TRUE(found) << "Expected write command file not found in buffer";
}

TEST(BufferManagerTest, EraseCommand_full_Buffer) {
	const std::string testDir = "./test_buffer_write";

	BufferManager mgr(testDir);
	// process ssd.exe e 3 1
	mgr.addErase(3, 1);
	mgr.addErase(4, 5);
	mgr.addErase(6, 7);
	mgr.addErase(9, 10);
}

TEST(BufferManagerTest, Make_All_empty_buffer_1) {
	const std::string testDir = "./test_buffer_write";

	BufferManager mgr(testDir);
	mgr.resetAllBuffer();
}

TEST(BufferManagerTest, Write_ReplacesOldWriteCommand_1) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	BufferManager mgr(testDir);
	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_empty").close();
	std::ofstream(testDir + "/3_empty").close();
	std::ofstream(testDir + "/4_empty").close();
	std::ofstream(testDir + "/5_empty").close();


	//w_20_0x12341234
	mgr.addWrite(20, "0x12341234");
}
