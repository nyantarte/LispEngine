#pragma once
#include"Win32Framework.h"
#include"Win32WindowClass.h"
class CWin32Window
{
	CWin32WindowClass m_wndClass;
	HWND m_hWnd;
public:
	CWin32Window(const TCHAR* lpszTitle,int w= CW_USEDEFAULT,int h= CW_USEDEFAULT);
	virtual LRESULT WndProc( UINT message, WPARAM wParam, LPARAM lParam);
	void resize(int w, int h);

	HWND GetHWND() {
		return m_hWnd;
	}

	void repaint();
protected:
	virtual void OnDestroy() { 
		SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)nullptr);
	}
	virtual void OnPaint() {}
	virtual void OnClick(int x, int y) {}
};

