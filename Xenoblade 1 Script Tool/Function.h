#pragma once
#include <string>

using namespace std;

class Function
{
	string name;
	unsigned int field2;
	unsigned int field4;
	unsigned int field6;
	unsigned int localPoolIndex;
	unsigned int field10;
	unsigned int start;
	unsigned int end;

public:
	Function(string name, unsigned int field2, unsigned int field4, unsigned int field6, unsigned int localPoolIndex, unsigned int field10, unsigned int start, unsigned int end);

	string getName();
	unsigned int getField2(), getField4(), getField6(), getLocalPoolIndex(), getField10(), getStart(), getEnd();
};

