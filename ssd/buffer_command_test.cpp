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
	// process ssd.exe w 3 0xABCD1234
	mgr.addWrite(3, "0xABCD1234");

	std::vector<std::string> expected = { "1_w_3_0xABCD1234",
											"2_empty",
											"3_empty",
											"4_empty",
											"5_empty" };
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
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

TEST(BufferManagerTest, READ_BUFFER_MATCH_1) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_e_0_4").close();
	std::ofstream(testDir + "/2_w_1_0x2").close();
	std::ofstream(testDir + "/3_w_3_0x4").close();
	std::ofstream(testDir + "/4_e_12_2").close();
	std::ofstream(testDir + "/5_w_13_0x10").close();
	BufferManager mgr(testDir);

	//ssd.exe r 1
	int lba = 1;
	std::string result = mgr.addRead(lba);
	EXPECT_EQ("0x2", result);
}

TEST(BufferManagerTest, READ_BUFFER_MATCH_2) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_e_0_4").close();
	std::ofstream(testDir + "/2_w_1_0x2").close();
	std::ofstream(testDir + "/3_w_12_0x4").close();
	std::ofstream(testDir + "/4_e_10_5").close();
	std::ofstream(testDir + "/5_w_13_0x10").close();
	BufferManager mgr(testDir);

	//ssd.exe r 12
	int lba = 12;
	std::string result = mgr.addRead(lba);
	EXPECT_EQ("0x00000000", result);
}

TEST(BufferManagerTest, EraseAfterWrite1) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_empty").close();
	std::ofstream(testDir + "/3_empty").close();
	std::ofstream(testDir + "/4_empty").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addErase(20, 2);

	std::vector<std::string> expected = {"1_e_20_2",
											"2_empty",
											"3_empty",
											"4_empty",
											"5_empty"};
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, EraseAfterWrite2) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_w_30_0xABCDABCD").close();
	std::ofstream(testDir + "/3_w_31_0xABCDABCD").close();
	std::ofstream(testDir + "/4_empty").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addErase(20, 2);

	std::vector<std::string> expected = {"1_w_30_0xABCDABCD",
											"2_w_31_0xABCDABCD",
											"3_e_20_2",
											"4_empty",
											"5_empty"};
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, EraseAfterWrite3) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_w_30_0xABCDABCD").close();
	std::ofstream(testDir + "/3_w_21_0xABCDABCD").close();
	std::ofstream(testDir + "/4_w_31_0xABCDABCD").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addErase(20, 2);

	std::vector<std::string> expected = {"1_w_30_0xABCDABCD",
											"2_w_31_0xABCDABCD",
											"3_e_20_2",
											"4_empty",
											"5_empty"};
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, EraseAfterWrite4) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_w_30_0xABCDABCD").close();
	std::ofstream(testDir + "/3_w_21_0xABCDABCD").close();
	std::ofstream(testDir + "/4_w_22_0xABCDABCD").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addErase(20, 3);

	std::vector<std::string> expected = {"1_w_30_0xABCDABCD",
											"2_e_20_3",
											"3_empty",
											"4_empty",
											"5_empty"};
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, EraseAfterWrite5) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_w_30_0xABCDABCD").close();
	std::ofstream(testDir + "/3_w_21_0xABCDABCD").close();
	std::ofstream(testDir + "/4_w_31_0xABCDABCD").close();
	std::ofstream(testDir + "/5_w_22_0xABCDABCD").close();
	BufferManager mgr(testDir);

	mgr.addErase(20, 3);

	std::vector<std::string> expected = {"1_e_20_3",
											"2_empty",
											"3_empty",
											"4_empty",
											"5_empty"};
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, MergeErase1) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_w_30_0xABCDABCD").close();
	std::ofstream(testDir + "/3_e_40_3").close();
	std::ofstream(testDir + "/4_empty").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addErase(43, 3);

	std::vector<std::string> expected = { "1_w_20_0xABCDABCD",
											"2_w_30_0xABCDABCD",
											"3_e_40_6",
											"4_empty",
											"5_empty" };
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, MergeErase2) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_w_30_0xABCDABCD").close();
	std::ofstream(testDir + "/3_e_40_3").close();
	std::ofstream(testDir + "/4_empty").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addErase(41, 3);

	std::vector<std::string> expected = { "1_w_20_0xABCDABCD",
											"2_w_30_0xABCDABCD",
											"3_e_40_4",
											"4_empty",
											"5_empty" };
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, MergeErase3) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_w_30_0xABCDABCD").close();
	std::ofstream(testDir + "/3_e_40_3").close();
	std::ofstream(testDir + "/4_empty").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addErase(41, 3);

	std::vector<std::string> expected = { "1_w_20_0xABCDABCD",
											"2_w_30_0xABCDABCD",
											"3_e_40_4",
											"4_empty",
											"5_empty" };
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, MergeErase4) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_w_30_0xABCDABCD").close();
	std::ofstream(testDir + "/3_e_40_3").close();
	std::ofstream(testDir + "/4_w_41_0xABCDABCD").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addErase(41, 3);

	std::vector<std::string> expected = { "1_w_20_0xABCDABCD",
											"2_w_30_0xABCDABCD",
											"3_e_40_4",
											"4_empty",
											"5_empty" };
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, MergeErase5) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_w_30_0xABCDABCD").close();
	std::ofstream(testDir + "/3_e_40_3").close();
	std::ofstream(testDir + "/4_w_41_0xABCDABCD").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addErase(43, 3);

	std::vector<std::string> expected = { "1_w_20_0xABCDABCD",
											"2_w_30_0xABCDABCD",
											"3_e_40_6",
											"4_w_41_0xABCDABCD",
											"5_empty" };
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, MergeErase6) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_w_30_0xABCDABCD").close();
	std::ofstream(testDir + "/3_e_40_3").close();
	std::ofstream(testDir + "/4_w_31_0xABCDABCD").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addErase(43, 3);

	std::vector<std::string> expected = { "1_w_20_0xABCDABCD",
											"2_w_30_0xABCDABCD",
											"3_e_40_6",
											"4_w_31_0xABCDABCD",
											"5_empty" };
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, MergeErase7) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_w_30_0xABCDABCD").close();
	std::ofstream(testDir + "/3_e_40_6").close();
	std::ofstream(testDir + "/4_w_31_0xABCDABCD").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addErase(42, 3);

	std::vector<std::string> expected = { "1_w_20_0xABCDABCD",
											"2_w_30_0xABCDABCD",
											"3_e_40_6",
											"4_w_31_0xABCDABCD",
											"5_empty" };
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, MergeErase8) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_e_40_2").close();
	std::ofstream(testDir + "/3_e_44_2").close();
	std::ofstream(testDir + "/4_w_31_0xABCDABCD").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addErase(42, 3);

	std::vector<std::string> expected = { "1_w_20_0xABCDABCD",
											"2_e_40_6",
											"3_w_31_0xABCDABCD",
											"4_empty",
											"5_empty" };
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, MergeErase9) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_e_40_2").close();
	std::ofstream(testDir + "/3_w_31_0xABCDABCD").close();
	std::ofstream(testDir + "/4_e_44_2").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addErase(42, 3);

	std::vector<std::string> expected = { "1_w_20_0xABCDABCD",
											"2_e_40_6",
											"3_w_31_0xABCDABCD",
											"4_empty",
											"5_empty" };
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, MergeErase10) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_w_20_0xABCDABCD").close();
	std::ofstream(testDir + "/2_e_40_2").close();
	std::ofstream(testDir + "/3_e_44_5").close();
	std::ofstream(testDir + "/4_w_31_0xABCDABCD").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addErase(49, 8);

	std::vector<std::string> expected = { "1_w_20_0xABCDABCD",
											"2_e_40_2",
											"3_e_44_5",
											"4_w_31_0xABCDABCD",
											"5_e_49_8" };
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, WriteAfterErase) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_e_0_2").close();
	std::ofstream(testDir + "/2_empty").close();
	std::ofstream(testDir + "/3_empty").close();
	std::ofstream(testDir + "/4_empty").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addWrite(1, "0x12341234");

	std::vector<std::string> expected = { "1_e_0_1",
											"2_w_1_0x12341234",
											"3_empty",
											"4_empty",
											"5_empty" };
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, WriteAfterErase2) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_e_0_2").close();
	std::ofstream(testDir + "/2_empty").close();
	std::ofstream(testDir + "/3_empty").close();
	std::ofstream(testDir + "/4_empty").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addWrite(0, "0x12341234");

	std::vector<std::string> expected = { "1_e_1_1",
											"2_w_0_0x12341234",
											"3_empty",
											"4_empty",
											"5_empty" };
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, WriteAfterErase3) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_e_0_2").close();
	std::ofstream(testDir + "/2_empty").close();
	std::ofstream(testDir + "/3_empty").close();
	std::ofstream(testDir + "/4_empty").close();
	std::ofstream(testDir + "/5_empty").close();
	BufferManager mgr(testDir);

	mgr.addWrite(0, "0x10101010");
	mgr.addWrite(1, "0x20202020");

	std::vector<std::string> expected = { "1_w_0_0x10101010",
											"2_w_1_0x20202020",
											"3_empty",
											"4_empty",
											"5_empty" };
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}

TEST(BufferManagerTest, WriteAfterFullBuffer) {
	const std::string testDir = "./test_buffer_write";
	fs::remove_all(testDir);
	fs::create_directory(testDir);

	std::ofstream(testDir + "/1_e_0_2").close();
	std::ofstream(testDir + "/2_e_5_2").close();
	std::ofstream(testDir + "/3_w_10_0x12341234").close();
	std::ofstream(testDir + "/4_e_24_0").close();
	std::ofstream(testDir + "/5_w_55_0x11112222").close();
	BufferManager mgr(testDir);

	mgr.addWrite(0, "0x10101010");

	std::vector<std::string> expected = { "1_w_0_0x10101010",
											"2_empty",
											"3_empty",
											"4_empty",
											"5_empty" };
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(testDir)) {
		files.push_back(entry.path().filename().string());
	}

	EXPECT_EQ(expected, files);
}