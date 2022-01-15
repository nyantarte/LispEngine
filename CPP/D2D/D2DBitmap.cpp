#include "D2DBitmap.h"
CD2DBitmap::CD2DBitmap(CD2DHwndRenderTarget& t, int w, int h) 
    :m_lpBitmap(nullptr)
{
    t.GetD2DRenderTarget()->CreateBitmap(
        D2D1::SizeU(w, h),
        D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)),
        &m_lpBitmap);
}
CD2DBitmap::CD2DBitmap(ID2D1Bitmap* lpB)
    :m_lpBitmap(lpB)
{}
CD2DBitmap::CD2DBitmap(ID2D1BitmapRenderTarget* irt)
    : m_lpBitmap(nullptr)
{
    ID2D1BitmapRenderTarget* lpTmp = nullptr;
    
    
    irt->GetBitmap(&m_lpBitmap);

}