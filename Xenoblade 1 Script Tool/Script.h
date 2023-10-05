#pragma once
#include "Script.h"
#include "Function.h"
#include "PluginImport.h"
#include "Object.h"
#include "Instruction.h"

class Script
{
	unsigned char version, flags, isLoaded;

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

	void initCode(unsigned char* memblock, unsigned int codeOffset);
	void initIDPool(unsigned char* memblock, unsigned int IDPoolOffset, unsigned int intPoolOffset);
	void initIntPool(unsigned char* memblock, unsigned int intPoolOffset);
	void initFixedPool(unsigned char* memblock, unsigned int fixedPoolOffset);
	void initStringPool(unsigned char* memblock, unsigned int stringPoolOffset, unsigned int functionPoolOffset);
	void initFunctionPool(unsigned char* memblock, unsigned int functionPoolOffset);
	void initPluginImports(unsigned char* memblock, unsigned int pluginImportsOffset);
	void initOCImports(unsigned char* memblock, unsigned int OCImportsOffset);
	void initFunctionImports(unsigned char* memblock, unsigned int functionImportsOffset);
	void initStaticVariables(unsigned char* memblock, unsigned int staticVariablesOffset);
	void initLocalPool(unsigned char* memblock, unsigned int localPoolOffset);
	void initSystemAttributePool(unsigned char* memblock, unsigned int systemAttributePoolOffset);
	void initUserAttributePool(unsigned char* memblock, unsigned int userAttributePoolOffset);

	void initCSVFunctionPool(std::vector<std::string> lines, int& currIndex);
	void initCSVIDPool(std::vector<std::string> lines, int& currIndex);
	void initCSVIntPool(std::vector<std::string> lines, int& currIndex);
	void initCSVFixedPool(std::vector<std::string> lines, int& currIndex);
	void initCSVStringPool(std::vector<std::string> lines, int& currIndex);
	void initCSVStaticVariables(std::vector<std::string> lines, int& currIndex);
	void initCSVPluginImports(std::vector<std::string> lines, int& currIndex);
	void initCSVOCImports(std::vector<std::string> lines, int& currIndex);
	void initCSVSystemAttributes(std::vector<std::string> lines, int& currIndex);
	void initCSVUserAttributes(std::vector<std::string> lines, int& currIndex);

	void decryptBytes(unsigned char* memblock, int start);
	void encryptBytes(unsigned char* memblock, int start);

public:
	Script(std::string fileName, bool isScript);
	void initScript(std::string fileName);
	void initCSV(std::string fileName);
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
