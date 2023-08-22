#include "Instruction.h"
#include "OpCode.h"
#include <stdexcept>
#include <vector>
#include <map>

Instruction::Instruction(OpCode oc, unsigned int operand)
{
    if (oc.getOperandSize() == 0) throw std::runtime_error("Expecting Instruction with operands, got " + oc.getOpCodeString());
    if (oc.getOpCodeVal() == OpCode::OpCodes::SWITCH) throw std::runtime_error("Expecting non-SWITCH Instruction");

    this->opCode = oc;
    this->operand = operand;
    this->defaultCase = -1;
}

Instruction::Instruction(OpCode oc)
{
    if (oc.getOperandSize() != 0) throw std::runtime_error("Expecting Instruction with no operand, got " + oc.getOpCodeString());
    if (oc.getOpCodeVal() == OpCode::OpCodes::SWITCH) throw std::runtime_error("Expecting non-SWITCH Instruction");

    this->opCode = oc;
    this->operand = -1;
    this->defaultCase = -1;
}

// For switch instructions
Instruction::Instruction(OpCode oc, unsigned int operand, std::map<int, unsigned int> switchCases, unsigned int defaultCase) {
    if (oc.getOpCodeVal() != OpCode::OpCodes::SWITCH) throw std::runtime_error("Expecting SWITCH Instruction, got " + oc.getOpCodeString());
    
    this->opCode = oc;
    this->operand = operand;
    this->switchCases = switchCases;
    this->defaultCase = defaultCase;
}

OpCode Instruction::getOpCode()
{
    return this->opCode;
}

int Instruction::getOperand()
{
    return this->operand;
}

std::map<int, unsigned int> Instruction::getSwitchCases()
{
    if (this->getOpCode().getOpCodeVal() != OpCode::OpCodes::SWITCH) throw std::runtime_error("Cannot call getSwitchCases() on non-SWITCH Instruction");
    return this->switchCases;
}

unsigned int Instruction::getDefaultCase()
{
    if (this->getOpCode().getOpCodeVal() != OpCode::OpCodes::SWITCH) throw std::runtime_error("Cannot call getDefaultCase() on non-SWITCH Instruction");
    return this->defaultCase;
}

std::vector<unsigned char> Instruction::getRawInstruction() {
    std::vector<unsigned char> rawInstruction;
    // Add opcode
    rawInstruction.push_back(this->opCode.getOpCodeVal());

    // Add operand, if there is one
    if (this->operand != -1) {
        unsigned int operandSize = this->opCode.getOperandSize();
        for (int i = operandSize - 1; i >= 0; i--) {
            rawInstruction.push_back(this->operand >> (i*8));
        }

        // More operands for SWITCH Opcode
        if (this->opCode.getOpCodeVal() == OpCode::OpCodes::SWITCH) {
            // Add default case
            for (int i = 3; i >= 0; i--) rawInstruction.push_back(this->defaultCase >> (i*8));
            // Add rest of cases
            for (auto it = this->switchCases.begin(); it != this->switchCases.end(); ++it) {
                for (int i = 3; i >= 0; i--) rawInstruction.push_back(it->first >> (i * 8));
                for (int i = 3; i >= 0; i--) rawInstruction.push_back(it->second >> (i * 8));
            }
        }

    }

    return rawInstruction;
}
