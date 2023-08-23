#include "Tests.h"
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

Tests::Tests(std::string scriptDirName, std::string outfileName) {

	// Make sure scriptDirName is a valid directory
	fs::path scriptDir(scriptDirName);
	if (!fs::is_directory(scriptDir)) throw std::runtime_error(scriptDirName + " is not a directory");

	std::string testOutputPath = scriptDirName + "/TestOutput/";
	fs::remove_all(testOutputPath);
	fs::create_directory(testOutputPath);

	// Create outfile
	std::ofstream outfile(testOutputPath + outfileName + ".txt");

	// Test each script file in scriptDirName
	bool allGood = true;
	for (fs::path scriptFile : fs::directory_iterator{ scriptDir }) {
		if (fs::is_directory(scriptFile)) continue; // ignore directories

		std::string inputScriptPath = scriptDirName + "/" + scriptFile.filename().string();
		std::string outputScriptPath = testOutputPath + scriptFile.stem().string();

		
		// Instantiate Script object
		Script script(inputScriptPath);
		std::cout << "Loaded " << inputScriptPath << '\n';
		
		// Generate output script
		script.generateScriptFile(outputScriptPath);
		std::cout << "Created script file " << outputScriptPath << "\n\n";

		// Compare input and output scripts, report in outfile

		// Open both files
		std::ifstream inputScript(inputScriptPath, std::ios::binary);
		std::ifstream outputScript(outputScriptPath, std::ios::binary);

		// Iterate over output script to check for differences (since output script won't have the end padding...yet)
		char outputData, inputData;
		int currByte = 0;
		std::vector<std::string> diff;
		while (outputScript.get(outputData) && inputScript.get(inputData)) {
			if (inputData != outputData) {
				std::string msg = "\tAt " + std::to_string(currByte) + ": " + std::to_string((unsigned char)inputData) + ", " + std::to_string((unsigned char)outputData) + "\n";
				diff.push_back(msg);
				allGood = false;
			}
			currByte++;
		}
		inputScript.close();
		outputScript.close();

		// Write report to outfile
		outfile << scriptFile.filename().string() << ":";
		if (diff.size() == 0) outfile << " Good\n";
		else {
			outfile << '\n';
			for (std::string str : diff) outfile << str;
		}
	}
	if (allGood) outfile << "\nAll Good :)";
	else outfile << "Not all Good :(";
	outfile.close();
}
