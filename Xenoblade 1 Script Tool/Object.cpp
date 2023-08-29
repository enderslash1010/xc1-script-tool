#include "Object.h"

Object::Object(Object::Type typeEnum, bool inArray, unsigned int length, int value, unsigned int field8)
{
	this->typeEnum = typeEnum;
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
	this->inArray = inArray;
	this->value = value;
	this->field8 = field8;
}

Object::Object(std::string type, bool inArray, unsigned int length, int value, unsigned int field8)
{
	this->type = type;
	if (strcmp("Null", type.c_str()) == 0) this->typeEnum = Type::Null;
	else if (strcmp("True", type.c_str()) == 0) this->typeEnum = Type::True;
	else if (strcmp("False", type.c_str()) == 0) this->typeEnum = Type::False;
	else if (strcmp("Int", type.c_str()) == 0) this->typeEnum = Type::Int;
	else if (strcmp("Fixed", type.c_str()) == 0) this->typeEnum = Type::Fixed;
	else if (strcmp("String", type.c_str()) == 0) this->typeEnum = Type::String;
	else if (strcmp("Array", type.c_str()) == 0) this->typeEnum = Type::Array;
	else if (strcmp("Function", type.c_str()) == 0) this->typeEnum = Type::Function;
	else if (strcmp("Plugin", type.c_str()) == 0) this->typeEnum = Type::Plugin;
	else if (strcmp("OC", type.c_str()) == 0) this->typeEnum = Type::OC;
	else if (strcmp("Sys", type.c_str()) == 0) this->typeEnum = Type::Sys;

	this->inArray = inArray;
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

int Object::getValue()
{
	return this->value;
}

unsigned int Object::getField8()
{
	return this->field8;
}

bool Object::isInArray()
{
	return this->inArray;
}
