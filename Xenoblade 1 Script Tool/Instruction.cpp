#include "Instruction.h"
#include "OpCode.h"
#include <stdexcept>
#include <map>

Instruction::Instruction(OpCode oc, unsigned int operand)
{
    if (oc.getOperandSize() == 0) throw std::runtime_error("Expecting Instruction with operands, got " + oc.getOpCodeString());
    if (oc.getOpCodeVal() == OpCode::OpCodes::SWITCH) throw std::runtime_error("Expecting non-SWITCH Instruction");

    this->opCode = oc;
    this->operand = operand;
}

Instruction::Instruction(OpCode oc)
{
    if (oc.getOperandSize() != 0) throw std::runtime_error("Expecting Instruction with no operand, got " + oc.getOpCodeString());
    if (oc.getOpCodeVal() == OpCode::OpCodes::SWITCH) throw std::runtime_error("Expecting non-SWITCH Instruction");

    this->opCode = oc;
    this->operand = -1;
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
