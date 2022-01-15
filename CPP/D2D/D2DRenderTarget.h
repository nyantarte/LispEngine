#pragma once
#include"framework.h"
#include"utils.h"
#include"D2DFactory.h"
#include "DWTextFormat.h"
#include"WICImagingFactory.h"
class CD2DSolidBrush;
class CD2DBitmap;


class CD2DRenderTargetBase {
protected:
	ID2D1RenderTarget* m_lpRenderTarget;
	D2D1_COLOR_F m_clearColor;
	std::shared_ptr<CD2DSolidBrush> m_curBrush;
public:
	CD2DRenderTargetBase()
		:m_lpRenderTarget(nullptr),
		 m_clearColor({ 0.0f, 0.0f, 0.0f,1.0f })
	{}
	virtual ~CD2DRenderTargetBase() {
		Release();
	}
	virtual void Release() {
		SAFE_RELEASE(m_lpRenderTarget);
	}

	void Clear();
	void DrawText(int x, int y, const TCHAR* lpszText, CDWTextFormat& f);
	void DrawLine(int x1, int y1, int x2, int y2, float lineWidth);
	void DrawEllipse(int x, int y, int xrad, int yrad, float lineWidth);
	void DrawRectangle(int l, int t, int w, int h, float lineWidth);
	void FillRectangle(int l, int t, int w, int h);
	void DrawBitmap(CD2DBitmap& bmp);
	void DrawBitmap(CD2DBitmap& bmp,int l,int t,int w,int h);
	std::shared_ptr<CD2DBitmap> LoadFromFile(CWICImageingFactory& factory, TCHAR* lpszFileName);
	void BeginDraw() {
		m_lpRenderTarget->BeginDraw();
	}
	void EndDraw() {

		m_lpRenderTarget->EndDraw();
	}
	ID2D1RenderTarget* GetD2DRenderTarget() {
		return m_lpRenderTarget;
	}

	std::shared_ptr<CD2DSolidBrush> GetCurrentBrush() {
		return m_curBrush;
	}
	void SetCurrentBrush(std::shared_ptr<CD2DSolidBrush> b) {
		m_curBrush = b;
	}
};

class CD2DHwndRenderTarget:public CD2DRenderTargetBase
{
	D2D1_PIXEL_FORMAT m_pixelFormat;
	D2D1_RENDER_TARGET_PROPERTIES m_renderTargetProperties;
	D2D1_HWND_RENDER_TARGET_PROPERTIES m_hwndRenderTargetProperties;
public:
	CD2DHwndRenderTarget(CD2DFactory& f,HWND hTarget, int w, int h);

	



};

class CD2DImageRenderTarget :public CD2DRenderTargetBase {
	std::shared_ptr<CD2DBitmap> m_bitmap;
public:
	CD2DImageRenderTarget(CD2DHwndRenderTarget& rt,int w,int h);
	CD2DImageRenderTarget(CD2DHwndRenderTarget& rt);


	virtual void Release() {
		//m_bitmap.reset();
		CD2DRenderTargetBase::Release();
	}
	std::shared_ptr<CD2DBitmap> GetBitmap();


	

};

typedef std::shared_ptr< CD2DImageRenderTarget> image_render_target_ptr;