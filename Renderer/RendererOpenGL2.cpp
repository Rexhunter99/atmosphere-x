#include "RendererOpenGL2.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include <crtdbg.h>


#ifdef _DEBUG
#define DEBUG_NEW_PLACEMENT (_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_NEW_PLACEMENT
#endif


static RendererGL2 *	global_instance = nullptr;
static HGLRC			global_rendercontext;


typedef struct _initstruct{
	size_t		uiSize;			///<The size of this struct, this verifies the validity of the struct
	HWND		hWindow;		///<Win32 & COM Window Handle
	HDC			hDC;			///<Windows GDI Device Context
	intptr_t	ptrData;		///<Custom data pointer
} initstruct_t;


// Create an instance of the Renderer class for use in the main engine project
IRenderer * Create(void * data)
{
	if (global_instance == nullptr)
		global_instance = new (_NORMAL_BLOCK, __FILE__, __LINE__) RendererGL2(data);
	return global_instance;
}


BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved)
{
	//  Perform global initialization.
	switch (nReason)
	{
	case DLL_PROCESS_ATTACH:
		//  For optimization.
		DisableThreadLibraryCalls(hDllHandle);

		global_instance = nullptr;
		break;

	case DLL_PROCESS_DETACH:
		if (global_instance != nullptr)
			global_instance->free();
		break;
	}

	return TRUE;
}


RendererGL2::RendererGL2(void * data)
{
	initstruct_t *init_data = (initstruct_t*)data;

	if (data != nullptr)
	{
		if (init_data->uiSize != sizeof(initstruct_t))
		{
			return;
		}
	}


}


RendererGL2::~RendererGL2()
{
}


void RendererGL2::free()
{
	delete this;
}


void RendererGL2::setFullscreen(bool go_fullscreen)
{
	// Go into fullscreen mode here
}
