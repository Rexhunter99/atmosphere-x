#include "stdafx.h"
#include "Global.h"


Global::Global()
{
}


Global::~Global()
{
}


void Global::_PluginFactory::addPlugin(const std::string &name, const std::string &module, void * init_data)
{
	std::lock_guard<std::mutex> lock(this->mtx);

	this->plugins.insert(std::make_pair(name,Plugin(module,init_data)));
}

Plugin & Global::_PluginFactory::getPlugin(const std::string &name)
{
	std::lock_guard<std::mutex> lock(this->mtx);

	auto p = this->plugins.find(name);

	if (p == this->plugins.end())
		return Plugin();

	return p->second;
}

void Global::_PluginFactory::removePlugin(const std::string &name)
{
	std::lock_guard<std::mutex> lock(this->mtx);

	auto p = this->plugins.find(name);

	if (p != this->plugins.end())
		this->plugins.erase(name);
}