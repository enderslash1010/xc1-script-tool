#include "PluginImport.h"
#include <string>

using namespace std;

PluginImport::PluginImport(string pluginName, string functionName)
{
	this->pluginName = pluginName;
	this->funtionName = functionName;
}

string PluginImport::getPluginName()
{
	return this->pluginName;
}

string PluginImport::getFunctionName()
{
	return this->funtionName;
}
