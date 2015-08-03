#pragma once

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <cstdint>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "Plugin.h"


class Global
{
public:
	~Global();

	static Global * getGlobal()
	{
		static Global g;
		return &g;
	}
	/*
	// Return reference version of above static method
	static Global & getGlobal()
	{
		static Global g;
		return g;
	}
	*/

	// Member variables
	std::mutex							plugin_mutex;
	std::map<std::string, Plugin>		plugins;

private:
	Global();
};

#endif //__GLOBAL_H__
