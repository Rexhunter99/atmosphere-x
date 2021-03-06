// Carnivores-Engine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Atmosphere-X.h"
#include "Window.h"
#include "ExceptionDialog.h"
#include "IRenderer.h"

#include <iostream>
#include <thread>


Window		g_main_window;

// Process the UI/UX and Renderer
void RenderThread(void)
{
	Global *g = Global::getGlobal();

	try
	{
		// Create the main window
		g_main_window = Window(TEXT("Carnivores"), Window::WF_STYLE_DEFAULT, Window::WF_POS_DEFAULT, 900, 600);

		// Load the renderer library
		g->PluginFactory.addPlugin("Renderer", "v_gl2", nullptr);
		Plugin &plugin_renderer = g->PluginFactory.getPlugin("Renderer");

		// Begin processing events
		g_main_window.processEventsBlocking();

		// Cleanup
		g->PluginFactory.removePlugin("Renderer");
	}
	catch (std::exception & e)
	{
		ExceptionDialog ed(e);
		ed.wait();
	}
}


#ifdef _WINDOWS

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
{
	Global *g = Global::getGlobal();
	g->CommandLine = split(std::string(lpszCmdLine), " ");

	#ifdef _DEBUG
	if (std::string(lpszCmdLine).find("-debug") != std::string::npos)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

#elif defined(_LINUX)

int main(int argc, char **argv)
{
	Global *g = Global::getGlobal();

#endif
	// Start a thread that handles the graphics
	std::thread render(RenderThread);

	// Wait for the render thread to finish, then join back to the main thread
	render.join();
 	
	return 0; // STATUS: OK
}
