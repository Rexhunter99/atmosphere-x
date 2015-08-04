// Only compile this in a Windows build target

#include "stdafx.h"
#include "Resource.h"
#include "Window.h"

// Helper macro to shorten function pointer calls
#define _run( f ) if ( f != nullptr ) \
			f
#define _to_hwnd( h ) reinterpret_cast<HWND>(h)


WNDCLASSEX	wcex;
ATOM		classAtom = 0;


void WindowEventResponse( uint32_t message, uint16_t param16, uint32_t param32, const Window * window )
{
	if (window == nullptr)
		return;

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
				if (f)
					f(param16>0);
			}
		}
		break;

	case WM_MOVE:
		if (window->_event[Window::WE_SHOW_OR_HIDE])
		{
			// Recast the generic function pointer to one that takes a bool parameter
			void(*f)(int16_t, int16_t) = reinterpret_cast<void(*)(int16_t, int16_t)>(window->_event[Window::WE_MOVED]);
			// Execute the function with the bool parameter
			if (f)
				f(LOWORD(param32), HIWORD(param32));
		}
		break;

	case WM_SIZE:
		if (window->_event[Window::WE_SHOW_OR_HIDE])
		{
			// Recast the generic function pointer to one that takes a bool parameter
			void(*f)(uint16_t,uint16_t) = reinterpret_cast<void(*)(uint16_t, uint16_t)>(window->_event[Window::WE_RESIZED]);
			// Execute the function with the bool parameter
			if (f)
				f(LOWORD(param32), HIWORD(param32));
		}
		break;

	default:
		return;
	}
}


LRESULT WINAPI _WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Window *i = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	// -- Catch a CREATE event to prevent nullptr exception
	if ( msg == WM_NCCREATE )
	{
		LPCREATESTRUCT cs = (LPCREATESTRUCT)lparam;
		// -- Assign the lparam (Window instance pointer) to the window
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams );
		i = reinterpret_cast<Window*>(cs->lpCreateParams);
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
	this->_native_gc = nullptr;
	ZeroMemory( &this->_event[0], sizeof(funcptr_t) * WE_MAX_VALUE );
	_event[0] = nullptr;
}


Window::Window(Window && src)
{
	this->_valid = src._valid;
	this->_native_handle = src._native_handle;
	this->_native_gc = src._native_gc;
	CopyMemory(this->_event, src._event, sizeof(funcptr_t) * WE_MAX_VALUE);
	SetWindowLongPtr((HWND)this->_native_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this) );

	src._valid = false;
	src._native_handle = nullptr;
	src._native_gc = nullptr;
	ZeroMemory( src._event, sizeof(funcptr_t) * WE_MAX_VALUE );
}


Window::Window(const std::tstring & title, int style, int position, uint16_t width, uint16_t height)
{
	this->_valid = false;
	ZeroMemory(&this->_event[0], sizeof(funcptr_t) * WE_MAX_VALUE );

	_initWindowClassEx();

	DWORD	exwinstyle = 0,
			winstyle = 0;
	int		winx = 0,
			winy = 0;

	// Set the styles
	if ( style == WF_DEFAULT )
	{
		winstyle = WS_VISIBLE | WS_BORDER | WS_SIZEBOX | WS_CAPTION | WS_SYSMENU;
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
		winx = (GetSystemMetrics(SM_CXSCREEN) / 2) - (width / 2);
		winy = (GetSystemMetrics(SM_CYSCREEN) / 2) - (height / 2);
	}

	// Create the window
	_native_handle = CreateWindowEx(exwinstyle,
									wcex.lpszClassName,
									title.c_str(),
									winstyle,
									winx, winy, width, height,
									HWND_DESKTOP,
									nullptr, wcex.hInstance, (void*)this );

	if ( IsWindow((HWND)_native_handle) )
	{
		this->_valid = true;
		this->_native_gc = GetDC(_to_hwnd(_native_handle));
	}
	else
	{
		THROW_RUNTIME_ERROR( "Failed to create the window!" );
	}

}


Window & Window::operator = (Window & src)
{
	this->_valid = src._valid;
	this->_native_handle = src._native_handle;
	this->_native_gc = src._native_gc;
	CopyMemory(this->_event, src._event, sizeof(funcptr_t) * WE_MAX_VALUE);
	SetWindowLongPtr((HWND)this->_native_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	src._valid = false;
	src._native_handle = nullptr;
	src._native_gc = nullptr;
	ZeroMemory(src._event, sizeof(funcptr_t) * WE_MAX_VALUE);

	return *this;
}


Window::~Window(void)
{
	HWND hwnd = _to_hwnd(_native_handle);
	if ( _valid && IsWindow(hwnd) )
	{
		ReleaseDC(hwnd, (HDC)_native_gc);
		DestroyWindow(hwnd);

		UnregisterClass(wcex.lpszClassName, wcex.hInstance);
		classAtom = 0;
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

	ShowWindow((HWND)_native_handle, SW_HIDE);
	ShowWindow((HWND)_native_handle, SW_SHOW);

	while ((result = GetMessage(&msg, (HWND)_native_handle, 0, 0)))
	{
		if ( result == -1 )
		{
			// TODO: Throw an exception here
			DWORD err = GetLastError();
			THROW_RUNTIME_ERROR(std::string("There was an error when getting the messasge on top of the message queue with code: ") + std::to_string(err));
			break;
		}

		if ( msg.hwnd != (HWND)_native_handle )
			continue;

		if (msg.message == WM_DESTROY)
			_valid = false;
		if (msg.message == WM_QUIT)
			break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


void Window::addEventListener(unsigned event, funcptr_t callback)
{
	if (this->_valid)
	{
		this->_event[event] = callback;
	}
}


void Window::setTitle(const std::tstring & title)
{
	if (!_valid)
		return;

	SetWindowText(_to_hwnd(_native_handle), title.c_str());
}

const void * Window::getNativeHandle() const
{
	return _native_handle;
}
