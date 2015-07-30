#include "stdafx.h"
#include "Window.h"
#include "resource.h"
#include "ExceptionDialog.h"


#define _to_hwnd( h ) reinterpret_cast<HWND>(const_cast<void*>(h))


// Message handler for about box.
INT_PTR CALLBACK _DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		MessageBox(0, L"Dialog Init", L"Message", MB_OK); 
		break;

	case WM_DESTROY:
		MessageBox(0, L"Dialog Destroy", L"Message", MB_OK); 
		break;

	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDOK:
			case IDCANCEL:
				DestroyWindow(hDlg);
				break;
			}
		}
		break;

	default:
		return (INT_PTR)false;
	}

	return (INT_PTR)true;
}


ExceptionDialog::ExceptionDialog(void)
{
	_valid = false;
}


ExceptionDialog::ExceptionDialog(const std::exception & e)
{
	_valid = false;
	_native_handle = CreateDialog(
		GetModuleHandle(nullptr),
		MAKEINTRESOURCE(IDD_EXCEPTIONBOX),
		GetForegroundWindow(),//reinterpret_cast<HWND>(const_cast<void*>(w.getNativeHandle())),
		_DlgProc );

	if ( IsWindow((HWND)_native_handle) )
	{
		_valid = true;
		ShowWindow((HWND)_native_handle, SW_SHOW);
	}
	else
	{
		throw std::runtime_error(
				std::string("Line: ") +
				std::to_string(__LINE__) + std::string("\r\n") +
				std::string("Unable to create the dialog window and returned error code ") +
				std::to_string(GetLastError())
				);
	}
}


ExceptionDialog::~ExceptionDialog(void)
{
	if ( _valid )
	{
		DestroyWindow(reinterpret_cast<HWND>(_native_handle));
	}
}


int ExceptionDialog::wait()
{
	if ( !_valid )
	{
		throw std::invalid_argument("ExceptionDialog is not a valid dialog form!");
		return -1;
	}

	MSG msg;
	BOOL result;

	while ( (result = GetMessage(&msg, (HWND)_native_handle, 0, 0)) != 0 )
	{
		/*if ( result == -1 )
		{
			// Error occured
			throw std::runtime_error(
				std::string("Line: ") +
				std::to_string(__LINE__) + std::string("\r\n") +
				std::string("An error occured in GetMessage with code ") +
				std::to_string(GetLastError())
				);
			return -1;
		}
		else*/
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}
