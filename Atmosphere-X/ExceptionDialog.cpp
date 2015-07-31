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
		break;
	case WM_CLOSE:
		DestroyWindow(hDlg);
		break;

	case WM_DESTROY:
		break;

	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDOK:
			case IDCANCEL:
			case IDABORT:
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


BOOL CenterWindow(HWND hwndWindow)
{
     HWND hwndParent;
     RECT rectWindow, rectParent;
 
     // make the window relative to its parent
     if ((hwndParent = GetParent(hwndWindow)) != NULL)
     {
         GetWindowRect(hwndWindow, &rectWindow);
         GetWindowRect(hwndParent, &rectParent);
 
         int nWidth = rectWindow.right - rectWindow.left;
         int nHeight = rectWindow.bottom - rectWindow.top;
 
         int nX = ((rectParent.right - rectParent.left) - nWidth) / 2 + rectParent.left;
         int nY = ((rectParent.bottom - rectParent.top) - nHeight) / 2 + rectParent.top;
 
         int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
         int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
 
         // make sure that the dialog box never moves outside of the screen
         if (nX < 0) nX = 0;
         if (nY < 0) nY = 0;
         if (nX + nWidth > nScreenWidth) nX = nScreenWidth - nWidth;
         if (nY + nHeight > nScreenHeight) nY = nScreenHeight - nHeight;
 
         MoveWindow(hwndWindow, nX, nY, nWidth, nHeight, FALSE);
 
         return TRUE;
     }
 
     return FALSE;
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
		HWND_DESKTOP,//reinterpret_cast<HWND>(const_cast<void*>(w.getNativeHandle())),
		_DlgProc );

	if ( IsWindow((HWND)_native_handle) )
	{
		_valid = true;
 
		SetDlgItemTextA((HWND)_native_handle, IDC_EXCEPTION_TEXT, e.what() );
		//CenterWindow( (HWND)_native_handle );
		ShowWindow((HWND)_native_handle, SW_SHOW);
	}
	else
	{
		THROW_RUNTIME_ERROR( std::string("Unable to create the dialog window and returned error code ") + std::to_string(GetLastError()) );
	}
}


ExceptionDialog::~ExceptionDialog(void)
{
	HWND hwnd = reinterpret_cast<HWND>(_native_handle);

	if ( _valid && IsWindow(hwnd) )
	{
		DestroyWindow(hwnd);
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

	while ( (result = GetMessage(&msg, (HWND)_native_handle, 0, 0)) != 0 && IsWindow((HWND)_native_handle) )
	{
		if ( result == -1 )
		{
			// Error occured
			THROW_RUNTIME_ERROR( std::string("An error occured in GetMessage with code ") + std::to_string(GetLastError()) );
			return -1;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}
