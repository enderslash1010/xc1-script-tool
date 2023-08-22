#include "Object.h"

Object::Object(Object::Type type, unsigned int length, unsigned int value, unsigned int field8)
{
	this->typeEnum = type;
	switch (this->typeEnum) {
	case 0: this->type = "Null"; break;
	case 1: this->type = "True"; break;
	case 2: this->type = "False"; break;
	case 3: this->type = "Int"; break;
	case 4: this->type = "Fixed"; break;
	case 5: this->type = "String"; break;
	case 6: this->type = "Array"; break;
	case 7: this->type = "Function"; break;
	case 8: this->type = "Plugin"; break;
	case 9: this->type = "OC"; break;
	case 10: this->type = "Sys"; break;
	}

	this->length = length;
	this->value = value;
	this->field8 = field8;
}

std::string Object::getType()
{
	return this->type;
}

Object::Type Object::getTypeEnum()
{
	return this->typeEnum;
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
