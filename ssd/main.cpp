#include "gmock/gmock.h"
#include "ssd_facade.h"
#include <string>
#include "ssd_constants.h"
#include "command_process.h"
#include "buffer_manager.h"
#include <iostream>

#include <filesystem>

int main(int argc, char** argv) {

#if _DEBUG
	
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();

#else
	CommandProcessor cmdProcess;
	std::filesystem::path currentPath = std::filesystem::current_path();
	std::cout << "Current working directory: " << currentPath << '\n';
	if (cmdProcess.process(argc, argv) == SUCCESS) {
		int type = cmdProcess.getOperator();
		int index = cmdProcess.getAddress();
		std::string target = cmdProcess.getInputValue();

		SsdFacade& ssdFacade = SsdFacade::getInstance();
		if (type == WRITE_OPERATION) {
			//const std::string testDir = "./test_buffer_write";
			const std::string testDir = std::filesystem::current_path().string() + "/test_buffer_write";
			BufferManager mgr(testDir);
			if (mgr.addWrite(index, target)) return 0;
			else {
				auto commands = mgr.flushBuffer();

				for (auto& cmd : commands) {
					CommandProcessor cp;
					cp.flushProcess(cmd);
					int cptype = cp.getOperator();
					int cpindex = cp.getAddress();
					std::string cptarget = cp.getInputValue();

					if (cptype == WRITE_OPERATION) {
						ssdFacade.writeSsdIndex(cpindex, cptarget);
					}
					else if (cptype == ERASE_OPERATION) {
						//
					}
				}
			}
		} else if (type == READ_OPERATION) { // Read operation
				ssdFacade.readSsdIndex(index);
		} else if (type == ERASE_OPERATION) {
			const std::string testDir = std::filesystem::current_path().string() + "/test_buffer_write";

			BufferManager mgr(testDir);
			if (mgr.addErase(index, std::stoi(target))) return 0;
			else {
				auto commands = mgr.flushBuffer();

				for (auto& cmd : commands) {
					CommandProcessor cp;
					cp.flushProcess(cmd);
					int cptype = cp.getOperator();
					int cpindex = cp.getAddress();
					std::string cptarget = cp.getInputValue();
					if (cptype == WRITE_OPERATION) {
						ssdFacade.writeSsdIndex(cpindex, cptarget);
					}
					else if (cptype == ERASE_OPERATION) {
						//
					}
				}
			}
		}
		//

		//if (type == WRITE_OPERATION) { // Write operation
		//	ssdFacade.writeSsdIndex(index, target);
		//}
		//else if (type == READ_OPERATION) { // Read operation
		//	ssdFacade.readSsdIndex(index);
		//}
		//else {
		//	// Handle other types or errors
		//}
	}
	else {
		cmdProcess.printErrorAndWriteToOutput();
	}
	return 0;

#endif
}
