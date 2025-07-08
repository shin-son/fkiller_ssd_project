#include "gmock/gmock.h"
#include "SsdFacade.h"
#include <string>
#include "ssd_constants.h"

int main(int argc, char** argv) {

#if _DEBUG
	
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();

#else

	//parameter rule check
	//TBD

	// parameter change type
	int type = READ_OPERATION;
	int index = 2;
	std::string target = "0xabcdabcd";

	SsdFacade& ssdFacade = SsdFacade::getInstance();

	if (type == 0) { // Write operation
		ssdFacade.writeSsdIndex(index, target);
	} 
	else if (type == 1) { // Read operation
		ssdFacade.readSsdIndex(index);
	} 
	else {
		// Handle other types or errors
	}

	return 0;

#endif
}
