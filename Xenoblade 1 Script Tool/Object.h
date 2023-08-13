#pragma once
#include <string>

class Object
{
	std::string type;
	unsigned int length;
	unsigned int value;
	unsigned int field8; // only in 64-bit scripts, for pointer storage space

public:
	Object(std::string type, unsigned int length, unsigned int value, unsigned int field8);

	std::string getType();
	unsigned int getLength(), getValue(), getField8();
};

