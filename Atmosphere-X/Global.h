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
private:
	Global();

	class _PluginFactory
	{
	public:

		void addPlugin(const std::string &, const std::string &, void*);
		Plugin & getPlugin(const std::string &);
		void removePlugin(const std::string &);
		
	private:
		std::mutex							mtx;
		std::map<std::string, Plugin>		plugins;
	};

public:
	~Global();

	static Global * getGlobal()
	{
		static Global g;
		return &g;
	}
	/*
	// Return reference version of above static method
	static Global & getGlobalRef()
	{
		static Global g;
		return g;
	}
	*/

	// Member variables
	_PluginFactory PluginFactory;

};

#endif //__GLOBAL_H__
