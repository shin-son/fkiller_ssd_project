#include "constants.h"
#include "command_factory.h"

std::unique_ptr<ICommand> CommandFactory::createCommand(const std::string& cmdType) {
    if (READ_COMMAND_NAME == cmdType) return std::make_unique<ReadCommand>();
    else if (WRITE_COMMAND_NAME == cmdType) return std::make_unique<WriteCommand>();
    else if (ERASE_COMMAND_NAME == cmdType) return std::make_unique<EraseCommand>();
    else if (ERASE_RANGE_COMMAND_NAME == cmdType) return std::make_unique<EraseRangeCommand>();
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