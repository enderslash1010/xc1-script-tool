#pragma once
#include <string>

class Object
{
public:
	enum Type {
		Null, True, False, Int, Fixed, String, Array, Function, Plugin, OC, Sys
	};

private:
	std::string type;
	Type typeEnum;
	bool inArray;
	unsigned int length;
	int value;
	unsigned int field8; // only in 64-bit scripts, for pointer storage space

public:
	Object(Type typeEnum, bool inArray, unsigned int length, int value, unsigned int field8);
	Object(std::string type, bool inArray, unsigned int length, int value, unsigned int field8);

	std::string getType();
	Type getTypeEnum();
	unsigned int getLength(), getField8();
	int getValue();
	bool isInArray();
};

