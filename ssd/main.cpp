#include "gmock/gmock.h"
#include "ssd_facade.h"
#include <string>
#include "ssd_constants.h"
#include "command_process.h"
#include "buffer_manager.h"
#include <iostream>

#include <filesystem>

void flushAndReset(BufferManager& mgr, SsdFacade& facade) {
	auto cmds = mgr.flushBuffer();
	for (auto& c : cmds) {
		CommandProcessor* flushProc = CommandProcessor::Builder()
			.setParam(c)
			.setOperator()
			.setAddress()
			.setData()
			.patterCheck();

		if (flushProc->getResult() == SUCCESS) {
			facade.run(*flushProc);
		}
	}
	mgr.resetAllBuffer();
}

int main(int argc, char** argv) {

#if _DEBUG
	
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();

#else
	
	CommandProcessor* cmdProcess = CommandProcessor::Builder()
		.setParam(argc, argv)
		.setOperator()
		.setAddress()
		.setData()
		.patterCheck();

	if (cmdProcess->getResult() != SUCCESS) {
		cmdProcess->printWriteToOutput(ERROR_STRING);
		return 0;
	}

	int type = cmdProcess->getOperator();

	SsdFacade& ssdFacade = SsdFacade::getInstance();
	const std::string testDir = std::filesystem::current_path().string() + "/buffer";
	BufferManager mgr(testDir);
	std::string value;

	switch (type) {
	case WRITE_OPERATION:
		if (!mgr.addWrite(cmdProcess->getAddress(), cmdProcess->getInputValue())) {
			flushAndReset(mgr, ssdFacade);
			mgr.addWrite(cmdProcess->getAddress(), cmdProcess->getInputValue());
		}
		break;

	case ERASE_OPERATION:
		if (!mgr.addErase(cmdProcess->getAddress(), cmdProcess->getSize())) {
			flushAndReset(mgr, ssdFacade);
			mgr.addErase(cmdProcess->getAddress(), cmdProcess->getSize());
		}
		break;

	case READ_OPERATION:
		value = mgr.addRead(cmdProcess->getAddress());
		if (value == EMPTY_STRING) {
			ssdFacade.readSsdIndex(*cmdProcess);
		} else {
			cmdProcess->printWriteToOutput(value);
		}
		break;

	default:
		std::cerr << "[ERROR] Unknown command type\n";
		break;
	}

	return 0;

#endif
}
