#include "Win32WindowClass.h"
#include "Win32Window.h"
#include "win32_error.h"
CWin32WindowClass::CWin32WindowClass(const TCHAR* lpszWinClassName) {
    ZeroMemory(&m_wcex, sizeof(m_wcex));
    m_wcex.cbSize = sizeof(WNDCLASSEX);

    m_wcex.style = CS_HREDRAW | CS_VREDRAW;
    m_wcex.lpfnWndProc = &CWin32WindowClass::WndProc;
    m_wcex.cbClsExtra = 0;
    m_wcex.cbWndExtra = 0;
    m_wcex.hInstance = GetModuleHandle(nullptr);
    m_wcex.hIcon = LoadIcon(m_wcex.hInstance, IDI_APPLICATION);// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NODEGARAGE));
    m_wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    m_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    m_wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_NODEGARAGE);
    m_wcex.lpszClassName = lpszWinClassName;
    m_wcex.hIconSm = LoadIcon(m_wcex.hInstance, IDI_APPLICATION);//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    if (!RegisterClassExW(&m_wcex)) {
        throw CWin32RegisterClassFailed();
    }
}
CWin32WindowClass::~CWin32WindowClass() {
    UnregisterClass(m_wcex.lpszClassName, m_wcex.hInstance);
}
HWND CWin32WindowClass::CreateHWND(const TCHAR* lpszWndClass, int w, int h,CWin32Window* lpW) {
    HWND hWnd = CreateWindowW(lpszWndClass, lpszWndClass, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, w, h, nullptr, nullptr, m_wcex.hInstance,lpW);

    if (!hWnd)
    {
        return hWnd;
    }
    SetWindowLong(hWnd, GWL_USERDATA, (LONG)lpW);
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    return hWnd;
}
LRESULT CWin32WindowClass::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    CWin32Window* lpWin = nullptr;
    if (WM_CREATE == message) {
        CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
        lpWin = (CWin32Window*)cs->lpCreateParams;
    }
    else {
        lpWin = (CWin32Window*)GetWindowLong(hWnd, GWL_USERDATA);
    }
    if (nullptr != lpWin) {
        return lpWin->WndProc(message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
