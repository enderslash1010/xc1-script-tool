#pragma once
#include "OpCode.h"
#include <vector>
#include <map>

class Instruction
{
public:

	Instruction(OpCode oc, unsigned int operand);
    Instruction(OpCode oc);
    Instruction(OpCode oc, unsigned int operand, std::map<int, unsigned int> switchCases, unsigned int defaultCase);

    OpCode getOpCode();
    int getOperand();

    std::map<int, unsigned int> getSwitchCases();
    unsigned int getDefaultCase();

    std::vector<unsigned char> getRawInstruction();

    private:
        OpCode opCode;
        int operand;

        // Only used for SWITCH Instructions
        std::map<int, unsigned int> switchCases;
        unsigned int defaultCase;
};
