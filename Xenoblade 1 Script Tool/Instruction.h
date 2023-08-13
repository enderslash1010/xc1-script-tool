#pragma once
#include "OpCode.h"
#include <unordered_map>

class Instruction
{
public:

	Instruction(OpCode oc, unsigned int operand);
    Instruction(OpCode oc);
    Instruction(OpCode oc, unsigned int operand, std::unordered_map<int, unsigned int> switchCases, unsigned int defaultCase);

    OpCode getOpCode();
    int getOperand();

    std::unordered_map<int, unsigned int> getSwitchCases();
    unsigned int getDefaultCase();

    private:
        OpCode opCode;
        int operand;

        // Only used for SWITCH Instructions
        std::unordered_map<int, unsigned int> switchCases;
        unsigned int defaultCase;
};
