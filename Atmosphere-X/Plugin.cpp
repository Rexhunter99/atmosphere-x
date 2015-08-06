#include "stdafx.h"

#if defined( _WINDOWS )
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
	_instance = 0;
	_module = (intptr_t)LoadLibrary(std::string(module_name + ".dll").c_str());

	if (!_module)
	{
		THROW_RUNTIME_ERROR(std::string("Error Code: ") + std::to_string(GetLastError()) + std::string("\r\nFailed to load the module \"") + module_name + std::string("\""));
	}
	else
	{
		void * (*_pluginCreate)(void*) = (void * (*)(void*))GetProcAddress((HMODULE)_module, "PluginCreate");
		void * (*_pluginDelete)(void) = (void * (*)(void))GetProcAddress((HMODULE)_module, "PluginDelete");
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

#elif defined( _LINUX )

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
	_instance = 0;
	_module = (intptr_t)dlopen(std::string(module_name + ".so").c_str());

	if (!_module)
	{
		THROW_RUNTIME_ERROR(std::string("Error Code: ") + std::to_string(dlerror()) + std::string("\r\nFailed to load the module \"") + module_name + std::string("\""));
	}
	else
	{
		const char * err = nullptr;

		// Retrieve the pointer to the symbol "PluginCreate"
		void * (*_pluginCreate)(void*) = (void * (*)(void*))dlsym((void*)_module, "PluginCreate");
		if (!(err = dlerror())) _instance = (intptr_t)_pluginCreate(init_data);
		else THROW_RUNTIME_ERROR(std::string("Error Code: ") + std::to_string(err) + std::string("\r\nFailed to get the address of the module function."));

		// Retrieve the pointer to the symbol "PluginDelete"
		void * (*_pluginDelete)(void) = (void * (*)(void))dlsym((void*)_module, "PluginDelete");
		if (!(err = dlerror())) THROW_RUNTIME_ERROR(std::string("Error Code: ") + std::to_string(err) + std::string("\r\nFailed to get the address of the module function."));
	}
}


Plugin::~Plugin()
{
	if (_module)
	{
		if (_pluginDelete)
			_pluginDelete();
		dlfree((void*)_module);
	}
}


// Get a pointer to the factory/plugin class from the dynamic library
const intptr_t Plugin::getInstance()
{
	return _instance;
}


#endif