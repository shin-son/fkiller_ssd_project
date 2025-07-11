#include "command_factory.h"

const list<string> CommandFactory::ALL_CMD_LIST = {
    READ_COMMAND_NAME,
    WRITE_COMMAND_NAME,
    ERASE_COMMAND_NAME,
    ERASE_RANGE_COMMAND_NAME,
    FULL_READ_COMMAND_NAME,
    FULL_WRITE_COMMAND_NAME,
    FLUSH_COMMAND_NAME,
    TEST_SCRIPT_1_FULL_COMMAND_NAME,
    TEST_SCRIPT_2_FULL_COMMAND_NAME,
    TEST_SCRIPT_3_FULL_COMMAND_NAME,
    TEST_SCRIPT_4_FULL_COMMAND_NAME,
};

std::unique_ptr<ICommand> CommandFactory::createCommand(const std::string& cmdType) {
    if (READ_COMMAND_NAME == cmdType) return std::make_unique<ReadCommand>();
    else if (WRITE_COMMAND_NAME == cmdType) return std::make_unique<WriteCommand>();
    else if (ERASE_COMMAND_NAME == cmdType) return std::make_unique<EraseCommand>();
    else if (ERASE_RANGE_COMMAND_NAME == cmdType) return std::make_unique<EraseRangeCommand>();
    else if (FULL_READ_COMMAND_NAME == cmdType) return std::make_unique<FullReadCommand>();
    else if (FULL_WRITE_COMMAND_NAME == cmdType) return std::make_unique<FullWriteCommand>();
    else if (FLUSH_COMMAND_NAME == cmdType) return std::make_unique<FlushCommand>();
    else if ((TEST_SCRIPT_1_FULL_COMMAND_NAME == cmdType) || (TEST_SCRIPT_1_SHORT_COMMAND_NAME == cmdType)) 
        return std::make_unique<FullWriteReadCompareCommand>();
    else if ((TEST_SCRIPT_2_FULL_COMMAND_NAME == cmdType) || (TEST_SCRIPT_2_SHORT_COMMAND_NAME == cmdType)) 
        return std::make_unique<PartialLbaWriteCommand>();
    else if ((TEST_SCRIPT_3_FULL_COMMAND_NAME == cmdType) || (TEST_SCRIPT_3_SHORT_COMMAND_NAME == cmdType)) 
        return std::make_unique<WriteReadAgingCommand>();
    else if ((TEST_SCRIPT_4_FULL_COMMAND_NAME == cmdType) || (TEST_SCRIPT_4_SHORT_COMMAND_NAME == cmdType)) 
        return std::make_unique<EraseWriteAgingCommand>();
    else
        return nullptr;
}

list<string> CommandFactory::getAllCommandType()
{
    return ALL_CMD_LIST;
}
