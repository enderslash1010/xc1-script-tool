#include "Script.h"
#include <fstream>
#include <string>
#include <iostream>
#include "Function.h"
#include "Object.h"

using namespace std;

union UInteger4 {
	unsigned int uinteger;
	unsigned char bytes[4];
};

union UInteger2 {
	unsigned int uinteger;
	unsigned char bytes[2];
};

union Float {
	float f;
	unsigned char bytes[4];
};

unsigned int Script::getUInteger4(unsigned char* memblock, int start) {
	UInteger4 result{};
	if (this->littleEndian) { // Little Endian
		for (int i = start; i < start + 4; i++) result.bytes[start - i + 3] = memblock[i];
	}
	else { // Big Endian
		for (int i = start; i < start + 4; i++) result.bytes[ i - start ] = memblock[i];
	}
	return result.uinteger;
}

unsigned int Script::getUInteger2(unsigned char* memblock, int start) {
	UInteger2 result{};
	if (this->littleEndian) { // Little Endian
		for (int i = start; i < start + 2; i++) result.bytes[start - i + 1] = memblock[i];
	}
	else { // Big Endian
		for (int i = start; i < start + 2; i++) result.bytes[i - start] = memblock[i];
	}
	return result.uinteger;
}

float Script::getFloat(unsigned char* memblock, int start) {
	Float result{};
	if (this->littleEndian) { // Little Endian
		for (int i = start; i < start + 4; i++) result.bytes[start - i + 3] = memblock[i];
	}
	else { // Big Endian
		for (int i = start; i < start + 4; i++) result.bytes[i - start] = memblock[i];
	}
	return result.f;
}

Script::Script(char* fileName) 
{
	ifstream buffer(fileName, ios::binary | ios::ate);
	if (!buffer.is_open()) throw runtime_error("Unable to open file");

	// Load buffer into char array
	int size = buffer.tellg();
	unsigned char* memblock = new unsigned char[size];
	buffer.seekg(0, ios::beg);
	buffer.read((char*) memblock, size);
	buffer.close();

	// Check file type
	if (size < 63 || memblock[0x0] != 0x53 || memblock[0x1] != 0x42 || memblock[0x2] != 0x20 || memblock[0x3] != 0x20) throw runtime_error("Wrong file type");
	
	// Check Endianness
	int n = 1;
	this->littleEndian = *(char*)&n == 1 ? true : false;

	this->version = memblock[0x4];
	cout << "Version: 0x" << (int) this->version << '\n';

	this->flags = memblock[0x6];
	cout << hex << "Flags: 0x" << (int) this->flags << '\n';
	this->is64Bit = (this->flags & 4) != 0;
	cout << "64 Bit: " << this->is64Bit << '\n';

	this->isLoaded = memblock[0x7];
	cout << hex << "isLoaded: 0x" << (int) this->isLoaded << '\n';

	this->codeOffset = getUInteger4(memblock, 8);
	cout << hex << "Code Offset: 0x" << this->codeOffset << '\n';

	this->IDPoolOffset = getUInteger4(memblock, 12);
	cout << hex << "ID Pool Offset: 0x" << this->IDPoolOffset << '\n';

	this->intPoolOffset = getUInteger4(memblock, 16);
	cout << hex << "Int Pool Offset: 0x" << this->intPoolOffset << '\n';

	this->fixedPoolOffset = getUInteger4(memblock, 20);
	cout << hex << "Fixed Pool Offset: 0x" << this->fixedPoolOffset << '\n';

	this->stringPoolOffset = getUInteger4(memblock, 24);
	cout << hex << "String Pool Offset: 0x" << this->stringPoolOffset << '\n';

	this->functionPoolOffset = getUInteger4(memblock, 28);
	cout << hex << "Function Pool Offset: 0x" << this->functionPoolOffset << '\n';

	this->pluginImportsOffset = getUInteger4(memblock, 32);
	cout << hex << "Plugin Imports Offset: 0x" << this->pluginImportsOffset << '\n';

	this->OCImportsOffset = getUInteger4(memblock, 36);
	cout << hex << "OC Imports Offset: 0x" << this->OCImportsOffset << '\n';

	this->functionImportsOffset = getUInteger4(memblock, 40);
	cout << hex << "Function Imports Offset: 0x" << this->functionImportsOffset << '\n';

	this->staticVariablesOffset = getUInteger4(memblock, 44);
	cout << hex << "Static Variables Offset: 0x" << this->staticVariablesOffset << '\n';

	this->localPoolOffset = getUInteger4(memblock, 48);
	cout << hex << "Local Pool Offset: 0x" << this->localPoolOffset << '\n';

	this->systemAttributePoolOffset = getUInteger4(memblock, 52);
	cout << hex << "System Attribute Pool Offset: 0x" << this->systemAttributePoolOffset << '\n';

	this->userAttributePoolOffset = getUInteger4(memblock, 56);
	cout << hex << "User Attribute Pool Offset: 0x" << this->userAttributePoolOffset << '\n';

	initIDPool(memblock);
	initIntPool(memblock);
	initFixedPool(memblock);
	initStringPool(memblock);
	initFunctionPool(memblock);
	initPluginImports(memblock);
	initOCImports(memblock);
	initFunctionImports(memblock);
	initStaticVariables(memblock);
	initLocalPool(memblock);
	initSystemAttributePool(memblock);
	initUserAttributePool(memblock);
	initCode(memblock);
}

void Script::initCode(unsigned char *memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->codeOffset);
	unsigned int length = getUInteger4(memblock, this->codeOffset + 8);

	unsigned int pc = this->codeOffset + dataOffset;
	unsigned int codeStart = pc;

	// TODO: find a way to represent each code instruction
	while (pc < codeStart + length) {
		pc++;
	}
}

void Script::initIDPool(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->IDPoolOffset); // Array of offsets to strings relative to start of this array (this->IDPoolOffset)
	unsigned int count = getUInteger4(memblock, this->IDPoolOffset + 4); // Number of strings in the section
	unsigned int size = getUInteger4(memblock, this->IDPoolOffset + 8); // Size in bytes of elements in offset array
	
	unsigned int arrStart = dataOffset + getUInteger4(memblock, dataOffset); // start of array of offsets to string data

	// Get start offset of string data
	unsigned int stringStart = 0; // start of strings
	for (unsigned int i = 0; i < size; i++) {
		stringStart = stringStart << 8;
		stringStart += memblock[arrStart + i];
	}
	stringStart += arrStart;

	// Decrypt string data
	if (stringStart % 4 != 0) cout << "Warning: decryption not aligned";
	for (unsigned int i = stringStart; i < this->intPoolOffset; i+=4) {
		decryptBytes(memblock, i);
	}

	// Represent each string ID in this->IDPool
	for (unsigned int IDOffset = arrStart; IDOffset < arrStart + (count*size); IDOffset+=size) {
		unsigned int stringOffset = 0; // offset to null-terminated string data
		for (unsigned int i = 0; i < size; i++) {
			stringOffset = stringOffset << 8;
			stringOffset += memblock[IDOffset + i];
		}
		stringOffset += arrStart; // offset is from array start

		// read string until null termination
		string ID = "";
		while (true) {
			unsigned int val = memblock[stringOffset];
			if (val == 0x0) break;
			else ID += (char)val;
			stringOffset++;
		}
		this->IDPool.push_back(ID);
	}

	// for (string str : this->IDPool) cout << str << '\n';
	
}

void Script::initIntPool(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->intPoolOffset);
	unsigned int count = getUInteger4(memblock, this->intPoolOffset + 4);

	// Represent each int in this->intPool
	unsigned int pc = this->intPoolOffset + dataOffset; // go to data
	for (unsigned int i = 0; i < count; i++, pc+=4) {
		this->intPool.push_back(getUInteger4(memblock, pc));
	}

	/*
	cout << "Int Pool: [";
	for (int i : this->intPool) cout << i << ", ";
	cout << " ]" << '\n';
	*/
}

void Script::initFixedPool(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->fixedPoolOffset);
	unsigned int count = getUInteger4(memblock, this->fixedPoolOffset + 4);

	// Represent each float in an attribute float array (this->fixedPool)
	unsigned int pc = this->fixedPoolOffset + dataOffset;
	for (unsigned int i = 0; i < count; i++, pc+=4) {
		this->fixedPool.push_back(getFloat(memblock, pc));
	}

	/*
	cout << "Fixed Pool: [";
	for (float i : this->fixedPool) cout << i << ", ";
	cout << " ]" << '\n';
	*/

}

void Script::initStringPool(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->stringPoolOffset); // Array of offsets to strings relative to start of this array (this->stringPoolOffset)
	unsigned int count = getUInteger4(memblock, this->stringPoolOffset + 4); // Number of strings in the section
	unsigned int size = getUInteger4(memblock, this->stringPoolOffset + 8); // Size in bytes of elements in offset array

	unsigned int arrStart = dataOffset + getUInteger4(memblock, dataOffset); // start of array of offsets to string data

	// Get start offset of string data
	unsigned int stringStart = 0; // start of strings
	for (unsigned int i = 0; i < size; i++) {
		stringStart = stringStart << 8;
		stringStart += memblock[arrStart + i];
	}
	stringStart += arrStart;

	// Decrypt string data
	if (stringStart % 4 != 0) cout << "Warning: decryption not aligned";
	for (unsigned int i = stringStart; i < this->functionPoolOffset; i += 4) {
		decryptBytes(memblock, i);
		// cout << hex << (int)memblock[i] << " " << (int)memblock[i + 1] << " " << (int)memblock[i + 2] << " " << (int)memblock[i + 3] << '\n';
	}

	// TODO: store string data into this->stringPool

}

void Script::initFunctionPool(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->functionPoolOffset);
	unsigned int count = getUInteger4(memblock, this->functionPoolOffset + 4);
	unsigned int size = getUInteger4(memblock, this->functionPoolOffset + 8);

	unsigned int pc = this->functionPoolOffset + dataOffset;

	// Represent functions in a function array
	for (unsigned int i = 0; i < count; i++, pc += size) {
		string name = this->IDPool.at(getUInteger2(memblock, pc));
		unsigned int field2 = getUInteger2(memblock, pc+2);
		unsigned int field4 = getUInteger2(memblock, pc+4);
		unsigned int field6 = getUInteger2(memblock, pc+6);
		unsigned int localPoolIndex = getUInteger2(memblock, pc+8);
		unsigned int field10 = getUInteger2(memblock, pc+10);
		unsigned int start = getUInteger4(memblock, pc+12);
		unsigned int end = getUInteger4(memblock, pc+16);
		Function f(name, field2, field4, field6, localPoolIndex, field10, start, end);
		this->functionPool.push_back(f);
	}

	// for (Function f : this->functionPool) cout << f.getName() << ": " << f.getStart() << ", " << f.getEnd() << '\n';
}

void Script::initPluginImports(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->pluginImportsOffset);
	unsigned int count = getUInteger4(memblock, this->pluginImportsOffset + 4);
	unsigned int size = getUInteger4(memblock, this->pluginImportsOffset + 8);

	unsigned int pc = this->pluginImportsOffset + dataOffset;

	// Represent plugin import in a PluginImport array
	for (unsigned int i = 0; i < count; i++, pc += size) {
		string pluginName = this->IDPool.at(getUInteger2(memblock, pc));
		string functionName = this->IDPool.at(getUInteger2(memblock, pc+2));
		PluginImport pi(pluginName, functionName);
		this->pluginImports.push_back(pi);
	}

	// for (PluginImport pi : this->pluginImports) cout << pi.getPluginName() << "::" << pi.getFunctionName() << '\n';

}

void Script::initOCImports(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->OCImportsOffset);
	unsigned int count = getUInteger4(memblock, this->OCImportsOffset + 4);
	unsigned int size = getUInteger4(memblock, this->OCImportsOffset + 8);

	unsigned int pc = this->OCImportsOffset + dataOffset;

	// Represent imported OC name IDs in an attribute array
	for (unsigned int i = 0; i < count; i++, pc += size) {
		string ID = this->IDPool.at(getUInteger2(memblock, pc));
		this->OCImports.push_back(ID);
	}

	// for (string str : this->OCImports) cout << str << '\n';

}

void Script::initFunctionImports(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->functionImportsOffset);
	unsigned int count = getUInteger4(memblock, this->functionImportsOffset + 4);
	unsigned int size = getUInteger4(memblock, this->functionImportsOffset + 8);

	// TODO: represent function imports in an attribute array
	// Note: xc1 doesn't use function imports in any of it's scripts, so we don't know how function imports are stored (most likely as indexes in ID Pool)
}

void Script::initStaticVariables(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->staticVariablesOffset);
	unsigned int count = getUInteger4(memblock, this->staticVariablesOffset + 4);

	unsigned int pc = this->staticVariablesOffset + dataOffset;

	// represent static objects in an attribute array
	for (int i = 0; i < count; i++) {
		string type = "";
		switch (memblock[pc]) {
		case 0: type = "Null"; break;
		case 1: type = "True"; break;
		case 2: type = "False"; break;
		case 3: type = "Int"; break;
		case 4: type = "Fixed"; break;
		case 5: type = "String"; break;
		case 6: type = "Array"; break;
		case 7: type = "Function"; break;
		case 8: type = "Plugin"; break;
		case 9: type = "OC"; break;
		case 10: type = "Sys"; break;
		}

		unsigned int length = getUInteger2(memblock, pc + 2);
		unsigned int value = getUInteger4(memblock, pc + 4);
		unsigned int field8 = 0;
		if (this->is64Bit) {
			unsigned int field8 = getUInteger4(memblock, pc + 8);
			pc += 12;
		}
		else pc += 8;

		Object so(type, length, value, field8);
		this->staticVariables.push_back(so);
	}

	// for (StaticObject so : this->staticVariables) cout << so.getType() << ", " << so.getLength() << ", " << so.getValue() << ", " << so.getField8() << '\n';

}

void Script::initLocalPool(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->localPoolOffset);
	unsigned int count = getUInteger4(memblock, this->localPoolOffset + 4);
	unsigned int size = getUInteger4(memblock, this->localPoolOffset + 8);

	unsigned int pc = this->localPoolOffset + dataOffset;
	unsigned int arrStart = pc;

	// Get all offsets to function stack data
	for (unsigned int i = 0; i < count; i++, pc += size) {
		unsigned int stackOffset = 0; // offset to null-terminated string data
		for (unsigned int j = 0; j < size; j++) {
			stackOffset = stackOffset << 8;
			stackOffset += memblock[pc + j];
		}
		stackOffset += arrStart; // offset is from array start

		// Represent function stacks in an attribute array
		unsigned int stackDataOffset = stackOffset + getUInteger4(memblock, stackOffset);
		unsigned int stackCount = getUInteger4(memblock, stackOffset + 4);

		vector<Object> functionStack;
		for (int j = 0; j < stackCount; j++) {
			string type = "";
			switch (memblock[stackDataOffset]) {
			case 0: type = "Null"; break;
			case 1: type = "True"; break;
			case 2: type = "False"; break;
			case 3: type = "Int"; break;
			case 4: type = "Fixed"; break;
			case 5: type = "String"; break;
			case 6: type = "Array"; break;
			case 7: type = "Function"; break;
			case 8: type = "Plugin"; break;
			case 9: type = "OC"; break;
			case 10: type = "Sys"; break;
			}

			unsigned int length = getUInteger2(memblock, stackDataOffset + 2);
			unsigned int value = getUInteger4(memblock, stackDataOffset + 4);
			unsigned int field8 = 0;
			if (this->is64Bit) {
				unsigned int field8 = getUInteger4(memblock, stackDataOffset + 8);
				stackDataOffset += 12;
			}
			else stackDataOffset += 8;

			Object so(type, length, value, field8);
			functionStack.push_back(so);
		}
		this->localPool.push_back(functionStack);
	}

	/*
	int index = 0;
	for (vector<Object> v : this->localPool) {
		cout << "Index " << index << ": " << '\n';
		for (Object o : v) {
			cout << o.getType() << ", " << o.getLength() << ", " << o.getValue() << ", " << o.getField8() << '\n';
		}
		cout << '\n';
		index++;
	}
	*/

}

void Script::initSystemAttributePool(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->systemAttributePoolOffset);
	unsigned int count = getUInteger4(memblock, this->systemAttributePoolOffset + 4);
	unsigned int size = getUInteger4(memblock, this->systemAttributePoolOffset + 8);

	unsigned int pc = this->systemAttributePoolOffset + dataOffset;

	// represent system attributes in an attribute array
	for (unsigned int i = 0; i < count; i++, pc += size) {
		unsigned int IDPoolIndex = 0;
		for (unsigned int j = 0; j < size; j++) {
			IDPoolIndex = IDPoolIndex << 8;
			IDPoolIndex += memblock[pc + j];
		}
		if (IDPoolIndex == 0xFFFF) this->systemAttributePool.push_back("");
		else this->systemAttributePool.push_back(this->IDPool.at(IDPoolIndex));
	
	}

	// for (string str : this->systemAttributePool) cout << str << '\n';

}

void Script::initUserAttributePool(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->userAttributePoolOffset);
	unsigned int count = getUInteger4(memblock, this->userAttributePoolOffset + 4);
	unsigned int size = getUInteger4(memblock, this->userAttributePoolOffset + 8);

	unsigned int pc = this->userAttributePoolOffset + dataOffset;

	// represent user attributes in an attribute array
	// Note: xc1 doesn't use user attributes in any of it's scripts
	for (unsigned int i = 0; i < count; i++, pc += size) {
		unsigned int IDPoolIndex = 0;
		for (unsigned int j = 0; j < size; j++) {
			IDPoolIndex = IDPoolIndex << 8;
			IDPoolIndex += memblock[pc + j];
		}
		if (IDPoolIndex == 0xFFFF) this->userAttributePool.push_back("");
		else this->userAttributePool.push_back(this->IDPool.at(IDPoolIndex));
	}

	// for (string str : this->userAttributePool) cout << str << '\n';

}

void Script::decryptBytes(unsigned char* memblock, int start) {
	unsigned char b0 = memblock[start];
	unsigned char b1 = memblock[start + 1];
	unsigned char b2 = memblock[start + 2];
	unsigned char b3 = memblock[start + 3];
	
	memblock[start] =     (b0 >> 2) + (b3 << 6);
	memblock[start + 1] = (b1 >> 2) + (b0 << 6);
	memblock[start + 2] = (b2 >> 2) + (b1 << 6);
	memblock[start + 3] = (b3 >> 2) + (b2 << 6);
}

void Script::encryptBytes(unsigned char* memblock, int start) {
	unsigned char f0 = memblock[start];
	unsigned char f1 = memblock[start + 1];
	unsigned char f2 = memblock[start + 2];
	unsigned char f3 = memblock[start + 3];
	
	memblock[start] =     (f0 << 2) + (f1 >> 6);
	memblock[start + 1] = (f1 << 2) + (f2 >> 6);
	memblock[start + 2] = (f2 << 2) + (f3 >> 6);
	memblock[start + 3] = (f3 << 2) + (f0 >> 6);
}
