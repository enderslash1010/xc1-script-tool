#include "Function.h"
#include <iostream>
#include <vector>

Function::Function(std::string name, unsigned int args, unsigned int field4, unsigned int field6, std::vector<Object> localPool, unsigned int localPoolIndex, unsigned int field10, unsigned int start, unsigned int end)
{
	this->name = name;
	this->args = args;
	this->field4 = field4;
	this->field6 = field6;
	this->localPool = localPool;
	this->localPoolIndex = localPoolIndex;
	this->field10 = field10;
	this->start = start;
	this->end = end;
}

void Function::addCode(std::vector<Instruction> code) 
{
	this->code = code;
}

void Function::setStart(int start)
{
	this->start = start;
}

void Function::setEnd(int end)
{
	this->end = end;
}

std::vector<Instruction> Function::getCode() 
{
	return this->code;
}

std::vector<unsigned char> Function::getRawCode()
{
	std::vector<unsigned char> rawCode;
	for (Instruction i : this->code) {
		std::vector<unsigned char> rawInstruction = i.getRawInstruction();
		rawCode.insert(rawCode.end(), rawInstruction.begin(), rawInstruction.end());
	}
	return rawCode;
}

std::string Function::getName()
{
	return this->name;
}

unsigned int Function::getArgs()
{
	return this->args;
}

unsigned int Function::getField4()
{
	return this->field4;
}

unsigned int Function::getField6()
{
	return this->field6;
}

std::vector<Object> Function::getLocalPool() 
{
	return this->localPool;
}

unsigned int Function::getLocalPoolIndex()
{
	return this->localPoolIndex;
}

// Used when generating a script file when creating the local pool
void Function::setLocalPoolIndex(unsigned int index) {
	this->localPoolIndex = index;
}

unsigned int Function::getField10()
{
	return this->field10;
}

unsigned int Function::getStart()
{
	return this->start;
}

unsigned int Function::getEnd()
{
	return this->end;
}
