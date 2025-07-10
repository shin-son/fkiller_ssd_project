#pragma once
#include <memory>
#include "all_commands.h"

// FACTORY PATTERN
class CommandFactory 
{
public:
    static std::unique_ptr<ICommand> createCommand(const std::string& cmdType);
};