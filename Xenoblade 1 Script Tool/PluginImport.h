#pragma once
#include <string>

class PluginImport
{
	std::string pluginName;
	std::string functionName;

public:
	PluginImport(std::string pluginName, std::string functionName);

	std::string getPluginName(), getFunctionName();
};

