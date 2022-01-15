#pragma once
#include"framework.h"
#include"utils.h"
#include"D2DRenderTarget.h"
class CD2DSolidBrush
{
	ID2D1SolidColorBrush* m_lpBrush;
public:
	CD2DSolidBrush(CD2DHwndRenderTarget& t, D2D1::ColorF color);
	CD2DSolidBrush(CD2DImageRenderTarget& t, D2D1::ColorF color);
	~CD2DSolidBrush() {
		Release();
	}
	void Release() {
		SAFE_RELEASE(m_lpBrush);
	}

	ID2D1SolidColorBrush* GetD2DSolidBrush() {
		return m_lpBrush;
	}
};

typedef std::shared_ptr< CD2DSolidBrush> brush_ptr;