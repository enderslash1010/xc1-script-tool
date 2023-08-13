#pragma once
#include "Instruction.h"

class Function
{
	std::string name;
	unsigned int field2;
	unsigned int field4;
	unsigned int field6;
	unsigned int localPoolIndex;
	unsigned int field10;
	unsigned int start;
	unsigned int end;

	std::vector<Instruction> code;

public:
	Function(std::string name, unsigned int field2, unsigned int field4, unsigned int field6, unsigned int localPoolIndex, unsigned int field10, unsigned int start, unsigned int end);

	void addCode(std::vector<Instruction> code);

	std::vector<Instruction> getCode();

	std::string getName();
	unsigned int getField2(), getField4(), getField6(), getLocalPoolIndex(), getField10(), getStart(), getEnd();
};

