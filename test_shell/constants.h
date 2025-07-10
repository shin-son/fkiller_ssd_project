#pragma once
// Please define constants which are shared for several class.
// If the constant is used only one class, it should be defined in the class.

// definition for Test Shell Runner
#define CONVERSATION_MODE				1
#define TEST_SCRIPT_MODE				2

// shell command
#define SSD_SIZE							100
#define HELP_COMMAND_NAME					"help"
#define EXIT_COMMAND_NAME					"exit"

// SSD command
#define READ_COMMAND_NAME					"read"
#define WRITE_COMMAND_NAME					"write"
#define ERASE_COMMAND_NAME					"erase"
#define ERASE_RANGE_COMMAND_NAME			"erase_range"
#define FULL_READ_COMMAND_NAME				"fullread"
#define FULL_WRITE_COMMAND_NAME				"fullwrite"

#define TEST_SCRIPT_1_FULL_COMMAND_NAME		"1_FullWriteAndReadCompare"
#define TEST_SCRIPT_1_SHORT_COMMAND_NAME	"1_"
#define TEST_SCRIPT_2_FULL_COMMAND_NAME		"2_PartialLBAWrite"
#define TEST_SCRIPT_2_SHORT_COMMAND_NAME	"2_"
#define TEST_SCRIPT_3_FULL_COMMAND_NAME		"3_WriteReadAging"
#define TEST_SCRIPT_3_SHORT_COMMAND_NAME	"3_"
#define TEST_SCRIPT_4_FULL_COMMAND_NAME		"4_EraseAndWriteAging"
#define TEST_SCRIPT_4_SHORT_COMMAND_NAME	"4_"

#define INVALID_COMMAND_MSG				"INVALID COMMAND"
#define ERROR							"ERROR"
#define DONE							"Done"
// Write 
#define WRITE_DONE_RETURN				"[Write] Done"
#define WRITE_ERROR_RETURN				"[Write] ERROR"

// Erase
#define ERASE_DONE_RETURN		 		"[Erase] Done"
#define ERASE_FAIL_RETURN		 		"[Erase] ERROR"
#define ERASE_UNIT_LBA_COUNT			 10

// Erase
#define ERASE_RANGE_DONE_RETURN		 		"[EraseRange] Done"
#define ERASE_RANGE_FAIL_RETURN		 		"[EraseRange] ERROR"

// WriteReadAging
#define WRITE_READ_AGIING_DONE_RETURN		"[WriteReadAging] Done"

// Test Script1 (FullWriteREadCompare)
#define LBA_COUNT_FOR_FULL_WRC				5
#define LOOP_COUNT_FOR_FULL_WRC				20

// Test Script2 (PartialLBAWrite)

#define TEST_SCRIPT_2_SUCCESS_MSG			"[PartialLBAWrite] Done"
#define TEST_SCRIPT_2_WRITE_FAIL_MSG		"[PartialLBAWrite] ERROR: Write Fail"
#define TEST_SCRIPT_2_VERIFY_FAIL_MSG		"[PartialLBAWrite] ERROR: Verify Fail"
#define INPUT_LBA_SEQUENCE					{ 4, 0, 3, 1, 2 }
#define INPUT_DATA_FOR_PARTIAL_LBA_WRITE	"0x12341234"
#define LOOP_COUNT_FOR_PARTIAL_LBA_WRITE	30
#define LBA_COUNT_FOR_PARTIAL_LBA_WRITE		5

// Test Script3 (WriteReadAging)
#define LOOP_COUNT_FOR_WRITE_READ_AGING				200

// Test Script4 (eraseAndWriteAging)
#define LOOP_COUNT_FOR_ERASE_WRITE_AGING			30

enum NEXT_TEST
{
	NEXT_KEEP_GOING = 0,
	NEXT_EXIT = 2,
	NEXT_MAX = 0xFFFFFFFF,
};