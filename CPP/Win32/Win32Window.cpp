#include "Win32Window.h"
CWin32Window::CWin32Window(const TCHAR* lpszTitle, int w, int h)
	:m_wndClass(lpszTitle)
{
	m_hWnd = m_wndClass.CreateHWND(lpszTitle, w, h,this);
}
LRESULT CWin32Window::WndProc( UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY: {
		OnDestroy();
		PostQuitMessage(0);
	}break;
	case WM_PAINT:
	{
		OnPaint();
	}
		break;
	case WM_LBUTTONUP: 
	{
		OnClick(LOWORD(lParam), HIWORD(lParam));
	}break;

	default:
		return DefWindowProc(m_hWnd, message, wParam, lParam);
	}
	return 0;
}
void CWin32Window::resize(int w, int h) {
	RECT wRect;
	GetWindowRect(m_hWnd,&wRect);
	MoveWindow(m_hWnd, wRect.left, wRect.top, w, h,true);
}
void CWin32Window::repaint() {
	UpdateWindow(m_hWnd);
}