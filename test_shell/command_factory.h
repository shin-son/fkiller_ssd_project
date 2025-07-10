#pragma once
#include <memory>
#include <string>
#include <list>
#include "constants.h"
#include "all_commands.h"

using std::string;
using std::list;

class CommandFactory 
{
public:
    std::unique_ptr<ICommand> createCommand(const std::string& cmdType);
    list<string> getAllCommandType();

private:
    static const list<string> ALL_CMD_LIST;
};