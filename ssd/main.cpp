#include "gmock/gmock.h"
#include "ssd_facade.h"
#include <string>
#include "ssd_constants.h"
#include "command_process.h"

int main(int argc, char** argv) {

#if _DEBUG
	
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();

#else
	CommandProcessor cmdProcess;
	if (cmdProcess.process(argc, argv) != SUCCESS) cmdProcess.printErrorAndWriteToOutput();

	int type = cmdProcess.getOperator();

	SsdFacade& ssdFacade = SsdFacade::getInstance();

	if (type == WRITE_OPERATION) { // Write operation
		ssdFacade.writeSsdIndex(cmdProcess);
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
