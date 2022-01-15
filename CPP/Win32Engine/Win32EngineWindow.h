#pragma once
#include"Win32EngineFramework.h"
typedef std::shared_ptr<CDWTextFormat> text_format_ptr;

class CWin32EngineWindow :
    public CWin32Window
{
#ifdef USE_D2D
    CD2DFactory m_d2dFactory;
    CD2DHwndRenderTarget m_hwndRenderTarget;

    
    CDWriteFactory m_dwFactory;
    text_format_ptr m_textFormat;



    
    typedef std::vector< image_render_target_ptr> ImageRenderTargetTbl;
    ImageRenderTargetTbl m_imageRenderTargetTbl;
    image_render_target_ptr m_curRenderTarget;
#endif
    
public:
    CWin32EngineWindow(const TCHAR* lpszTitle, int w = CW_USEDEFAULT, int h = CW_USEDEFAULT);
    CD2DHwndRenderTarget& GetD2DHwndRenderTarget() {
        return m_hwndRenderTarget;

    }
    
    image_render_target_ptr GetCurRenderTarget() {
        return m_curRenderTarget;
    }
    image_render_target_ptr GetRenderTarget(int i) {
        return m_imageRenderTargetTbl[i];

    }

    text_format_ptr GetTextFormat() {
        return m_textFormat;
    }

    void GetSize(SIZE& s);
    
    bool isClosed () {
        return nullptr == (CWin32Window*)GetWindowLong(GetHWND(), GWL_USERDATA);
    }
    
    
protected:
    virtual void OnDestroy();
    virtual void OnPaint();
    virtual void OnClick(int x, int y);
};

