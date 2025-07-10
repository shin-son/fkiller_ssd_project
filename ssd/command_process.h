#pragma once
#include <string>
#include <vector>
#include "pattern_checker.h"
#include "ssd_constants.h"
#include <iostream>

using std::string;
using std::vector;

class CommandProcessor {
public:

    class Builder {
    
    public:
        Builder& setParam(int argc, char* argv[]) {
            argCount = argc;
            argsVector = { argv + 0, argv + argc };
            return *this;
        }

        Builder& setParam(vector<string>& args) {
            argCount = 4;
            argsVector = args;
            return *this;
        }

        Builder& setOperator() {
            if (argCount >= 2) {
                string cmd = argsVector[1];

                if (isWriteCommand(cmd)) {
                    cmdProc->ssdOperator = WRITE_OPERATION;
                }
                if (isReadCommand(cmd)) {
                    cmdProc->ssdOperator = READ_OPERATION;
                }
                if (isEraseCommand(cmd)) {
                    cmdProc->ssdOperator = ERASE_OPERATION;
                }
            }
            return *this;
        }

        Builder& setAddress() {
            if (argCount >= 3) {
                cmdProc->address = argsVector[2];
            }
            return *this;
        }

        Builder& setData() {
            if (argCount >= 4) {
                cmdProc->data = argsVector[3];
            }
            return *this;
        }

        CommandProcessor* patterCheck() {
            if (argCount < 2) {
                std::cout << "Usage: ssd.exe [w/r] [args...]\n";
                cmdProc->result = INVALID_COMMAND;
            }

            if (!patternChecker.isValidAddress(cmdProc->address)) {
                cmdProc->result = INVALID_ARGUMENT;
            }

            if (cmdProc->ssdOperator == WRITE_OPERATION) {
                if (!patternChecker.isValidMemoryValue(cmdProc->data)) {
                    cmdProc->result = INVALID_ARGUMENT;
                }
            }

            if (cmdProc->ssdOperator == ERASE_OPERATION) {
                if (!patternChecker.isValidSize(cmdProc->data)) {
                    cmdProc->result = INVALID_ARGUMENT;
                }
            }            

            return cmdProc;
        }

    private:
        bool isWriteCommand(const string& cmd) {
            return cmd == "w" || cmd == "W";
        }

        bool isReadCommand(const string& cmd) {
            return cmd == "r" || cmd == "R";
        }

        bool isEraseCommand(const string& cmd) {
            return cmd == "e" || cmd == "E";
        }

        int argCount;
        vector<string> argsVector;
        
        PatternChecker patternChecker;

        CommandProcessor * cmdProc = new CommandProcessor();        
    };

    int getOperator();
    int getAddress();
    std::string getInputValue();
    int getSize();
    int getResult();

    void printWriteToOutput(std::string value);

private:
    CommandProcessor() : result(SUCCESS) {};

    int ssdOperator;
    string address;
    string data;
    int result;
};