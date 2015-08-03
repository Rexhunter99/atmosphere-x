#include "RendererOpenGL2.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>


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
		global_instance = new RendererGL2(data);
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
		MessageBox(0, "Attaching v_gl2.dll", "v_gl2", 0);
		break;

	case DLL_PROCESS_DETACH:
		if (global_instance != nullptr)
			global_instance->free();
		MessageBox(0, "Detaching v_gl2.dll", "v_gl2", 0);
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
