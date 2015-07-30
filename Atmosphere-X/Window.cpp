// Only compile this in a Windows build target

#include "stdafx.h"
#include "Resource.h"
#include "Window.h"

// Helper macro to shorten function pointer calls
#define _run( f ) if ( f ) \
			f

WNDCLASSEX	wcex;
ATOM		classAtom = 0;

void WindowEventResponse( uint32_t message, uint16_t param16, uint32_t param32, const Window * window )
{
	switch (message)
	{
	case WM_CREATE:
		_run(window->_event[Window::WE_CREATE])();
		break;

	case WM_CLOSE:
		_run(window->_event[Window::WE_CLOSE])();
		break;

	case WM_SHOWWINDOW:
		{
			if ( window->_event[Window::WE_SHOW_OR_HIDE] )
			{
				// Recast the generic function pointer to one that takes a bool parameter
				void (*f)(bool) = reinterpret_cast<void(*)(bool)>(window->_event[Window::WE_SHOW_OR_HIDE]);
				// Execute the function with the bool parameter
				f(param16>0);
			}
		}
		break;
	}
}


LRESULT WINAPI _WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	Window *i = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	// -- Catch a CREATE event to prevent nullptr exception
	if ( msg == WM_CREATE )
	{
		// -- Assign the lparam (Window instance pointer) to the window
		SetWindowLongPtr(hwnd, GWLP_USERDATA, lparam );
		i = reinterpret_cast<Window*>(lparam);
	}

	WindowEventResponse( msg, wparam, lparam, i );

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hwnd, &ps);
		break;

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return 0;
}

void _initWindowClassEx()
{
	if ( classAtom )
		return;

	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize				= sizeof(WNDCLASSEX);
	wcex.hInstance			= GetModuleHandle(nullptr);
	wcex.hbrBackground		= (HBRUSH)COLOR_WINDOW;
	wcex.lpszClassName		= TEXT("CARN_WINDOW_CLASS");
	wcex.hIcon				= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MYICON));
	wcex.hIconSm			= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MYICON_SMALL));
	wcex.hCursor			= LoadCursor(nullptr, IDC_ARROW);
	wcex.lpfnWndProc		= _WinProc;
	wcex.style				= CS_OWNDC;

	classAtom = RegisterClassEx(&wcex);

	if ( !classAtom )
	{
		// error output
	}
}


Window::Window(void)
{
	this->_valid = false;
	this->_native_handle = nullptr;
	ZeroMemory( _event, sizeof(funcptr_t) * WE_MAX_VALUE );
}


Window::Window(Window && src)
{
	this->_valid = src._valid;
	this->_native_handle = src._native_handle;
	CopyMemory(this->_event, src._event, sizeof(funcptr_t) * WE_MAX_VALUE);
	SetWindowLongPtr((HWND)this->_native_handle, GWLP_USERDATA, reinterpret_cast<LONG>(this) );

	src._valid = false;
	src._native_handle = nullptr;
	ZeroMemory( src._event, sizeof(funcptr_t) * WE_MAX_VALUE );
}


Window::Window(const std::tstring & title, int style, int position, uint16_t width, uint16_t height)
{
	this->_valid = false;
	ZeroMemory( _event, sizeof(funcptr_t) * WE_MAX_VALUE );

	_initWindowClassEx();

	DWORD	exwinstyle = 0,
			winstyle = 0;
	int		winx = 0,
			winy = 0;

	// Set the styles
	if ( style == WF_DEFAULT )
	{
		winstyle = WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_SYSMENU;
	}
	else if ( style == WF_STYLE_BORDERLESS )
	{
		winstyle = WS_VISIBLE;
	}
	else if ( style == WF_STYLE_MINIMAL )
	{
		winstyle = WS_VISIBLE | WS_BORDER | WS_SYSMENU;
	}
	else if ( style == WF_STYLE_SLIM )
	{
		winstyle = WS_VISIBLE | WS_BORDER | WS_SYSMENU;
	}
	else if ( style == WF_STYLE_BULKY )
	{
		winstyle = WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	}
	else if ( style == WF_STYLE_FULLSCREEN )
	{
		winstyle = WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
	}

	// Set the position
	if ( position == WF_POS_DEFAULT )
	{
		winx = CW_USEDEFAULT;
		winy = CW_USEDEFAULT;
	}
	else if ( position == WF_POS_CENTER )
	{
		winx = GetSystemMetrics(SM_CXSCREEN) / 2;
		winy = GetSystemMetrics(SM_CYSCREEN) / 2;
	}

	// Create the window
	_native_handle = CreateWindowEx(exwinstyle,
									wcex.lpszClassName, title.c_str(),
									winstyle,
									winx, winy, width, height,
									HWND_DESKTOP,
									nullptr, wcex.hInstance, (void*)this );

	if ( IsWindow((HWND)_native_handle) )
	{
		this->_valid = true;

		SetWindowLongPtr((HWND)_native_handle, GWLP_USERDATA, (LONG_PTR)this);
	}
}


Window::~Window(void)
{
	if ( _valid )
	{
		DestroyWindow( (HWND)_native_handle );
	}
}


bool Window::processEvents(void)
{
	MSG msg;

	if ( PeekMessage(&msg, (HWND)_native_handle, 0, 0, 0) )
	{

		if ( msg.message == WM_QUIT )
			return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}


void Window::processEventsBlocking(void)
{
	MSG msg;
	BOOL result;

	while ((result = GetMessage(&msg, (HWND)_native_handle, 0, 0)))
	{
		if ( result == -1 )
		{
			// TODO: Throw an exception here
			DWORD err = GetLastError();
			throw std::runtime_error(
				std::string("There was an error when getting the messasge on top of the message queue with code: ") +
				std::to_string(err)
				);
			break;
		}

		if ( msg.message == WM_QUIT )
			break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	throw std::invalid_argument("Testing exception");
}


const void * Window::getNativeHandle() const
{
	return _native_handle;
}
