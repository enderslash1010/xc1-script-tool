#include "Function.h"
#include <string>

Function::Function(string name, unsigned int field2, unsigned int field4, unsigned int field6, unsigned int localPoolIndex, unsigned int field10, unsigned int start, unsigned int end)
{
	this->name = name;
	this->field2 = field2;
	this->field4 = field4;
	this->field6 = field6;
	this->localPoolIndex = localPoolIndex;
	this->field10 = field10;
	this->start = start;
	this->end = end;
}

string Function::getName()
{
	return this->name;
}

unsigned int Function::getField2()
{
	return this->field2;
}

unsigned int Function::getField4()
{
	return this->field4;
}

unsigned int Function::getField6()
{
	return this->field6;
}

unsigned int Function::getLocalPoolIndex()
{
	return this->localPoolIndex;
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
