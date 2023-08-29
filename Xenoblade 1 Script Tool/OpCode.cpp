#include "OpCode.h"
#include <unordered_map>
#include <string>
#include <stdexcept>

const std::string OpCode::OpCodeStrings[] = { 
    "NOP",
    "CONST_0",
    "CONST_1",
    "CONST_2",
    "CONST_3",
    "CONST_4",
    "CONST_I",
    "CONST_I_W",
    "POOL_INT",
    "POOL_INT_W",
    "POOL_FIXED",
    "POOL_FIXED_W",
    "POOL_STR",
    "POOL_STR_W",
    "LD",
    "ST",
    "LD_ARG",
    "ST_ARG",
    "ST_ARG_OMIT",
    "LD_0",
    "LD_1",
    "LD_2",
    "LD_3",
    "ST_0",
    "ST_1",
    "ST_2",
    "ST_3",
    "LD_ARG_0",
    "LD_ARG_1",
    "LD_ARG_2",
    "LD_ARG_3",
    "ST_ARG_0",
    "ST_ARG_1",
    "ST_ARG_2",
    "ST_ARG_3",
    "LD_STATIC",
    "LD_STATIC_W",
    "ST_STATIC",
    "ST_STATIC_W",
    "LD_AR",
    "ST_AR",
    "LD_NIL",
    "LD_TRUE",
    "LD_FALSE",
    "LD_FUNC",
    "LD_FUNC_W",
    "LD_PLUGIN",
    "LD_PLUGIN_W",
    "LD_FUNC_FAR",
    "LD_FUNC_FAR_W",
    "MINUS",
    "NOT",
    "L_NOT",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "MOD",
    "OR",
    "AND",
    "R_SHIFT",
    "L_SHIFT",
    "EQ",
    "NE",
    "GT",
    "LT",
    "GE",
    "LE",
    "L_OR",
    "L_AND",
    "JMP",
    "JPF",
    "CALL",
    "CALL_W",
    "CALL_IND",
    "RET",
    "NEXT",
    "PLUGIN",
    "PLUGIN_W",
    "CALL_FAR",
    "CALL_FAR_W",
    "GET_OC",
    "GET_OC_W",
    "GETTER",
    "GETTER_W",
    "SETTER",
    "SETTER_W",
    "SEND",
    "SEND_W",
    "TYPEOF",
    "SIZEOF",
    "SWITCH",
    "INC",
    "DEC",
    "EXIT",
    "BP"
};

// Expected operand size in bytes
const unsigned int OpCode::OperandSizes[] = {
    0, // NOP
    0, // CONST_0
    0, // CONST_1
    0, // CONST_2
    0, // CONST_3
    0, // CONST_4
    1, // CONST_I
    2, // CONST_I_W
    1, // POOL_INT
    2, // POOL_INT_W
    1, // POOL_FIXED
    2, // POOL_FIXED_W
    1, // POOL_STR
    2, // POOL_STR_W
    1, // LD
    1, // ST
    1, // LD_ARG
    1, // ST_ARG
    1, // ST_ARG_OMIT
    0, // LD_0
    0, // LD_1
    0, // LD_2
    0, // LD_3
    0, // ST_0
    0, // ST_1
    0, // ST_2
    0, // ST_3
    0, // LD_ARG_0
    0, // LD_ARG_1
    0, // LD_ARG_2
    0, // LD_ARG_3
    0, // ST_ARG_0
    0, // ST_ARG_1
    0, // ST_ARG_2
    0, // ST_ARG_3
    1, // LD_STATIC
    2, // LD_STATIC_W
    1, // ST_STATIC
    2, // ST_STATIC_W
    0, // LD_AR
    0, // ST_AR
    0, // LD_NIL
    0, // LD_TRUE
    0, // LD_FALSE
    1, // LD_FUNC
    2, // LD_FUNC_W
    1, // LD_PLUGIN
    2, // LD_PLUGIN_W
    1, // LD_FUNC_FAR
    2, // LD_FUNC_FAR_W
    0, // MINUS
    0, // NOT
    0, // L_NOT
    0, // ADD
    0, // SUB
    0, // MUL
    0, // DIV
    0, // MOD
    0, // OR
    0, // AND
    0, // R_SHIFT
    0, // L_SHIFT
    0, // EQ
    0, // NE
    0, // GT
    0, // LT
    0, // GE
    0, // LE
    0, // L_OR
    0, // L_AND
    2, // JMP
    2, // JPF
    1, // CALL
    2, // CALL_W
    0, // CALL_IND
    0, // RET
    0, // NEXT
    1, // PLUGIN
    2, // PLUGIN_W
    1, // CALL_FAR
    2, // CALL_FAR_W
    1, // GET_OC
    2, // GET_OC_W
    1, // GETTER
    2, // GETTER_W
    1, // SETTER
    2, // SETTER_W
    1, // SEND
    2, // SEND_W
    0, // TYPEOF
    0, // SIZEOF
    1, // SWITCH
    0, // INC
    0, // DEC
    0, // EXIT
    0, // BP
};

int OpCode::stringToOpCode(std::string str) {
    int opcodeVal = -1;
    for (int i = 0; i < 96; i++) {
        if (strcmp(OpCode::OpCodeStrings[i].c_str(), str.c_str()) == 0) {
            return i;
            break;
        }
    }
    return opcodeVal;
}

OpCode::OpCode(unsigned int opcodeVal)
{
	this->opcodeVal = opcodeVal;
}

OpCode::OpCode(std::string opcodeString)
{
    this->opcodeVal = stringToOpCode(opcodeString);
    if (this->opcodeVal == -1) throw std::runtime_error(opcodeString + " is not a valid opcode");
}

unsigned int OpCode::getOperandSize()
{
    return OperandSizes[this->opcodeVal];
}

std::string OpCode::getOpCodeString()
{
    return OpCodeStrings[this->opcodeVal];
}

int OpCode::getOpCodeVal()
{
    return this->opcodeVal;
}

unsigned int OpCode::getOperandSize(OpCode::OpCodes oc)
{
    return OperandSizes[oc];
}

unsigned int OpCode::getOperandSize(std::string str)
{
    int opcodeVal = stringToOpCode(str);
    if (opcodeVal == -1) throw std::runtime_error(str + " is not a valid opcode");
    return OpCode::OperandSizes[opcodeVal];
}
