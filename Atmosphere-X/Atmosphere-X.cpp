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
Plugin		g_renderer_plugin;

// Process the UI/UX and Renderer
void RenderThread(void)
{
	Global *g = Global::getGlobal();

	//Window window;
	std::tstring title = TEXT("Carnivores");

	try
	{
		// Create the main window
		g_main_window = Window(title, Window::WF_STYLE_DEFAULT, Window::WF_POS_DEFAULT, 900, 600);

		// Load the renderer library
		g->plugin_mutex.lock();
		Plugin p("v_gl2.dll", nullptr);
		g->plugins.insert(std::make_pair("renderer", p));
		IRenderer *renderer = (IRenderer*)p.getInstance();
		g->plugin_mutex.unlock();

		// Begin processing events
		g_main_window.processEventsBlocking();
	}
	catch (std::exception & e)
	{
		ExceptionDialog ed(e);
		ed.wait();
	}
}


#ifdef _WINDOWS
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow) {
	#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif
#elif defined(_LINUX)
int main(int argc, char **argv) {
#endif
	// Start a thread that handles the graphics
	std::thread render(RenderThread);

	// Wait for the render thread to finish, then join back to the main thread
	render.join();
 	
	return 0; // STATUS: OK
}
