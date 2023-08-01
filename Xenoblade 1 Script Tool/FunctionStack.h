#pragma once
#include "Object.h"
#include <vector>

class FunctionStack
{
	unsigned int dataOffset;
	unsigned int count;
	vector<Object> localObjects;

public:
	FunctionStack(unsigned int dataOffset, unsigned int count);

	vector<Object> getLocalObjects();
};

