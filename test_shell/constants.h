#pragma once
// Please define constants which are shared for several class.
// If the constant is used only one class, it should be defined in the class.
#define HELP_COMMAND_NAME					"help"
#define EXIT_COMMAND_NAME					"exit"
#define READ_COMMAND_NAME					"read"
#define WRITE_COMMAND_NAME					"write"
#define ERASE_COMMAND_NAME					"erase"
#define ERASE_RANGE_COMMAND_NAME			"erase_range"

#define TEST_SCRIPT_1_FULL_COMMAND_NAME		"1_FullWriteAndReadCompare"
#define TEST_SCRIPT_1_SHORT_COMMAND_NAME	"1_"
#define TEST_SCRIPT_2_FULL_COMMAND_NAME		"2_PartialLBAWrite"
#define TEST_SCRIPT_2_SHORT_COMMAND_NAME	"2_"
#define TEST_SCRIPT_3_FULL_COMMAND_NAME		"3_WriteReadAging"
#define TEST_SCRIPT_3_SHORT_COMMAND_NAME	"3_"
#define TEST_SCRIPT_4_FULL_COMMAND_NAME		"4_EraseAndWriteAging"
#define TEST_SCRIPT_4_SHORT_COMMAND_NAME	"4_"

#define INVALID_COMMAND_MSG				"INVALID COMMAND"


// Write 
#define WRITE_DONE_RETURN				"[Write] Done"
#define WRITE_ERROR_RETURN				"[Write] ERROR"

// Erase
#define ERASE_DONE_RETURN		 		"[Erase] Done"
#define ERASE_FAIL_RETURN		 		"[Erase] ERROR"
#define ERASE_UNIT_LBA_COUNT			 10

// Erase
#define ERASE_RANGE_DONE_RETURN		 		"[Erase_Range] Done"
#define ERASE_RANGE_FAIL_RETURN		 		"[Erase_Range] ERROR"

// definition for Test Shell Runner
#define CONVERSATION_MODE				1
#define TEST_SCRIPT_MODE				2

// Test Script2 (PartialLBAWrite)

#define TEST_SCRIPT_2_SUCCESS_MSG			"[TestSecript2 - PartialLBAWrite] Done"
#define TEST_SCRIPT_2_WRITE_FAIL_MSG		"[TestSecript2 - PartialLBAWrite] Write Fail"
#define TEST_SCRIPT_2_VERIFY_FAIL_MSG		"[TestSecript2 - PartialLBAWrite] Verify Fail"
#define INPUT_LBA_SEQUENCE					{ 4, 0, 3, 1, 2 }
#define INPUT_DATA_FOR_PARTIAL_LBA_WRITE	"0x12341234"
#define LOOP_COUNT_FOR_PARTIAL_LBA_WRITE	30
#define LBA_COUNT_FOR_PARTIAL_LBA_WRITE		5

// Test Script4 (eraseAndWriteAging)
#define LOOP_COUNT_FOR_AGING				30

enum TEST_NEXT
{
    NEXT_KEEP_GOING = 0,
    NEXT_EXIT = 2,
    NEXT_MAX = 0xFFFFFFFF,
};