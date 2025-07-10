#include "gmock/gmock.h"
#include "buffer_manager.h"
#include <filesystem>
#include <fstream>
#include "command_process.h"

namespace fs = std::filesystem;

TEST(BufferManagerTest, WriteCommand_First_Buffer) {
	const std::string testDir = "./test_buffer_write";

	if (fs::exists(testDir) && fs::is_directory(testDir))
		fs::create_directory(testDir);

	BufferManager mgr(testDir);
	mgr.resetAllBuffer();
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

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_empty").close();
	std::ofstream(testDir + "/3_empty").close();
	std::ofstream(testDir + "/4_empty").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	//w_20_0x12341234
	mgr.addWrite(20, "0x12341234");
}

TEST(BufferManagerTest, FLUSH_TEST_1) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_w_1_0x12").close();
	std::ofstream(testDir + "/3_e_2_4").close();
	std::ofstream(testDir + "/4_w_10_0x12").close();
	std::ofstream(testDir + "/5_e_10_10").close();
	BufferManager mgr(testDir);

	//w_20_0x12341234
	mgr.addWrite(50, "0x12341234");
}

TEST(BufferManagerTest, READ_BUFFER_NO_MATCH) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_w_1_0x12").close();
	std::ofstream(testDir + "/3_e_2_4").close();
	std::ofstream(testDir + "/4_w_12_0x12").close();
	std::ofstream(testDir + "/5_e_14_10").close();

	BufferManager mgr(testDir);

	//ssd.exe r 90
	int lba = 90;
	std::string result = mgr.addRead(lba);
	EXPECT_EQ("", result);
}

TEST(BufferManagerTest, READ_BUFFER_MATCH) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_w_1_0x12").close();
	std::ofstream(testDir + "/3_e_2_4").close();
	std::ofstream(testDir + "/4_w_12_0x12").close();
	std::ofstream(testDir + "/5_e_14_10").close();
	BufferManager mgr(testDir);

	//ssd.exe r 3
	int lba = 3;
	std::string result = mgr.addRead(lba);
	EXPECT_EQ("0x00000000", result);
}