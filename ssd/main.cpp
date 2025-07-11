#include "gmock/gmock.h"
#include "ssd_facade.h"
#include <string>
#include "ssd_constants.h"
#include "command_process.h"
#include "buffer_manager.h"
#include <iostream>
#include "ssd_initial_files.h"

#include <filesystem>

int main(int argc, char** argv) {

#if _DEBUG

	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();

#else
	const std::string testDir = std::filesystem::current_path().string() + "\\..\\buffer";
	SsdInitialFiles ssdInitialFiles;
	ssdInitialFiles.initialize(testDir);

	BufferManager mgr;

	vector<string> argsVector;
	for (int index = 0; index < argc; ++index) {
		argsVector.push_back(argv[index]);
	}
	
	for (int index = argc; index < 4; ++index) {
		argsVector.push_back("");
	}

	CommandProcessor* cmdProcess = CommandProcessor::Builder()
		.setOperator(argsVector[1])
		.setAddress(argsVector[2])
		.setData(argsVector[3])
		.patternCheck();

	if (cmdProcess->getResult() != SUCCESS) {
		cmdProcess->printWriteToOutput(ERROR_STRING);
		return 0;
	}

	int type = cmdProcess->getOperator();

	std::string value;

	switch (type) {
	case WRITE_OPERATION:
		{
			mgr.addWrite(cmdProcess->getAddress(), cmdProcess->getInputValue());
			cmdProcess->printWriteToOutput("");
		}
		break;

	case ERASE_OPERATION:
		{
			mgr.addErase(cmdProcess->getAddress(), cmdProcess->getSize());
			cmdProcess->printWriteToOutput("");
		}
		break;

	case READ_OPERATION:
		value = mgr.addRead(cmdProcess->getAddress());
		if (value == EMPTY_STRING) {
			SsdFacade& ssdFacade = SsdFacade::getInstance();
			ssdFacade.readSsdIndex(*cmdProcess);
		} else {
			cmdProcess->printWriteToOutput(value);
		}
		break;

	case FLUSH_OPERATION:
		{
			mgr.flushAndReset();
		}
		break;

	default:
		std::cerr << "[ERROR] Unknown command type\n";
		break;
	}

	return 0;

#endif
}
