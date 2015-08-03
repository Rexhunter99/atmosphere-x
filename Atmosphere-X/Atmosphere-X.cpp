// Carnivores-Engine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Atmosphere-X.h"
#include "Window.h"
#include "ExceptionDialog.h"
#include "IRenderer.h"
#include "Plugin.h"

#include <iostream>
#include <thread>
#include <atomic>


std::atomic<Window>		g_main_window;
std::atomic<Plugin>		g_renderer_plugin;

// Process the UI/UX and Renderer
void RenderThread(void)
{
	//Window window;
	std::tstring title = TEXT("Carnivores");

	try
	{
		// Create the main window
		Window window(title, Window::WF_STYLE_DEFAULT, Window::WF_POS_DEFAULT, 900, 600);

		// Load the renderer library
		Plugin plugin_renderer("v_gl2.dll", nullptr);
		IRenderer *renderer = (IRenderer*)plugin_renderer.getInstance();

		// Begin processing events
		window.processEventsBlocking();
	}
	catch (std::exception & e)
	{
		ExceptionDialog ed(e);
		ed.wait();
	}
}


#ifdef _WINDOWS
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
#elif defined(_LINUX)
int main(int argc, char **argv)
#endif
{
	// Start a thread that handles the graphics
	std::thread render(RenderThread);

	// Wait for the render thread to finish, then join back to the main thread
	render.join();
 	
	return 0; // STATUS: OK
}
