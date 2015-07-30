// Carnivores-Engine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Atmosphere-X.h"
#include "Window.h"
#include "ExceptionDialog.h"

#include <thread>


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void RenderThread(void)
{
	Window window;
	std::tstring title = TEXT("Hello World");

	std::exception te = std::bad_alloc();
	ExceptionDialog ed1(te );

		ed1.wait();
		Sleep( 5 * 1000 );

	try
	{
		window = Window(title, Window::WF_STYLE_DEFAULT, Window::WF_POS_DEFAULT, 900, 600);

		window.processEventsBlocking();
	}
	catch (std::exception & e)
	{
		ExceptionDialog ed(e);

		ed.wait();
		Sleep( 5 * 1000 );
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


/*LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		}
	}
	return 0;
}*/

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
