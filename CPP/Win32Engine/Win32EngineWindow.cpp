#include"Win32EngineWindow.h"
#include"Win32EngineConfig.h"
CWin32EngineWindow::CWin32EngineWindow(const TCHAR* lpszTitle, int w, int h)
	:CWin32Window(lpszTitle, w, h),
#ifdef USE_D2D
	m_d2dFactory(true),
	m_hwndRenderTarget(m_d2dFactory, GetHWND(), w, h),
	 m_dwFactory(),
	 m_textFormat(new CDWTextFormat(m_dwFactory,DEF_TEXT_FONT,DEF_TEXT_SIZE))

#endif
{
	for (int i = 0; i < GPAGE_NUM; ++i) {
		m_imageRenderTargetTbl.push_back(image_render_target_ptr(new CD2DImageRenderTarget(m_hwndRenderTarget)));
	}
	m_curRenderTarget = m_imageRenderTargetTbl[0];
}

void CWin32EngineWindow::OnDestroy() {
#ifdef USE_D2D
	m_imageRenderTargetTbl.clear();
	m_textFormat->Release();
	m_dwFactory.Release();
	m_hwndRenderTarget.Release();
	m_d2dFactory.Release();
#endif
	CWin32Window::OnDestroy();
}
void CWin32EngineWindow::OnPaint() {
	CWin32Window::OnPaint();
#ifdef USE_D2D
	
	m_hwndRenderTarget.BeginDraw();
	m_hwndRenderTarget.Clear();

	for (image_render_target_ptr irt : m_imageRenderTargetTbl) {
		m_hwndRenderTarget.DrawBitmap(*(irt->GetBitmap()));
	}
	m_hwndRenderTarget.EndDraw();
	

	
#endif

}
void CWin32EngineWindow::OnClick(int x, int y) {
	
}
void CWin32EngineWindow::GetSize(SIZE& s) {
	CRect rect;
	GetClientRect(GetHWND(), &rect);
	s.cx = rect.GetWidth();
	s.cy = rect.GetHeight();
}


