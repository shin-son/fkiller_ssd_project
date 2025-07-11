#include "ssd_erase.h"
#include "ssd_constants.h"

void SsdErase::eraseSsdNandData(int startIndex, int size) {

	for (int index = startIndex; index < startIndex + size; ++index) {
		getSsdNandData()[index] = INIT_STRING;
	}
}
