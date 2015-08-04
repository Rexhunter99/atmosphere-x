// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include "Global.h"
#include "Plugin.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <Windows.h>

// C RunTime Header Files
#include <cstdlib>
#include <cstdint>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Include C++ RunTime Header Files
#include <array>
#include <exception>
#include <string>
#include <vector>


namespace std {
#ifdef _UNICODE
	typedef wstring tstring;
#else
	typedef string tstring;
#endif
};


#ifndef THROW_RUNTIME_ERROR
#define THROW_RUNTIME_ERROR(m) throw std::runtime_error( std::string("Line: ") + std::to_string(__LINE__) + std::string("\r\n") + std::string("File: ") + std::string(__FILE__) + std::string("\r\n") + m )
#endif

#ifdef _DEBUG
#define DEBUG_NEW_PLACEMENT (_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_NEW_PLACEMENT
#endif


std::vector<std::string> split(std::string src, const std::string & delimiter);

// TODO: reference additional headers your program requires here
