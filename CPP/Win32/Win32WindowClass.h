#pragma once
#include"Win32Framework.h"
class CWin32Window;
class CWin32WindowClass
{
	WNDCLASSEXW m_wcex;
public:
	CWin32WindowClass(const TCHAR* lpszClassName);
	~CWin32WindowClass();

	HWND CreateHWND(const TCHAR* lpszWndClass, int w, int h,CWin32Window* lpW);
private:
	static LRESULT CALLBACK  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

