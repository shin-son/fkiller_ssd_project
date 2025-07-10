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
        Builder& setOperator(const string & param) {
            string cmd = param;

            if (isWriteCommand(cmd)) {
                cmdProc->ssdOperator = WRITE_OPERATION;
            }
            else if (isReadCommand(cmd)) {
                cmdProc->ssdOperator = READ_OPERATION;
            }
            else if (isEraseCommand(cmd)) {
                cmdProc->ssdOperator = ERASE_OPERATION;
            }
            else if (isFlushCommand(cmd)) {
                cmdProc->ssdOperator = FLUSH_OPERATION;
            }
            else {
                cmdProc->ssdOperator = INIT_OPERATION;
            }
            return *this;
        }

        Builder& setAddress(const string& param) {
            cmdProc->address = param;
            return *this;
        }

        Builder& setData(const string& param) {
            cmdProc->data = param;
            return *this;
        }

        CommandProcessor* patternCheck() {
            if (cmdProc->ssdOperator == INIT_OPERATION) {
                std::cout << "Usage: ssd.exe [w/r] [args...]\n";
                cmdProc->result = INVALID_COMMAND;
            }

            if (cmdProc->ssdOperator == FLUSH_OPERATION) return cmdProc;

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

        bool isFlushCommand(const string& cmd) {
            return cmd == "f" || cmd == "F";
        }

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