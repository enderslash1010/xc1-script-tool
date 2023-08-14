#pragma once
#include "Script.h"
#include "Function.h"
#include "PluginImport.h"
#include "Object.h"
#include "Instruction.h"

class Script
{
	unsigned char version, flags, isLoaded;
	unsigned int codeOffset, IDPoolOffset, intPoolOffset, fixedPoolOffset, stringPoolOffset, functionPoolOffset;
	unsigned int pluginImportsOffset, OCImportsOffset, functionImportsOffset;
	unsigned int staticVariablesOffset, localPoolOffset, systemAttributePoolOffset, userAttributePoolOffset, debugSymbolsOffset;

	bool littleEndian, is64Bit;

	std::vector<std::string> IDPool, stringPool, OCImports, systemAttributePool, userAttributePool;
	std::vector<int> intPool;
	std::vector<float> fixedPool;
	std::vector<Function> functionPool;
	std::vector<PluginImport> pluginImports;
	std::vector<Object> staticVariables;
	std::vector<std::vector<Object>> localPool;

	unsigned int getUInteger4(unsigned char* memblock, int start);
	unsigned int getUInteger2(unsigned char* memblock, int start);
	unsigned int getInteger2(unsigned char* memblock, int start);
	unsigned int getInteger4(unsigned char* memblock, int start);
	float getFloat(unsigned char* memblock, int start);

	void initCode(unsigned char* memblock);
	void initIDPool(unsigned char* memblock);
	void initIntPool(unsigned char* memblock);
	void initFixedPool(unsigned char* memblock);
	void initStringPool(unsigned char* memblock);
	void initFunctionPool(unsigned char* memblock);
	void initPluginImports(unsigned char* memblock);
	void initOCImports(unsigned char* memblock);
	void initFunctionImports(unsigned char* memblock);
	void initStaticVariables(unsigned char* memblock);
	void initLocalPool(unsigned char* memblock);
	void initSystemAttributePool(unsigned char* memblock);
	void initUserAttributePool(unsigned char* memblock);

	void decryptBytes(unsigned char* memblock, int start);
	void encryptBytes(unsigned char* memblock, int start);

public:
	Script(char* fileName);
	void generateOutfile(std::string name);
};
