#include "Win32App.h"
CWin32App* CWin32App::s_Self=nullptr;
CWin32App::CWin32App() {
    s_Self = this;
}

int CWin32App::run() {
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        //      if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            
        }
    }
    return (int)msg.wParam;
}
void CWin32App::ProcEvents() {
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    if (PeekMessage(&msg,nullptr, 0, 0, PM_NOREMOVE)) {
    
        GetMessage(&msg, nullptr, 0, 0);
        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }
}
