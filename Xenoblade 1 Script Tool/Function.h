#pragma once
#include "Instruction.h"
#include <vector>
#include "Object.h"

class Function
{
	std::string name;
	unsigned int args;
	unsigned int field4;
	unsigned int field6;
	std::vector<Object> localPool;
	unsigned int localPoolIndex;
	unsigned int field10;
	unsigned int start;
	unsigned int end;

	std::vector<Instruction> code;

public:
	Function(std::string name, unsigned int args, unsigned int field4, unsigned int field6, std::vector<Object> localPool, unsigned int localPoolIndex, unsigned int field10, unsigned int start, unsigned int end);

	void addCode(std::vector<Instruction> code);
	void setStart(int start);
	void setEnd(int end);

	std::vector<Instruction> getCode();
	std::vector<unsigned char> getRawCode();

	std::string getName();
	unsigned int getArgs(), getField4(), getField6(), getLocalPoolIndex(), getField10(), getStart(), getEnd();
	void setLocalPoolIndex(unsigned int index);
	std::vector<Object> getLocalPool();
};

