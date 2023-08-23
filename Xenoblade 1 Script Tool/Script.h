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
	Script(std::string fileName);
	void generateOutfile(std::string name);
	void generateScriptFile(std::string name);

	int getIndexInIDPool(std::string ID);

	std::vector<unsigned char> generateCodeSection();
	std::vector<unsigned char> generateIntPoolSection();
	std::vector<unsigned char> generateFixedPoolSection();
	std::vector<unsigned char> generateStringSection(std::vector<std::string> input);
	std::vector<unsigned char> generateFunctionPoolSection();
	std::vector<unsigned char> generatePluginImportsSection();
	std::vector<unsigned char> generateOCImportsSection();
	std::vector<unsigned char> generateFunctionImportsSection();
	std::vector<unsigned char> generateStaticVariablesSection();
	std::vector<unsigned char> generateLocalPoolSection();
	std::vector<unsigned char> generateSystemAttributesSection();
	std::vector<unsigned char> generateUserAttributesSection();
};
