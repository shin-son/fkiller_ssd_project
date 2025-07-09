#include "gmock/gmock.h"
#include "SsdFacade.h"
#include <string>
#include "ssd_constants.h"
#include "command_process.h"

int main(int argc, char** argv) {

#if _DEBUG
	
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();

#else
	CommandProcessor cmdProcess;
	if (cmdProcess.process(argc, argv) == SUCCESS) {
		int type = cmdProcess.getOperator();
		int index = cmdProcess.getAddress();
		std::string target = cmdProcess.getInputValue();

		SsdFacade& ssdFacade = SsdFacade::getInstance();

		if (type == WRITE_OPERATION) { // Write operation
			ssdFacade.writeSsdIndex(index, target);
		}
		else if (type == READ_OPERATION) { // Read operation
			ssdFacade.readSsdIndex(index);
		}
		else {
			// Handle other types or errors
		}
	}
	else {
		cmdProcess.printErrorAndWriteToOutput();
	}
	return 0;

#endif
}
