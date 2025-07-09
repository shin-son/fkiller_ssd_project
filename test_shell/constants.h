#pragma once
// Please define constants which are shared for several class.
// If the constant is used only one class, it should be defined in the class.
#define RETURN_WRITE_DONE				"[Write] Done"
#define INVALID_COMMAND_MSG				"INVALID COMMAND"

#define EXIT_COMMAND_NAME				"exit"

// definition for Test Script2(PartialLBAWrite)
#define TEST_SCRIPT_2_FULL_COMMAND_NAME	 "2_PartialLBAWrite"
#define TEST_SCRIPT_2_SHORT_COMMAND_NAME "2_"
#define TEST_SCRIPT_2_SUCCESS_MSG		 "[TestSecript2 - PartialLBAWrite] Done"
#define TEST_SCRIPT_2_WRITE_FAIL_MSG	 "[TestSecript2 - PartialLBAWrite] Write Fail"
#define TEST_SCRIPT_2_VERIFY_FAIL_MSG	 "[TestSecript2 - PartialLBAWrite] Verify Fail"
#define INPUT_LBA_SEQUENCE				 { 4, 0, 3, 1, 2 }
#define INPUT_DATA_FOR_PARTIAL_LBA_WRITE "0x12341234"
#define LOOP_COUNT_FOR_PARTIAL_LBA_WRITE  30
#define LBA_COUNT_FOR_PARTIAL_LBA_WRITE	 5

#define ERASE_UNIT_LBA_COUNT			 10