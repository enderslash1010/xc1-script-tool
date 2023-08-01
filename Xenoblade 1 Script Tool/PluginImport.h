#pragma once
#include <string>

using namespace std;

class PluginImport
{
	string pluginName;
	string funtionName;

public:
	PluginImport(string pluginName, string functionName);

	string getPluginName(), getFunctionName();
};

