#pragma once

#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include <cstdint>
#include <map>
#include <string>


// Interface to load external dynamic libraries (DLL or SO) and access their implementations
class Plugin
{
public:
	Plugin();
	Plugin(Plugin &&);
	Plugin(const std::string & module_name, void * init_data);
	~Plugin();

	// Get a pointer to the factory/plugin class from the dynamic library
	const intptr_t getInstance();

private:
	Plugin(const Plugin &);

	intptr_t						_module;
	intptr_t						_instance;
	std::map<std::string, void*>	_funcs;

};

#endif //__PLUGIN_H__
