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
	if (cmdProcess.process(argc, argv) != SUCCESS) {
		cmdProcess.printErrorAndWriteToOutput();
		return 0;
	}

	int type = cmdProcess.getOperator();

	SsdFacade& ssdFacade = SsdFacade::getInstance();
	const std::string testDir = std::filesystem::current_path().string() + "/test_buffer_write";
	BufferManager mgr(testDir);
	if (type == WRITE_OPERATION) { // Write operation
		if (!mgr.addWrite(cmdProcess.getAddress(), cmdProcess.getInputValue())) {
			auto cmds = mgr.flushBuffer();
			for (auto& c : cmds) {
				CommandProcessor flushProc;
				if (flushProc.flushProcess(c) == SUCCESS) {
					ssdFacade.run(flushProc);
				}
			}
			mgr.resetAllBuffer();
			mgr.addWrite(cmdProcess.getAddress(), cmdProcess.getInputValue());
		}
	}
	else if (type == READ_OPERATION) { // Read operation
		ssdFacade.readSsdIndex(cmdProcess);
	}
	else if (type == ERASE_OPERATION) { // Erase operation
		ssdFacade.eraseSsdIndexToSize(cmdProcess);
	}
	else {
		// Handle other types or errors
	}

	return 0;

#endif
}
