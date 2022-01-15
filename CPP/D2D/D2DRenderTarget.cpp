#include "D2DRenderTarget.h"
#include "D2DSolidBrush.h"
#include "D2DBitmap.h"
#include"D2D_error.h"


void CD2DRenderTargetBase::Clear() {


	m_lpRenderTarget->Clear(m_clearColor);

}
void CD2DRenderTargetBase::DrawText(int x, int y, const TCHAR* lpszText, CDWTextFormat& f) {

	size_t len = wcslen(lpszText);
	D2D1_RECT_F drawRect = D2D1::RectF(x, y,
		x+len * f.GetDWTextFormat()->GetFontSize(),
		y+f.GetDWTextFormat()->GetFontSize());

	m_lpRenderTarget->DrawText(lpszText,
		len,
		f.GetDWTextFormat(),
		&drawRect,
		m_curBrush->GetD2DSolidBrush(),
		D2D1_DRAW_TEXT_OPTIONS_NONE);


}
void CD2DRenderTargetBase::DrawLine(int x1, int y1, int x2, int y2, float lineWidth) {
	D2D1_POINT_2F p1 = D2D1::Point2F((FLOAT)x1, (FLOAT)y1);
	D2D1_POINT_2F p2 = D2D1::Point2F((FLOAT)x2, (FLOAT)y2);

	m_lpRenderTarget->DrawLine(p1, p2, m_curBrush->GetD2DSolidBrush(), (FLOAT)lineWidth);
}
void CD2DRenderTargetBase::DrawEllipse(int x, int y, int xrad, int yrad, float lineWidth) {
	D2D1_POINT_2F pos = D2D1::Point2F((FLOAT)x, (FLOAT)y);
	D2D1_ELLIPSE e = D2D1::Ellipse(pos, (FLOAT)xrad, (FLOAT)yrad);
	m_lpRenderTarget->DrawEllipse(&e, m_curBrush->GetD2DSolidBrush(), (FLOAT)lineWidth);
}
void CD2DRenderTargetBase::DrawRectangle(int l, int t, int w, int h, float lineWidth) {

	D2D1_RECT_F r = D2D1::RectF((FLOAT)l, (FLOAT)t, (FLOAT)(l + w), (FLOAT)(t + h));
	m_lpRenderTarget->DrawRectangle(&r, m_curBrush->GetD2DSolidBrush(), (FLOAT)lineWidth);
}
void CD2DRenderTargetBase::FillRectangle(int l, int t, int w, int h) {
	D2D1_RECT_F r = D2D1::RectF((FLOAT)l, (FLOAT)t, (FLOAT)(l + w), (FLOAT)(t + h));
	m_lpRenderTarget->FillRectangle(&r, m_curBrush->GetD2DSolidBrush());


}
void CD2DRenderTargetBase::DrawBitmap(CD2DBitmap& bmp) {
	D2D1_SIZE_U size = bmp.GetSize();
	m_lpRenderTarget->DrawBitmap(bmp.GetBitmap(),
		D2D1::RectF(0.0f, 0.0f, size.width, size.height),
		1.0f, //alpha blending multiplier,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,//D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, //interpolation mode,
		D2D1::RectF(0.0f, 0.0f, size.width, size.height));
}
void CD2DRenderTargetBase::DrawBitmap(CD2DBitmap& bmp, int l, int t, int w, int h) {
	D2D1_SIZE_U size = bmp.GetSize();
	m_lpRenderTarget->DrawBitmap(bmp.GetBitmap(),
		D2D1::RectF(l,t,w,h),
		1.0f, //alpha blending multiplier,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,//D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, //interpolation mode,
		D2D1::RectF(0.0f, 0.0f, size.width, size.height));
}

image_ptr CD2DRenderTargetBase::LoadFromFile(CWICImageingFactory& factory, TCHAR* lpszFileName) {
	

	IWICBitmapDecoder* pWICBitmapDecoder = NULL;
	IWICBitmapFrameDecode* pWICBitmapFrame = NULL;
	IWICFormatConverter* pFormatConverter = NULL;

	ID2D1Bitmap* pBitmap = NULL;
	enum {
		OK = 0,
		FAILED_CREATE_DECODER,
		FAILED_GET_FRAME,
		FAILED_CREATE_FORMAT_CONVERTER,
		FAILED_FORMAT_CONVERTER_INITIALIZE,
		FAILED_CREATE_BITMAP,

	};
	int result = OK;

	HRESULT hR = factory.GetWICImagingFactory()->CreateDecoderFromFilename(
		lpszFileName
		, NULL
		, GENERIC_READ
		, WICDecodeMetadataCacheOnLoad
		, &pWICBitmapDecoder
	);


	if (FAILED(hR)) {
		result = FAILED_CREATE_DECODER;
		goto Finish;
	}


	hR = pWICBitmapDecoder->GetFrame(0, &pWICBitmapFrame);
	if (FAILED(hR)) {
		result = FAILED_GET_FRAME;
		goto Finish;

	}



	hR = factory.GetWICImagingFactory()->CreateFormatConverter(&pFormatConverter);
	if (FAILED(hR)) {

		result = FAILED_CREATE_FORMAT_CONVERTER;
		goto Finish;
	}


	hR = pFormatConverter->Initialize(
		pWICBitmapFrame                   // BitmapSource
		, GUID_WICPixelFormat32bppPBGRA     // ピクセルフォーマット
		, WICBitmapDitherTypeNone           // BitmapDitherType
		, NULL                              // バレット
		, 1.0f                              // 透過率
		, WICBitmapPaletteTypeMedianCut     // パレットタイプ
	);
	if (FAILED(hR)) {
		result = FAILED_FORMAT_CONVERTER_INITIALIZE;
		goto Finish;

	}


	hR = m_lpRenderTarget->CreateBitmapFromWicBitmap(pFormatConverter, NULL, &pBitmap);
	if (FAILED(hR)) {
		result = FAILED_CREATE_BITMAP;
		goto Finish;


	}
	
Finish:
	SAFE_RELEASE(pWICBitmapDecoder);
	SAFE_RELEASE(pWICBitmapFrame);
	SAFE_RELEASE(pFormatConverter);


	switch (result) {
	case FAILED_CREATE_DECODER:
		throw CWICCreateDecorderError();
	case FAILED_GET_FRAME:
		throw CWICGetFrameError();
	case FAILED_CREATE_FORMAT_CONVERTER:
		throw CWICCreateFormatConverterError();
	case FAILED_FORMAT_CONVERTER_INITIALIZE:

		throw CWICFormatConverterInitError();
	case FAILED_CREATE_BITMAP:
		throw CD2DCreateBitmapError();
	default:;
	}
	return image_ptr(new CD2DBitmap(pBitmap));
}

CD2DHwndRenderTarget::CD2DHwndRenderTarget(CD2DFactory& f, HWND hTarget, int w, int h)
	:CD2DRenderTargetBase()
{
	ZeroMemory(&m_pixelFormat, sizeof(m_pixelFormat));
	ZeroMemory(&m_renderTargetProperties, sizeof(m_renderTargetProperties));
	ZeroMemory(&m_hwndRenderTargetProperties, sizeof(m_hwndRenderTargetProperties));

	D2D1_SIZE_U renderTargetSize = { (UINT32)w,(UINT32)h };

	m_pixelFormat.format = DXGI_FORMAT_UNKNOWN;
	m_pixelFormat.alphaMode = D2D1_ALPHA_MODE_UNKNOWN;

	m_renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	m_renderTargetProperties.pixelFormat = m_pixelFormat;
	m_renderTargetProperties.dpiX = 0;
	m_renderTargetProperties.dpiY = 0;
	m_renderTargetProperties.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	m_renderTargetProperties.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	m_hwndRenderTargetProperties.hwnd = hTarget;
	m_hwndRenderTargetProperties.pixelSize = renderTargetSize;
	m_hwndRenderTargetProperties.presentOptions = D2D1_PRESENT_OPTIONS_NONE;

	ID2D1HwndRenderTarget* lpTmp = nullptr;

	HRESULT hr = f.GetD2D1Factory()->CreateHwndRenderTarget(
		&m_renderTargetProperties, &m_hwndRenderTargetProperties,
		&lpTmp
	);
	if (FAILED(hr)) {
		throw CD2DRenderTargetCreateError();
	}
	hr = lpTmp->QueryInterface(IID_ID2D1RenderTarget, (void**)&m_lpRenderTarget);
	if (FAILED(hr)) {
		throw CQueryInterfaceFailed();

	}

	m_curBrush.reset(new CD2DSolidBrush(*this, D2D1::ColorF(D2D1::ColorF::White)));
}

CD2DImageRenderTarget::CD2DImageRenderTarget(CD2DHwndRenderTarget& rt, int w, int h)
	:CD2DRenderTargetBase()
{
	D2D1_SIZE_F renderTargetSize = { w,h };
	ID2D1BitmapRenderTarget* lpTmp=nullptr;
	HRESULT hr=rt.GetD2DRenderTarget()->CreateCompatibleRenderTarget(
		renderTargetSize,
		&lpTmp);

	if (FAILED(hr)) {
		throw CD2DRenderTargetCreateError();
	}
	m_bitmap.reset(new CD2DBitmap(lpTmp));

	hr = lpTmp->QueryInterface(IID_ID2D1RenderTarget, (void**)&m_lpRenderTarget);

	SAFE_RELEASE(lpTmp);
	if (FAILED(hr)) {
		throw CQueryInterfaceFailed();

	}

	m_curBrush.reset(new CD2DSolidBrush(*this, D2D1::ColorF(D2D1::ColorF::White)));
}
CD2DImageRenderTarget::CD2DImageRenderTarget(CD2DHwndRenderTarget& rt)
	:CD2DRenderTargetBase()
{

	ID2D1BitmapRenderTarget* lpTmp = nullptr;
	HRESULT hr = rt.GetD2DRenderTarget()->CreateCompatibleRenderTarget(
		&lpTmp);

	if (FAILED(hr)) {
		throw CD2DRenderTargetCreateError();
	}
	
	m_bitmap.reset(new CD2DBitmap(lpTmp));

	
	hr = lpTmp->QueryInterface(IID_ID2D1RenderTarget, (void**)&m_lpRenderTarget);
	
	SAFE_RELEASE(lpTmp);
	if (FAILED(hr)) {
		throw CQueryInterfaceFailed();

	}
	m_curBrush.reset(new CD2DSolidBrush(*this, D2D1::ColorF(D2D1::ColorF::White)));
}
std::shared_ptr<CD2DBitmap> CD2DImageRenderTarget::GetBitmap() {
	/*ID2D1BitmapRenderTarget* lpTmp;
	HRESULT hr = m_lpRenderTarget->QueryInterface(IID_ID2D1BitmapRenderTarget, (void**)&lpTmp);
	if (FAILED(hr)) {
		throw CQueryInterfaceFailed();

	}

	image_ptr r(new CD2DBitmap(lpTmp));
	SAFE_RELEASE(lpTmp);
	return r;
	*/
	return m_bitmap;
}

