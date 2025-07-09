#pragma once
#include <string>

static const int WRITE_OPERATION = 0;
static const int READ_OPERATION = 1;
static const int ERASE_OPERATION = 2;

static const int SUCCESS = 0;
static const int INVALID_COMMAND = -1;
static const int INVALID_ARGUMENT = -2;

const std::string ERROR_STRING = "ERROR";

static const int ALL_BUFFER_USED = -1;