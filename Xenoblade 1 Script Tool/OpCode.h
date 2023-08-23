#pragma once
#include <string>

class OpCode
{
	unsigned int opcodeVal;

	static const std::string OpCodeStrings[96];
	static const unsigned int OperandSizes[96];

public:
	OpCode(unsigned int opcodeVal);
	OpCode(std::string opcode);
	OpCode() = default;

    const enum OpCodes {
        NOP,
        CONST_0,
        CONST_1,
        CONST_2,
        CONST_3,
        CONST_4,
        CONST_I,
        CONST_I_W,
        POOL_INT,
        POOL_INT_W,
        POOL_FIXED,
        POOL_FIXED_W,
        POOL_STR,
        POOL_STR_W,
        LD,
        ST,
        LD_ARG,
        ST_ARG,
        ST_ARG_OMIT,
        LD_0,
        LD_1,
        LD_2,
        LD_3,
        ST_0,
        ST_1,
        ST_2,
        ST_3,
        LD_ARG_0,
        LD_ARG_1,
        LD_ARG_2,
        LD_ARG_3,
        ST_ARG_0,
        ST_ARG_1,
        ST_ARG_2,
        ST_ARG_3,
        LD_STATIC,
        LD_STATIC_W,
        ST_STATIC,
        ST_STATIC_W,
        LD_AR,
        ST_AR,
        LD_NIL,
        LD_TRUE,
        LD_FALSE,
        LD_FUNC,
        LD_FUNC_W,
        LD_PLUGIN,
        LD_PLUGIN_W,
        LD_FUNC_FAR,
        LD_FUNC_FAR_W,
        MINUS,
        NOT,
        L_NOT,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        OR,
        AND,
        R_SHIFT,
        L_SHIFT,
        EQ,
        NE,
        GT,
        LT,
        GE,
        LE,
        L_OR,
        L_AND,
        JMP,
        JPF,
        CALL,
        CALL_W,
        CALL_IND,
        RET,
        NEXT,
        PLUGIN,
        PLUGIN_W,
        CALL_FAR,
        CALL_FAR_W,
        GET_OC,
        GET_OC_W,
        GETTER,
        GETTER_W,
        SETTER,
        SETTER_W,
        SEND,
        SEND_W,
        TYPEOF,
        SIZEOF,
        SWITCH,
        INC,
        DEC,
        EXIT,
        BP
    };

	unsigned int getOperandSize();
	std::string getOpCodeString();
	unsigned int getOpCodeVal();
};

