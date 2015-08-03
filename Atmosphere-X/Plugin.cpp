#include "stdafx.h"
#include "Plugin.h"


Plugin::Plugin()
{
	_module = 0;
	_instance = 0;
}


Plugin::Plugin(Plugin && src)
{
	_module = src._module;
	_instance = src._instance;

	src._module = 0;
	src._instance = 0;
}


Plugin::Plugin(const std::string & module_name, void * init_data)
{
	_module = (intptr_t)LoadLibrary(module_name.c_str());
	_instance = 0;

	if (!_module)
	{
		THROW_RUNTIME_ERROR(std::string("Error Code: ") + std::to_string(GetLastError()) + std::string("\r\nFailed to load the module \"") + module_name + std::string("\""));
	}
	else
	{
		void * (*_pluginCreate)(void*) = (void * (*)(void*))GetProcAddress((HMODULE)_module, "Create");
		if (_pluginCreate)
		{
			_instance = (intptr_t)_pluginCreate(init_data);
		}
		else
		{
			THROW_RUNTIME_ERROR(std::string("Error Code: ") + std::to_string(GetLastError()) + std::string("\r\nFailed to get the address of the module function."));
		}
	}
}


Plugin::~Plugin()
{
	if (_module)
	{
		FreeLibrary((HMODULE)_module);
	}
}


// Get a pointer to the factory/plugin class from the dynamic library
const intptr_t Plugin::getInstance()
{
	return _instance;
}
