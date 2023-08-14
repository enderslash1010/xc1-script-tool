#pragma once
#include "Instruction.h"
#include <vector>

class Function
{
	std::string name;
	unsigned int args;
	unsigned int field4;
	unsigned int field6;
	unsigned int localPoolIndex;
	unsigned int field10;
	unsigned int start;
	unsigned int end;

	std::vector<Instruction> code;

public:
	Function(std::string name, unsigned int args, unsigned int field4, unsigned int field6, unsigned int localPoolIndex, unsigned int field10, unsigned int start, unsigned int end);

	void addCode(std::vector<Instruction> code);

	std::vector<Instruction> getCode();

	std::string getName();
	unsigned int getArgs(), getField4(), getField6(), getLocalPoolIndex(), getField10(), getStart(), getEnd();
};

