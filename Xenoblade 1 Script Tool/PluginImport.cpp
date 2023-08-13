
#include "PluginImport.h"
#include <string>

PluginImport::PluginImport(std::string pluginName, std::string functionName)
{
	this->pluginName = pluginName;
	this->functionName = functionName;
}

std::string PluginImport::getPluginName()
{
	return this->pluginName;
}

std::string PluginImport::getFunctionName()
{
	return this->functionName;
}
