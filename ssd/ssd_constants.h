#pragma once
#include <string>

static const int INIT_OPERATION = -1;
static const int WRITE_OPERATION = 0;
static const int READ_OPERATION = 1;
static const int ERASE_OPERATION = 2;
static const int FLUSH_OPERATION = 3;

static const int SUCCESS = 0;
static const int INVALID_COMMAND = -1;
static const int INVALID_ARGUMENT = -2;

const std::string ERROR_STRING = "ERROR";

static const int ALL_BUFFER_USED = -1;
static const int NOT_FOUND_ANY_BUFFER = -1;
const std::string EMPTY_STRING = "";
const std::string INIT_STRING = "0x00000000";
