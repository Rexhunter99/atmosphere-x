// stdafx.cpp : source file that includes just the standard includes
// Carnivores-Engine.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#include "IRenderer.h"
// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

typedef struct IRenderer::_initstruct {
	size_t		uiSize;			///<The size of this struct, this verifies the validity of the struct
	HWND		hWindow;		///<Win32 & COM Window Handle
	HDC			hDC;			///<Windows GDI Device Context
	intptr_t	ptrData;		///<Custom data pointer
} IRenderer::initstruct_t;

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