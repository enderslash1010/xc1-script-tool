#include "Script.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include "Function.h"
#include "Object.h"

union UInteger4 {
	unsigned int uinteger;
	unsigned char bytes[4];
};
union UInteger2 {
	unsigned int uinteger;
	unsigned char bytes[2];
};
union Integer2 {
	int integer;
	unsigned char bytes[2];
};
union Integer4 {
	int integer;
	unsigned char bytes[4];
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
unsigned int Script::getInteger4(unsigned char* memblock, int start) {
	Integer4 result{};
	if (this->littleEndian) { // Little Endian
		for (int i = start; i < start + 4; i++) result.bytes[start - i + 3] = memblock[i];
	}
	else { // Big Endian
		for (int i = start; i < start + 4; i++) result.bytes[i - start] = memblock[i];
	}
	return result.integer;
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
unsigned int Script::getInteger2(unsigned char* memblock, int start) {
	Integer2 result{};
	if (this->littleEndian) { // Little Endian
		for (int i = start; i < start + 2; i++) result.bytes[start - i + 1] = memblock[i];
	}
	else { // Big Endian
		for (int i = start; i < start + 2; i++) result.bytes[i - start] = memblock[i];
	}
	return result.integer;
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

Script::Script(std::string fileName, bool isScript) 
{
	if (isScript) initScript(fileName);
	else initCSV(fileName);
}

void Script::initScript(std::string fileName) {
	std::ifstream buffer(fileName, std::ios::binary | std::ios::ate);
	if (!buffer.is_open()) throw std::runtime_error("Unable to open file " + fileName);

	// Load buffer into char array
	int size = buffer.tellg();
	unsigned char* memblock = new unsigned char[size];
	buffer.seekg(0, std::ios::beg);
	buffer.read((char*)memblock, size);
	buffer.close();

	// Check file type
	if (size < 63 || memblock[0x0] != 0x53 || memblock[0x1] != 0x42 || memblock[0x2] != 0x20 || memblock[0x3] != 0x20) throw std::runtime_error("Wrong file type");

	// Check Endianness
	int n = 1;
	this->littleEndian = *(char*)&n == 1 ? true : false;

	this->version = memblock[0x4];
	//std::cout << "Version: 0x" << (int) this->version << '\n';

	this->flags = memblock[0x6];
	//std::cout << std::hex << "Flags: 0x" << (int) this->flags << '\n';
	this->is64Bit = (this->flags & 4) != 0;
	//std::cout << "64 Bit: " << this->is64Bit << '\n';

	this->isLoaded = memblock[0x7];
	//std::cout << std::hex << "isLoaded: 0x" << (int) this->isLoaded << '\n';

	this->codeOffset = getUInteger4(memblock, 8);
	//std::cout << std::hex << "Code Offset: 0x" << this->codeOffset << '\n';

	this->IDPoolOffset = getUInteger4(memblock, 12);
	//std::cout << std::hex << "ID Pool Offset: 0x" << this->IDPoolOffset << '\n';

	this->intPoolOffset = getUInteger4(memblock, 16);
	//std::cout << std::hex << "Int Pool Offset: 0x" << this->intPoolOffset << '\n';

	this->fixedPoolOffset = getUInteger4(memblock, 20);
	//std::cout << std::hex << "Fixed Pool Offset: 0x" << this->fixedPoolOffset << '\n';

	this->stringPoolOffset = getUInteger4(memblock, 24);
	//std::cout << std::hex << "String Pool Offset: 0x" << this->stringPoolOffset << '\n';

	this->functionPoolOffset = getUInteger4(memblock, 28);
	//std::cout << std::hex << "Function Pool Offset: 0x" << this->functionPoolOffset << '\n';

	this->pluginImportsOffset = getUInteger4(memblock, 32);
	//std::cout << std::hex << "Plugin Imports Offset: 0x" << this->pluginImportsOffset << '\n';

	this->OCImportsOffset = getUInteger4(memblock, 36);
	//std::cout << std::hex << "OC Imports Offset: 0x" << this->OCImportsOffset << '\n';

	this->functionImportsOffset = getUInteger4(memblock, 40);
	//std::cout << std::hex << "Function Imports Offset: 0x" << this->functionImportsOffset << '\n';

	this->staticVariablesOffset = getUInteger4(memblock, 44);
	//std::cout << std::hex << "Static Variables Offset: 0x" << this->staticVariablesOffset << '\n';

	this->localPoolOffset = getUInteger4(memblock, 48);
	//std::cout << std::hex << "Local Pool Offset: 0x" << this->localPoolOffset << '\n';

	this->systemAttributePoolOffset = getUInteger4(memblock, 52);
	//std::cout << std::hex << "System Attribute Pool Offset: 0x" << this->systemAttributePoolOffset << '\n';

	this->userAttributePoolOffset = getUInteger4(memblock, 56);
	//std::cout << std::hex << "User Attribute Pool Offset: 0x" << this->userAttributePoolOffset << '\n';

	initIDPool(memblock);
	initIntPool(memblock);
	initFixedPool(memblock);
	initStringPool(memblock);
	initLocalPool(memblock);
	initFunctionPool(memblock);
	initPluginImports(memblock);
	initOCImports(memblock);
	initFunctionImports(memblock);
	initStaticVariables(memblock);
	initSystemAttributePool(memblock);
	initUserAttributePool(memblock);
	initCode(memblock);
}

std::vector<std::string> split(std::string str, std::string delim = ",") {
	std::vector<std::string> result;
	int start, end = -1 * delim.size();
	do {
		start = end + delim.size();
		end = str.find(delim, start);
		result.push_back(str.substr(start, end - start));
	} while (end != -1);
	return result;
}

void Script::initCSV(std::string fileName) {
	// TODO: make sure file has extension .csv
	
	// Open file
	std::ifstream csvFile(fileName);
	if (!csvFile.is_open()) throw std::runtime_error("Unable to open file " + fileName);
	
	// Read contents of file into vector or something
	std::vector<std::string> lines;
	while (csvFile.good()) {
		std::string str;
		std::getline(csvFile, str, '\n');
		lines.push_back(str);
	}

	// Find Function Pool
	int currIndex = -1;
	for (int i = 0; i < lines.size(); i++) {
		if (std::regex_match(lines[i], std::regex("Function Pool:(,*)"))) {
			currIndex = i + 1;
			break;
		}
	}
	if (currIndex == -1) throw std::runtime_error("Function Pool is missing");

	// parse .csv data in lines to attribute vectors
	initCSVFunctionPool(lines, currIndex);
	initCSVIDPool(lines, currIndex);
	initCSVIntPool(lines, currIndex);
	initCSVFixedPool(lines, currIndex);
	initCSVStringPool(lines, currIndex);
	initCSVStaticVariables(lines, currIndex);
	initCSVPluginImports(lines, currIndex);
	initCSVOCImports(lines, currIndex);
	initCSVSystemAttributes(lines, currIndex);
	initCSVUserAttributes(lines, currIndex);
}

void Script::initCSVFunctionPool(std::vector<std::string> lines, int& currIndex) {

	while (true) {
		if (std::regex_match(lines[currIndex], std::regex("ID Pool:(,*)"))) break; // Stop when next section (ID Pool) is reached
		std::string functionName = lines[currIndex];

		if (!std::regex_match(lines[currIndex + 1], std::regex("Args,Field 4,Field 6,Field 10(,*)"))) throw std::runtime_error("Invalid attribute header for function " + functionName);
		std::vector<std::string> argVals = split(lines[currIndex + 2]);
		if (argVals.size() < 4) throw std::runtime_error("Invalid attributes for function " + functionName);
		unsigned int args = std::stoi(argVals[0]);
		unsigned int field4 = std::stoi(argVals[1]);
		unsigned int field6 = std::stoi(argVals[2]);
		unsigned int field10 = std::stoi(argVals[3]);

		currIndex += 4; // Go to local pool header if present, otherwise it goes to code instruction header 

		// read local pool if present
		std::vector<Object> localPool;
		if (std::regex_match(lines[currIndex], std::regex("Local Pool:(,*)"))) {
			if (!std::regex_match(lines[currIndex + 1], std::regex("Type,inArray,Length,Value,Field 8(,*)"))) throw std::runtime_error("Invalid Local Pool header for function " + functionName);
			currIndex += 2; // Go to local pool entries
			while (!std::regex_match(lines[currIndex], std::regex(",*"))) {
				// parse local pool entries
				std::vector<std::string> localEntryStr = split(lines[currIndex]);
				if (localEntryStr.size() < 5) throw std::runtime_error("Invalid Local Pool entry in function " + functionName);
				std::string type = localEntryStr[0];
				bool inArray = std::stoi(localEntryStr[1]) == 1 ? true : false;
				unsigned int length = std::stoi(localEntryStr[2]);
				int value = std::stoi(localEntryStr[3]);
				unsigned int field8 = std::stoi(localEntryStr[4]);

				localPool.push_back(Object(type, inArray, length, value, field8));
				currIndex++;
			}
			currIndex++;
		}

		if (!std::regex_match(lines[currIndex], std::regex("Opcode,Operand(,*)"))) throw std::runtime_error("Invalid code section header for function " + functionName);
		currIndex++; // Go to instruction list

		// parse code into vector
		std::vector<Instruction> code;
		while (!std::regex_match(lines[currIndex], std::regex(",*"))) {
			std::vector<std::string> instructionStr = split(lines[currIndex]);
			std::string opcode = instructionStr[0];

			if (strcmp("SWITCH", opcode.c_str()) == 0) { // handle SWITCH Instruction
				if (instructionStr.size() < 4) throw std::runtime_error("Invalid SWITCH Instruction in function " + functionName);
				if (strcmp("Case", instructionStr[2].c_str()) != 0 || strcmp("Offset", instructionStr[3].c_str()) != 0) throw std::runtime_error("Invalid SWITCH case/offset header in function " + functionName);
				unsigned int numCases = std::stoi(instructionStr[1]);

				// Get default case
				std::vector<std::string> defaultStr = split(lines[currIndex + 1]);
				if (defaultStr.size() < 4 || strcmp(defaultStr[2].c_str(), "default") != 0) throw std::runtime_error("Invalid default case in SWITCH Instruction in function " + functionName);
				unsigned int defaultCase = std::stoi(defaultStr[3]);
				currIndex += 2;

				// Get all other cases
				std::map<int, unsigned int> switchCases;
				for (int i = 0; i < numCases; i++) {
					std::vector<std::string> caseStr = split(lines[currIndex]);
					if (caseStr.size() < 4) throw std::runtime_error("Invalid case in SWITCH Instruction in function " + functionName);
					if (strcmp(caseStr[0].c_str(), "") != 0 || strcmp(caseStr[1].c_str(), "") != 0) throw std::runtime_error("SWITCH Instruction cases terminated early (check that SWITCH operand = number of cases)");
					int caseVal = std::stoi(caseStr[2]);
					int offset = std::stoi(caseStr[3]);
					switchCases.insert({ caseVal , offset });
					currIndex++;
				}

				// Make sure there are no other cases
				if (lines.size() > currIndex && std::regex_match(lines[currIndex], std::regex(",,([^,]*,[^,]*)(.*)"))) throw std::runtime_error("SWITCH Instruction has more cases than specified (check that SWITCH operand = number of cases)");
				currIndex--;
			}
			else { // Handle Non-SWITCH Instruction
				switch (OpCode::getOperandSize(opcode)) {
				case 0:
					code.push_back(Instruction(opcode));
					break;
				case 1:
				case 2:
					if (instructionStr.size() < 2) throw std::runtime_error("Instruction " + opcode + " needs an operand");
					code.push_back(Instruction(opcode, std::stoi(instructionStr[1])));
					break;
				}
			}
			currIndex++;
		}

		// Add function to this->functionPool, and add Local Pool to this->localPool if there is one
		if (localPool.size() != 0) {
			this->functionPool.push_back(Function(functionName, args, field4, field6, localPool, this->localPool.size(), field10, -1, -1)); // -1 are for attributes determined when generating a script
			this->localPool.push_back(localPool);
		}
		else {
			this->functionPool.push_back(Function(functionName, args, field4, field6, localPool, 0xFFFF, field10, -1, -1)); // -1 are for attributes determined when generating a script
		}
		currIndex++;
	}
}

void Script::initCSVIDPool(std::vector<std::string> lines, int& currIndex) {
	// currIndex is at "ID Pool:" here
	if (!std::regex_match(lines[++currIndex], std::regex("Index,Value(,*)"))) throw std::runtime_error("Invalid ID Pool header");
	currIndex++; // Go to 0th index
	while (true) {
		if (std::regex_match(lines[currIndex], std::regex("(,*)"))) break; // Check if it's the end of the section
		std::vector<std::string> IDStr = split(lines[currIndex]);
		if (IDStr.size() < 2) throw std::runtime_error("Invalid ID Pool entry at index " + this->IDPool.size()); // Validate entry
		this->IDPool.push_back(IDStr[1]); // Push ID to this->IDPool
		currIndex++; // Go to next ID Pool entry
	}
	currIndex++; // Go to next section (Int Pool)
}

void Script::initCSVIntPool(std::vector<std::string> lines, int& currIndex) {
	// currIndex is at "Int Pool:" here
	if (!std::regex_match(lines[currIndex++], std::regex("Int Pool:(,*)"))) throw std::runtime_error("Missing \"Int Pool:\" after ID Pool");
	if (!std::regex_match(lines[currIndex++], std::regex("Index,Value(,*)"))) throw std::runtime_error("Invalid Int Pool header");
	while (true) {
		if (std::regex_match(lines[currIndex], std::regex("(,*)"))) break; // Check if it's the end of the section
		std::vector<std::string> IntStr = split(lines[currIndex]);
		if (IntStr.size() < 2) throw std::runtime_error("Invalid Int Pool entry at index " + this->intPool.size()); // Validate entry
		this->intPool.push_back(std::stoi(IntStr[1])); // Push ID to this->IntPool
		currIndex++; // Go to next Int Pool entry
	}
	currIndex++; // Go to next section (Fixed Pool)
}

void Script::initCSVFixedPool(std::vector<std::string> lines, int& currIndex) {
	// currIndex is at "Fixed Pool:" here
	if (!std::regex_match(lines[currIndex++], std::regex("Fixed Pool:(,*)"))) throw std::runtime_error("Missing \"Fixed Pool:\" after Int Pool");
	if (!std::regex_match(lines[currIndex++], std::regex("Index,Value(,*)"))) throw std::runtime_error("Invalid Fixed Pool header");
	while (true) {
		if (std::regex_match(lines[currIndex], std::regex("(,*)"))) break; // Check if it's the end of the section
		std::vector<std::string> fixedStr = split(lines[currIndex]);
		if (fixedStr.size() < 2) throw std::runtime_error("Invalid Fixed Pool entry at index " + this->fixedPool.size()); // Validate entry
		this->fixedPool.push_back(std::stof(fixedStr[1])); // Push to this->fixedPool
		currIndex++; // Go to next fixed Pool entry
	}
	currIndex++; // Go to next section (String Pool)
}

void Script::initCSVStringPool(std::vector<std::string> lines, int& currIndex) {
	// currIndex is at "String Pool:" here
	if (!std::regex_match(lines[currIndex++], std::regex("String Pool:(,*)"))) throw std::runtime_error("Missing \"String Pool:\" after Fixed Pool");
	if (!std::regex_match(lines[currIndex++], std::regex("Index,Value(,*)"))) throw std::runtime_error("Invalid String Pool header");
	while (true) {
		if (std::regex_match(lines[currIndex], std::regex("(,*)"))) break; // Check if it's the end of the section
		std::vector<std::string> str = split(lines[currIndex]);
		if (str.size() < 2) throw std::runtime_error("Invalid String Pool entry at index " + this->stringPool.size()); // Validate entry
		this->stringPool.push_back(str[1]); // Push to this->stringPool
		currIndex++; // Go to next String Pool entry
	}
	currIndex++; // Go to next section (Static variables)
}

void Script::initCSVStaticVariables(std::vector<std::string> lines, int& currIndex) {
	// currIndex is at "Static Variables:" here
	if (!std::regex_match(lines[currIndex++], std::regex("Static Variables:(,*)"))) throw std::runtime_error("Missing \"Static Variables:\" after String Pool");
	if (!std::regex_match(lines[currIndex++], std::regex("Index,Type,inArray,Length,Value,Field 8(,*)"))) throw std::runtime_error("Invalid Static Variables header");
	while (true) {
		if (std::regex_match(lines[currIndex], std::regex("(,*)"))) break; // Check if it's the end of the section
		std::vector<std::string> staticVar = split(lines[currIndex]);
		if (staticVar.size() < 6) throw std::runtime_error("Static Variable at index " + this->staticVariables.size()); // Validate entry
		this->staticVariables.push_back(Object(staticVar[1], staticVar[2] == "1" ? true : false, std::stoi(staticVar[3]), std::stoi(staticVar[4]), std::stoi(staticVar[5]))); // Push to this->staticVariables
		currIndex++; // Go to next Static Variable
	}
	currIndex++; // Go to next section (Plugin Imports)
}

void Script::initCSVPluginImports(std::vector<std::string> lines, int& currIndex) {
	// currIndex is at "Plugin Imports:"
	if (!std::regex_match(lines[currIndex++], std::regex("Plugin Imports:(,*)"))) throw std::runtime_error("Missing \"Plugin Imports:\" after Static Variables");
	if (!std::regex_match(lines[currIndex++], std::regex("Index,Plugin Name,Function(,*)"))) throw std::runtime_error("Invalid Plugin Imports header");
	while (true) {
		if (std::regex_match(lines[currIndex], std::regex("(,*)"))) break; // Check if it's the end of the section
		std::vector<std::string> plugin = split(lines[currIndex]);
		if (plugin.size() < 3) throw std::runtime_error("Invalid Plugin Import at index " + this->pluginImports.size()); // Validate entry
		this->pluginImports.push_back(PluginImport(plugin[1], plugin[2]));
		currIndex++; // Go to next Plugin Import
	}
	currIndex++; // Go to next section (OC Imports)
}

void Script::initCSVOCImports(std::vector<std::string> lines, int& currIndex) {
	// currIndex is at "OC Imports:"
	if (!std::regex_match(lines[currIndex++], std::regex("OC Imports:(,*)"))) throw std::runtime_error("Missing \"OC Imports:\" after Plugin Imports");
	if (!std::regex_match(lines[currIndex++], std::regex("Index,Name(,*)"))) throw std::runtime_error("Invalid OC Imports header");
	while (true) {
		if (std::regex_match(lines[currIndex], std::regex("(,*)"))) break; // Check if it's the end of the section
		std::vector<std::string> oc = split(lines[currIndex]);
		if (oc.size() < 2) throw std::runtime_error("Invalid OC Import at index " + this->OCImports.size()); // Validate entry
		this->OCImports.push_back(oc[1]);
		currIndex++; // Go to next OC Import
	}
	currIndex++; // Go to next section (System Attributes)
}

void Script::initCSVSystemAttributes(std::vector<std::string> lines, int& currIndex) {
	// currIndex is at "System Attribute Pool:"
	if (!std::regex_match(lines[currIndex++], std::regex("System Attribute Pool:(,*)"))) throw std::runtime_error("Missing \"System Attribute Pool:\" after OC Imports");
	if (!std::regex_match(lines[currIndex++], std::regex("Index,Name(,*)"))) throw std::runtime_error("Invalid System Attribute Pool header");
	while (true) {
		if (std::regex_match(lines[currIndex], std::regex("(,*)"))) break; // Check if it's the end of the section
		std::vector<std::string> sysAttr = split(lines[currIndex]);
		if (sysAttr.size() < 2) throw std::runtime_error("Invalid System Attribute at index " + this->systemAttributePool.size()); // Validate entry
		this->systemAttributePool.push_back(sysAttr[1]);
		currIndex++; // Go to next System Attribute
	}
	currIndex++; // Go to next section (User Attributes)
}

void Script::initCSVUserAttributes(std::vector<std::string> lines, int& currIndex) {
	// currIndex is at "User Attribute Pool:"
	if (!std::regex_match(lines[currIndex++], std::regex("User Attribute Pool:(,*)"))) throw std::runtime_error("Missing \"User Attribute Pool:\" after System Attribute Pool");
	if (!std::regex_match(lines[currIndex++], std::regex("Index,Name(,*)"))) throw std::runtime_error("Invalid User Attribute Pool header");
	while (true) {
		if (std::regex_match(lines[currIndex], std::regex("(,*)"))) break; // Check if it's the end of the section
		std::vector<std::string> userAttr = split(lines[currIndex]);
		if (userAttr.size() < 2) throw std::runtime_error("Invalid User Attribute at index " + this->userAttributePool.size()); // Validate entry
		this->userAttributePool.push_back(userAttr[1]);
		currIndex++; // Go to next System Attribute
	}
	currIndex++; // Go to next section (User Attributes)
}

void Script::initCode(unsigned char *memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->codeOffset);
	unsigned int length = getUInteger4(memblock, this->codeOffset + 8);

	unsigned int codeStart = this->codeOffset + dataOffset;

	// Put instructions in a vector in each function object
	for (Function &f : this->functionPool) {
		unsigned int pc = f.getStart() + codeStart; // Go to start of function

		std::vector<Instruction> functionInstructions;
		while (pc < f.getEnd() + codeStart) {
			OpCode opcode(memblock[pc++]);
			int size = opcode.getOperandSize();

			if (opcode.getOpCodeVal() == OpCode::OpCodes::SWITCH) { // SWITCH
				int numCases = memblock[pc++];
				unsigned int defaultCase = getUInteger4(memblock, pc); pc += 4;

				std::map<int, unsigned int> switchCases;
				for (int i = 0; i < numCases; i++) {
					int value = getInteger4(memblock, pc); pc += 4;
					int jump = getInteger4(memblock, pc); pc += 4;
					switchCases.insert({ value, jump });
				}
				functionInstructions.push_back(Instruction(opcode, numCases, switchCases, defaultCase));
				continue;
			}

			switch (size) {
			case 0:
				functionInstructions.push_back(Instruction(opcode));
				break;
			case 1:
				functionInstructions.push_back(Instruction(opcode, memblock[pc]));
				break;
			case 2:
				functionInstructions.push_back(Instruction(opcode, getInteger2(memblock, pc)));
				break;
			}
			pc += size;

		}
		f.addCode(functionInstructions);
	}

	/*
	for (Function f : this->functionPool) {
		cout << f.getName() << ":\n";
		for (Instruction i : f.getCode()) {
			if (i.getOperand() == -1) cout << '\t' << i.getOpCode().getOpCodeString() << '\n';
			else if (i.getOpCode().getOpCodeVal() == OpCode::OpCodes::SWITCH) {
				cout << '\t' << i.getOpCode().getOpCodeString() << ": " << i.getOperand() << '\n';

				std::unordered_map<int, unsigned int> switchCases = i.getSwitchCases();
				for (auto it = switchCases.begin(); it != switchCases.end(); ++it) {
					std::cout << "\t\t" << "case: " << it->first << ", offset: " << it->second << '\n';
				}
				std::cout << "\t\t" << "default: " << i.getDefaultCase() << '\n';
			}
			else cout << '\t' << i.getOpCode().getOpCodeString() << ": " << i.getOperand() << '\n';
		}
	}
	*/
	
}

void Script::initIDPool(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->IDPoolOffset); // Array of offsets to strings relative to start of this array (this->IDPoolOffset)
	unsigned int count = getUInteger4(memblock, this->IDPoolOffset + 4); // Number of strings in the section
	unsigned int size = getUInteger4(memblock, this->IDPoolOffset + 8); // Size in bytes of elements in offset array
	
	unsigned int arrStart = dataOffset + this->IDPoolOffset; // start of array of offsets to string data

	// Get start offset of string data
	unsigned int stringStart = 0; // start of strings
	for (unsigned int i = 0; i < size; i++) {
		stringStart = stringStart << 8;
		stringStart += memblock[arrStart + i];
	}
	stringStart += arrStart;

	// Decrypt string data
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
		std::string ID = "";
		while (true) {
			unsigned int val = memblock[stringOffset];
			if (val == 0x0) break;
			else ID += (char)val;
			stringOffset++;
		}
		this->IDPool.push_back(ID);
	}

	// for (std::string str : this->IDPool) std::cout << str << '\n';
	
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

	unsigned int arrStart = dataOffset + this->stringPoolOffset; // start of array of offsets to string data

	// Get start offset of string data
	unsigned int stringStart = 0; // start of strings
	for (unsigned int i = 0; i < size; i++) {
		stringStart = stringStart << 8;
		stringStart += memblock[arrStart + i];
	}
	stringStart += arrStart;

	// Decrypt string data
	for (unsigned int i = stringStart; i < this->functionPoolOffset; i += 4) {
		decryptBytes(memblock, i);
	}

	// Represent each string ID in this->stringPool
	for (unsigned int offsetArr = arrStart; offsetArr < arrStart + (count * size); offsetArr += size) {
		unsigned int stringOffset = 0; // offset to string data
		for (unsigned int i = 0; i < size; i++) {
			stringOffset = stringOffset << 8;
			stringOffset += memblock[offsetArr + i];
		}
		stringOffset += arrStart; // offset is from array start

		// read string
		std::string str = "";
		while (true) {
			unsigned int val = memblock[stringOffset];
			if (val == 0x0) break;
			str += (char)val;
			stringOffset++;
		}
		this->stringPool.push_back(str);
	}

	// for (std::string str : this->stringPool) std::cout << str << '\n';

}

void Script::initFunctionPool(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->functionPoolOffset);
	unsigned int count = getUInteger4(memblock, this->functionPoolOffset + 4);
	unsigned int size = getUInteger4(memblock, this->functionPoolOffset + 8);

	unsigned int pc = this->functionPoolOffset + dataOffset;

	// Represent functions in a function array
	for (unsigned int i = 0; i < count; i++, pc += size) {
		std::string name = this->IDPool.at(getUInteger2(memblock, pc));
		unsigned int field2 = getUInteger2(memblock, pc+2);
		unsigned int field4 = getUInteger2(memblock, pc+4);
		unsigned int field6 = getUInteger2(memblock, pc+6);
		unsigned int localPoolIndex = getUInteger2(memblock, pc+8);
		unsigned int field10 = getUInteger2(memblock, pc+10);
		unsigned int start = getUInteger4(memblock, pc+12);
		unsigned int end = getUInteger4(memblock, pc+16);

		std::vector<Object> localPool;
		if (localPoolIndex != 0xFFFF) localPool = this->localPool.at(localPoolIndex);

		Function f(name, field2, field4, field6, localPool, localPoolIndex, field10, start, end);
		this->functionPool.push_back(f);
	}
	// for (Function f : this->functionPool) std::cout << f.getName() << ": " << f.getStart() << ", " << f.getEnd() << '\n';
}

void Script::initPluginImports(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->pluginImportsOffset);
	unsigned int count = getUInteger4(memblock, this->pluginImportsOffset + 4);
	unsigned int size = getUInteger4(memblock, this->pluginImportsOffset + 8);

	unsigned int pc = this->pluginImportsOffset + dataOffset;

	// Represent plugin import in a PluginImport array
	for (unsigned int i = 0; i < count; i++, pc += size) {
		std::string pluginName = this->IDPool.at(getUInteger2(memblock, pc));
		std::string functionName = this->IDPool.at(getUInteger2(memblock, pc+2));
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
		std::string ID = this->IDPool.at(getUInteger2(memblock, pc));
		this->OCImports.push_back(ID);
	}

	// for (string str : this->OCImports) cout << str << '\n';

}

void Script::initFunctionImports(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->functionImportsOffset);
	unsigned int count = getUInteger4(memblock, this->functionImportsOffset + 4);
	unsigned int size = getUInteger4(memblock, this->functionImportsOffset + 8);

	// Note: xc1 doesn't use function imports in any of it's scripts, so we don't know how function imports are stored (most likely as indexes in ID Pool)
}

void Script::initStaticVariables(unsigned char* memblock) {
	unsigned int dataOffset = getUInteger4(memblock, this->staticVariablesOffset);
	unsigned int count = getUInteger4(memblock, this->staticVariablesOffset + 4);

	unsigned int pc = this->staticVariablesOffset + dataOffset;

	// represent static objects in an attribute array
	for (unsigned int i = 0; i < count; i++) {

		unsigned int type = memblock[pc];
		unsigned char arrayFlag = memblock[pc + 1];
		bool inArray = arrayFlag == 0xB5 ? true : false;
		unsigned int length = getUInteger2(memblock, pc + 2);
		unsigned int value = getUInteger4(memblock, pc + 4);
		unsigned int field8 = 0;
		if (this->is64Bit) {
			unsigned int field8 = getUInteger4(memblock, pc + 8);
			pc += 12;
		}
		else pc += 8;

		Object so((Object::Type) type, inArray, length, value, field8);
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

		std::vector<Object> functionStack;
		for (unsigned int j = 0; j < stackCount; j++) {
			unsigned int type = memblock[stackDataOffset];
			unsigned char arrayFlag = memblock[stackDataOffset + 1];
			bool inArray = arrayFlag == 0xB5 ? true : false;
			unsigned int length = getUInteger2(memblock, stackDataOffset + 2);
			unsigned int value = getUInteger4(memblock, stackDataOffset + 4);
			unsigned int field8 = 0;
			if (this->is64Bit) {
				unsigned int field8 = getUInteger4(memblock, stackDataOffset + 8);
				stackDataOffset += 12;
			}
			else stackDataOffset += 8;

			Object so((Object::Type) type, inArray, length, value, field8);
			functionStack.push_back(so);
		}
		this->localPool.push_back(functionStack);
	}

	/*
	int index = 0;
	for (std::vector<Object> v : this->localPool) {
		std::cout << "Index " << index << ": " << '\n';
		for (Object o : v) {
			std::cout << o.getType() << ", " << o.getLength() << ", " << o.getValue() << ", " << o.getField8() << '\n';
		}
		std::cout << '\n';
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

void Script::generateOutfile(std::string name) {
	std::ofstream outfile(name + ".csv");

	// Function Pool (with Local Pool and Code)
	outfile << "Function Pool:" << '\n';
	for (Function f : this->functionPool) {
		outfile << f.getName() << '\n';
		outfile << "Args,Field 4,Field 6,Field 10\n";
		outfile << f.getArgs() << "," << f.getField4() << "," << f.getField6() << "," << f.getField10() << "\n\n";
		if (f.getLocalPool().size() != 0) {
			outfile << "Local Pool:" << '\n' << "Type,inArray,Length,Value,Field 8\n";
			for (Object o : f.getLocalPool()) {
				outfile << o.getType() << "," << o.isInArray() << "," << o.getLength() << "," << o.getValue() << "," << o.getField8() << '\n';
			}
			outfile << '\n';
		}
		outfile << "Opcode,Operand\n";
		for (Instruction i : f.getCode()) {
			if (i.getOperand() == -1) {
				outfile << i.getOpCode().getOpCodeString() << '\n';
			}
			else if (i.getOpCode().getOpCodeVal() == OpCode::OpCodes::SWITCH) {
				outfile << "SWITCH," << i.getOperand() << ",Case,Offset\n,,default," << i.getDefaultCase() << '\n';
				std::map<int, unsigned int> switchCases = i.getSwitchCases();
				for (auto it = switchCases.begin(); it != switchCases.end(); ++it) {
					outfile << ",," << it->first << "," << it->second << '\n';
				}
			}
			else {
				outfile << i.getOpCode().getOpCodeString() << "," << i.getOperand() << '\n';
			}
		}

		outfile << '\n';
	}

	// ID Pool
	outfile << "ID Pool:\nIndex,Value\n";
	for (int i = 0; i < this->IDPool.size(); i++) {
		outfile << i << "," << this->IDPool.at(i) << '\n';
	}
	outfile << '\n';

	// Int Pool
	outfile << "Int Pool:\nIndex,Value\n";
	for (int i = 0; i < this->intPool.size(); i++) {
		outfile << i << "," << this->intPool.at(i) << '\n';
	}
	outfile << '\n';

	// Fixed Pool
	outfile << "Fixed Pool:\nIndex,Value\n";
	for (int i = 0; i < this->fixedPool.size(); i++) {
		outfile << i << "," << this->fixedPool.at(i) << '\n';
	}
	outfile << '\n';

	// String Pool
	outfile << "String Pool:\nIndex,Value\n";
	for (int i = 0; i < this->stringPool.size(); i++) {
		outfile << i << "," << this->stringPool.at(i) << '\n';
	}
	outfile << '\n';

	// Static Vars
	outfile << "Static Variables:\nIndex,Type,inArray,Length,Value,Field 8\n";
	for (int i = 0; i < this->staticVariables.size(); i++) {
		Object o = this->staticVariables.at(i);
		outfile << i << "," << o.getType() << "," << o.isInArray() << "," << o.getLength() << "," << o.getValue() << "," << o.getField8() << '\n';
	}
	outfile << '\n';

	// Plugin Imports
	outfile << "Plugin Imports:\nIndex,Plugin Name,Function\n";
	for (int i = 0; i < this->pluginImports.size(); i++) {
		PluginImport pi = this->pluginImports.at(i);
		outfile << i << "," << pi.getPluginName() << "," << pi.getFunctionName() << '\n';
	}
	outfile << '\n';

	// OC Imports
	outfile << "OC Imports:\nIndex,Name\n";
	for (int i = 0; i < this->OCImports.size(); i++) {
		outfile << i << "," << this->OCImports.at(i) << '\n';
	}
	outfile << '\n';

	// System Attribute Pool
	outfile << "System Attribute Pool:\nIndex,Name\n";
	for (int i = 0; i < this->systemAttributePool.size(); i++) {
		outfile << i << "," << this->systemAttributePool.at(i) << '\n';
	}
	outfile << '\n';

	// User Attribute Pool
	outfile << "User Attribute Pool:\nIndex,Name\n";
	for (int i = 0; i < this->userAttributePool.size(); i++) {
		outfile << i << "," << this->userAttributePool.at(i) << '\n';
	}
	outfile << '\n';

	outfile.close();
}

void addValueToVector(std::vector<unsigned char>& vector, unsigned int numBytes, int value) {
	for (int i = numBytes - 1; i >= 0; i--) vector.push_back(value >> (i * 8));
}
void addValueToVectorStart(std::vector<unsigned char>& vector, unsigned int numBytes, int value) {
	for (int i = 0; i < numBytes; i++) vector.insert(vector.begin(), value >> (i * 8));
}

void addHeaderOffset(std::vector<unsigned char>& script, int& currOffset, std::vector<unsigned char>& section) {
	addValueToVector(script, 4, currOffset);
	currOffset += section.size();
}

void Script::generateScriptFile(std::string name) {
	// Make new script file name.sb
	std::ofstream script(name + ".sb", std::ios::trunc | std::ios::binary);

	// Generate script file data from the data in the vector arrays
	// Code section
	std::vector<unsigned char> codeSection = this->generateCodeSection();

	// ID Pool section
	std::vector<unsigned char> IDPoolSection = this->generateStringSection(this->IDPool);

	// Int Pool section
	std::vector<unsigned char> intPoolSection = this->generateIntPoolSection();

	// Fixed Pool section
	std::vector<unsigned char> fixedPoolSection = this->generateFixedPoolSection();

	// String Pool section
	std::vector<unsigned char> stringPoolSection = this->generateStringSection(this->stringPool);

	// Local Pool section
	std::vector<unsigned char> localPoolSection = this->generateLocalPoolSection();

	// Function Pool section
	std::vector<unsigned char> functionPoolSection = this->generateFunctionPoolSection();

	// Plugin Imports section
	std::vector<unsigned char> pluginImportsSection = this->generatePluginImportsSection();

	// OC Imports section
	std::vector<unsigned char> OCImportsSection = this->generateOCImportsSection();

	// Function Imports section
	std::vector<unsigned char> functionImportsSection = this->generateFunctionImportsSection();

	// Static Variables section
	std::vector<unsigned char> staticVarsSection = this->generateStaticVariablesSection();

	// System Attributes Pool section
	std::vector<unsigned char> systemAttributesSection = this->generateSystemAttributesSection();

	// User Attribute Pool section
	std::vector<unsigned char> userAttributesSection = this->generateUserAttributesSection();

	// Create header and append all sections
	std::vector<unsigned char> header;
	header.insert(header.end(), {0x53, 0x42, 0x20, 0x20}); // file magic
	header.insert(header.end(), {this->version, 0});
	header.push_back(this->flags);
	header.push_back(this->isLoaded);

	int currOffset = 0x40; // size of header
	addHeaderOffset(header, currOffset, codeSection);
	addHeaderOffset(header, currOffset, IDPoolSection);
	addHeaderOffset(header, currOffset, intPoolSection);
	addHeaderOffset(header, currOffset, fixedPoolSection);
	addHeaderOffset(header, currOffset, stringPoolSection);
	addHeaderOffset(header, currOffset, functionPoolSection);
	addHeaderOffset(header, currOffset, pluginImportsSection);
	addHeaderOffset(header, currOffset, OCImportsSection);
	addHeaderOffset(header, currOffset, functionImportsSection);
	addHeaderOffset(header, currOffset, staticVarsSection);
	addHeaderOffset(header, currOffset, localPoolSection);
	addHeaderOffset(header, currOffset, systemAttributesSection);
	addHeaderOffset(header, currOffset, userAttributesSection);
	header.insert(header.end(), { 0, 0, 0, 0 }); // debug symbols offset

	script.write((char*)header.data(), header.size());
	script.write((char*)codeSection.data(), codeSection.size());
	script.write((char*)IDPoolSection.data(), IDPoolSection.size());
	script.write((char*)intPoolSection.data(), intPoolSection.size());
	script.write((char*)fixedPoolSection.data(), fixedPoolSection.size());
	script.write((char*)stringPoolSection.data(), stringPoolSection.size());
	script.write((char*)functionPoolSection.data(), functionPoolSection.size());
	script.write((char*)pluginImportsSection.data(), pluginImportsSection.size());
	script.write((char*)OCImportsSection.data(), OCImportsSection.size());
	script.write((char*)functionImportsSection.data(), functionImportsSection.size());
	script.write((char*)staticVarsSection.data(), staticVarsSection.size());
	script.write((char*)localPoolSection.data(), localPoolSection.size());
	script.write((char*)systemAttributesSection.data(), systemAttributesSection.size());
	script.write((char*)userAttributesSection.data(), userAttributesSection.size());

	// pad to nearest 16 (the actual script files pad even more)
	int fileSize = header.size() + codeSection.size() + IDPoolSection.size() + intPoolSection.size() + fixedPoolSection.size() + stringPoolSection.size() + functionPoolSection.size() + pluginImportsSection.size()
		+ OCImportsSection.size() + functionImportsSection.size() + staticVarsSection.size() + localPoolSection.size() + systemAttributesSection.size() + userAttributesSection.size();
	
	for (int i = 0; i < 16 - (fileSize & 0xF); i++) {
		script << (unsigned char)0x0;
	}
	
	script.close();
}

int Script::getIndexInIDPool(std::string ID) {
	for (int j = 0; j < this->IDPool.size(); j++) {
		std::string str = this->IDPool.at(j);
		if (strcmp(str.c_str(), ID.c_str()) == 0) return j;
	}
	return -1;
}

std::vector<unsigned char> Script::generateCodeSection() {
	std::vector<unsigned char> code;

	// Add _main_ function separately, since it's spacer values are different
	code.push_back(0x5E); // spacer for _main_
	std::vector<unsigned char> _main_ = this->functionPool.at(0).getRawCode();
	if (_main_.size() != 0) {
		code.insert(code.end(), _main_.begin(), _main_.end());
		code.push_back(0x5E); // spacer for _main_
	}

	for (int i = 1; i < this->functionPool.size(); i++) {
		Function& f = this->functionPool.at(i);
		std::vector<unsigned char> funcCode = f.getRawCode();
		code.insert(code.end(), funcCode.begin(), funcCode.end());
		code.push_back(0x4B); // spacer
	}
	while (code.size() % 4 != 0) code.push_back(0x0); // make sure code is aligned

	// Add Section Header
	unsigned int codeSize = _main_.size() == 0 ? 1 : code.size();

	addValueToVectorStart(code, 4, codeSize); // Size of code in bytes
	addValueToVectorStart(code, 4, 0); // Blank field
	addValueToVectorStart(code, 4, 0xC); // Offset to code data, always 0xC since header is fixed size

	return code;
}
std::vector<unsigned char> Script::generateIntPoolSection() {
	std::vector<unsigned char> intPool;

	for (int i : this->intPool) {
		Integer4 int4;
		int4.integer = i;
		if (this->littleEndian) {
			for (int j = 3; j >= 0; j--) intPool.push_back(int4.bytes[j]);
		}
		else {
			for (int j = 0; j < 4; j++) intPool.push_back(int4.bytes[j]);
		}
	}

	// add section header
	addValueToVectorStart(intPool, 4, this->intPool.size()); // number of ints in int pool
	addValueToVectorStart(intPool, 4, 8); // Offset to int array is always 8

	return intPool;
}
std::vector<unsigned char> Script::generateFixedPoolSection() {
	std::vector<unsigned char> fixedPool;

	for (float f : this->fixedPool) {
		Float afloat;
		afloat.f = f;
		if (this->littleEndian) {
			for (int j = 3; j >= 0; j--) fixedPool.push_back(afloat.bytes[j]);
		}
		else {
			for (int j = 0; j < 4; j++) fixedPool.push_back(afloat.bytes[j]);
		}
	}

	// add section header
	addValueToVectorStart(fixedPool, 4, this->fixedPool.size()); // number of floats in fixed pool
	addValueToVectorStart(fixedPool, 4, 8); // Offset to float array is always 8

	return fixedPool;
}
std::vector<unsigned char> Script::generateStringSection(std::vector<std::string> input) {
	std::vector<unsigned char> stringPool;

	unsigned int numStrings = input.size();

	std::vector<int> offsetArr;
	std::vector<unsigned char> stringData;

	// Populates offsetArr and stringData
	int currOffset = 0;
	for (int i = 0; i < numStrings; i++) {
		offsetArr.push_back(currOffset);
		currOffset += strlen(input.at(i).c_str()) + 1; // strlen doesn't include null terminator byte, so need to add 1

		for (char c : input.at(i)) stringData.push_back(c);
		stringData.push_back(0); // null terminator
	}

	// pad stringData to be a multiple of 4
	while (stringData.size() % 4 != 0) stringData.push_back(0);

	// encrypt string data
	for (int i = 0; i < stringData.size(); i += 4) encryptBytes(stringData.data(), i);

	// Add offset array and ID data to IDPool vector
	unsigned int offsetElementSize = 2;
	if (offsetArr.size() != 0 && (offsetArr.at(offsetArr.size() - 1) + (offsetArr.size() * offsetElementSize)) > 0xFFFF) offsetElementSize = 4;
	for (int c : offsetArr) {
		addValueToVector(stringPool, offsetElementSize, c + (offsetArr.size() * offsetElementSize));
	}
	stringPool.insert(stringPool.end(), stringData.begin(), stringData.end());

	// add section header
	addValueToVectorStart(stringPool, 4, offsetElementSize); // Size in bytes of elements in offset array
	addValueToVectorStart(stringPool, 4, numStrings); // number of strings in section
	addValueToVectorStart(stringPool, 4, 0xC); // Offset to code data, always 0xC since header is fixed size

	return stringPool;
}
std::vector<unsigned char> Script::generateFunctionPoolSection() {
	std::vector<unsigned char> functionPool;

	// Create and populate an array of function data
	std::vector<unsigned char> functionData;
	for (int i = 0; i < this->functionPool.size(); i++) {
		Function func = this->functionPool.at(i);

		// Get index in ID Pool for function name
		int nameIndex = getIndexInIDPool(func.getName());
		if (nameIndex == -1) throw std::runtime_error("Function name " + func.getName() + " is not in ID Pool");
		
		addValueToVector(functionData, 2, nameIndex);
		addValueToVector(functionData, 2, func.getArgs());
		addValueToVector(functionData, 2, func.getField4());
		addValueToVector(functionData, 2, func.getField6());
		addValueToVector(functionData, 2, func.getLocalPoolIndex());
		addValueToVector(functionData, 2, func.getField10());
		addValueToVector(functionData, 4, func.getStart());
		addValueToVector(functionData, 4, func.getEnd());
	}

	addValueToVector(functionPool, 4, 0xC); // offset to function data is always 0xC
	addValueToVector(functionPool, 4, this->functionPool.size()); // add number of functions
	addValueToVector(functionPool, 4, 0x14); // size of function data is always 0x14
	functionPool.insert(functionPool.end(), functionData.begin(), functionData.end());

	return functionPool;
}
std::vector<unsigned char> Script::generatePluginImportsSection() {
	std::vector<unsigned char> pluginImports;

	for (PluginImport pi : this->pluginImports) {
		int pluginIndex = getIndexInIDPool(pi.getPluginName());
		int functionIndex = getIndexInIDPool(pi.getFunctionName());
		if (pluginIndex == -1) throw std::runtime_error("Plugin name " + pi.getPluginName() + " is not in ID Pool");
		if (functionIndex == -1) throw std::runtime_error("Function name " + pi.getPluginName() + " is not in ID Pool");

		addValueToVector(pluginImports, 2, pluginIndex);
		addValueToVector(pluginImports, 2, functionIndex);
	}

	addValueToVectorStart(pluginImports, 4, 4);
	addValueToVectorStart(pluginImports, 4, this->pluginImports.size());
	addValueToVectorStart(pluginImports, 4, 0xC);

	return pluginImports;
}
std::vector<unsigned char> Script::generateOCImportsSection() {
	std::vector<unsigned char> OCImports;

	for (std::string OC : this->OCImports) {
		int OCIndex = getIndexInIDPool(OC);
		if (OCIndex == -1) throw std::runtime_error("OC Import " + OC + " is not in ID Pool");
		addValueToVector(OCImports, 2, OCIndex);
	}

	// pad to multiple of 4
	while (OCImports.size() % 4 != 0) OCImports.push_back(0);

	addValueToVectorStart(OCImports, 4, 2); // size in bytes of elements
	addValueToVectorStart(OCImports, 4, this->OCImports.size()); // number of OC Imports
	addValueToVectorStart(OCImports, 4, 0xC); // offset to data is always 0xC

	return OCImports;
}
std::vector<unsigned char> Script::generateFunctionImportsSection() {
	std::vector<unsigned char> functionImports;

	// Note: no xb1 scripts have function imports
	addValueToVector(functionImports, 4, 0xC); // offset to data 
	addValueToVector(functionImports, 4, 0); // number of function imports
	addValueToVector(functionImports, 4, 4); // size of function import

	return functionImports;
}
std::vector<unsigned char> Script::generateStaticVariablesSection() {
	std::vector<unsigned char> staticVariables;

	for (Object o : this->staticVariables) {
		addValueToVector(staticVariables, 1, o.getTypeEnum());
		staticVariables.push_back(0);
		addValueToVector(staticVariables, 2, o.getLength());
		addValueToVector(staticVariables, 4, o.getValue());
		if (this->is64Bit) addValueToVector(staticVariables, 4, o.getField8());
	}

	addValueToVectorStart(staticVariables, 4, this->staticVariables.size()); // number of static vars
	addValueToVectorStart(staticVariables, 4, 8); // offset to static vars

	return staticVariables;
}
std::vector<unsigned char> Script::generateLocalPoolSection() {
	std::vector<unsigned char> localPool;

	std::vector<int> offsetArr;
	std::vector<unsigned char> localData;

	unsigned int currIndex = 0;
	unsigned int currOffset = 0;
	for (Function& f : this->functionPool) {
		if (f.getLocalPool().size() == 0) continue; // skip functions with no local pool

		offsetArr.push_back(currOffset);
		addValueToVector(localData, 4, 8); // offset to local object is always 8
		addValueToVector(localData, 4, f.getLocalPool().size()); // local pool size
		currOffset += 8;
		for (Object o : f.getLocalPool()) {
			addValueToVector(localData, 1, o.getTypeEnum());

			// if local var is in an array, this byte is B5 for some reason
			if (o.isInArray()) localData.push_back(0xB5);
			else localData.push_back(0);

			addValueToVector(localData, 2, o.getLength());
			addValueToVector(localData, 4, o.getValue());

			if (this->is64Bit) {
				addValueToVector(localData, 4, o.getField8());
				currOffset += 12;
			}
			else currOffset += 8;
		}

		f.setLocalPoolIndex(currIndex++); // store local pool index in function, to generate the function pool later
	}

	unsigned int extraBytes = (offsetArr.size() * 2) % 4; // Assume offsetNumBytes is 2 here; if offsetNumBytes needs to be 4, then we don't need any extra bytes

	unsigned int offsetNumBytes = 2;
	if (offsetArr.size() != 0 && (offsetArr.at(offsetArr.size() - 1) + (offsetArr.size() * offsetNumBytes) + (extraBytes * offsetNumBytes)) > 0xFFFF) offsetNumBytes = 4;

	addValueToVector(localPool, 4, 0xC); // offset to data is always 0xC
	addValueToVector(localPool, 4, offsetArr.size()); // number of elements in offset array
	addValueToVector(localPool, 4, offsetNumBytes); // size in bytes of elements in offset array

	// add offsetArr.size()*offsetNumBytes to each index, and add to localPool
	if (offsetNumBytes == 2) {
		for (int c : offsetArr) addValueToVector(localPool, offsetNumBytes, c + (offsetArr.size() * offsetNumBytes) + extraBytes); // need to add extra padding bytes to offsetArr elements
		// add extra padding bytes
		for (int i = 0; i < extraBytes; i++) localPool.push_back(0);
	}
	else {
		for (int c : offsetArr) addValueToVector(localPool, offsetNumBytes, c + (offsetArr.size() * offsetNumBytes)); // no need to add extra padding bytes to offsets here, since it will always be a multiple of 4
	}

	// append localData to localPool
	localPool.insert(localPool.end(), localData.begin(), localData.end());

	// padding at the end
	while (localPool.size() % 4 != 0) localPool.push_back(0);

	return localPool;
}
std::vector<unsigned char> Script::generateSystemAttributesSection() {
	std::vector<unsigned char> systemAttributes;

	addValueToVector(systemAttributes, 4, 0xC); // offset to data
	addValueToVector(systemAttributes, 4, this->systemAttributePool.size()); // number of elements in array
	addValueToVector(systemAttributes, 4, 2); // size in bytes of elements in array

	for (std::string str : this->systemAttributePool) {
		if (strcmp(str.c_str(), "") == 0) {
			addValueToVector(systemAttributes, 2, 0xFFFF);
			continue;
		}

		int index = getIndexInIDPool(str);
		if (index == -1) throw std::runtime_error("System Attribute " + str + " is not in ID Pool");
		addValueToVector(systemAttributes, 2, index);
	}

	return systemAttributes;
}
std::vector<unsigned char> Script::generateUserAttributesSection() {
	std::vector<unsigned char> userAttributes;

	addValueToVector(userAttributes, 4, 0xC);
	addValueToVector(userAttributes, 4, this->userAttributePool.size());
	addValueToVector(userAttributes, 4, 4);

	return userAttributes;
}
