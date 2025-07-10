#pragma once
#include <memory>
#include <vector>
#include <string>
#include "all_commands.h"

using std::vector;
using std::string;

// FACTORY PATTERN
class CommandFactory 
{
public:
    std::unique_ptr<ICommand> createCommand(const std::string& cmdType);
    vector<string> getAllCommandType();

private:
    static vector<string> ALL_CMD;
};