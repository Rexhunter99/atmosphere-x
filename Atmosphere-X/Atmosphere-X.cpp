// Carnivores-Engine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Atmosphere-X.h"
#include "Window.h"
#include "ExceptionDialog.h"

#include <thread>
#include <iostream>

void WindowEventShown(bool cmdShow)
{
	MessageBoxA(0, "Window shown!", "Message", MB_OK);
}

void RenderThread(void)
{
	//Window window;
	std::tstring title = TEXT("Carnivores");

	try
	{
		Window window(title, Window::WF_STYLE_DEFAULT, Window::WF_POS_DEFAULT, 900, 600);
		window.addEventListener(Window::WE_SHOW_OR_HIDE, (funcptr_t)WindowEventShown);
		window.processEventsBlocking();
	}
	catch (std::exception & e)
	{
		ExceptionDialog ed(e);
		ed.wait();
	}
}


// Forward declarations of functions included in this code module:
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

#ifdef _UNICODE
//int tmain(int argc, wchar_t **argv)
int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
#else
//int main(int argc, char **argv)
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
#endif
{
	// Start a thread that handles the graphics
	//std::thread render(RenderThread);
	RenderThread();

	// Wait for the render thread to finish, then join back to the main thread
	//render.join();
 	
	return 0; // STATUS: OK
}
