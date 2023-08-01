#pragma once
#include <string>

using namespace std;

class Object
{
	string type;
	unsigned int length;
	unsigned int value;
	unsigned int field8; // only in 64-bit scripts, for pointer storage space

public:
	Object(string type, unsigned int length, unsigned int value, unsigned int field8);

	string getType();
	unsigned int getLength(), getValue(), getField8();
};

