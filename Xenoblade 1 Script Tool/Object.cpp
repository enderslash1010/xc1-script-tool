#include "Object.h"
#include <string>

using namespace std;

Object::Object(string type, unsigned int length, unsigned int value, unsigned int field8)
{
	this->type = type;
	this->length = length;
	this->value = value;
	this->field8 = field8;
}

string Object::getType()
{
	return this->type;
}

unsigned int Object::getLength()
{
	return this->length;
}

unsigned int Object::getValue()
{
	return this->value;
}

unsigned int Object::getField8()
{
	return this->field8;
}
