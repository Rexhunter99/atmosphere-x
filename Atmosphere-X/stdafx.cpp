// stdafx.cpp : source file that includes just the standard includes
// Carnivores-Engine.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

std::vector<std::string> split(std::string src, const std::string & delimiter)
{
	std::vector<std::string> v;

	size_t pos = 0;
	std::string token;
	while ((pos = src.find(delimiter)) != std::string::npos)
	{
		token = src.substr(0, pos);
		v.push_back(token);
		src.erase(0, pos + delimiter.length());
	}
	v.push_back(src);

	return v;
}