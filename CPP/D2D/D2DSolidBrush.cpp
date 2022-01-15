#include "D2DSolidBrush.h"
#include "D2D_error.h"
CD2DSolidBrush::CD2DSolidBrush(CD2DHwndRenderTarget& t, D2D1::ColorF color)
	:m_lpBrush(nullptr)

{
	HRESULT hR=t.GetD2DRenderTarget()->CreateSolidColorBrush(color, &m_lpBrush);

	if (FAILED(hR)) {
		throw CD2DCreateSolidBrushError();
	}
}
CD2DSolidBrush:: CD2DSolidBrush(CD2DImageRenderTarget& t, D2D1::ColorF color) 
	:m_lpBrush(nullptr)

{
	HRESULT hR = t.GetD2DRenderTarget()->CreateSolidColorBrush(color, &m_lpBrush);

	if (FAILED(hR)) {
		throw CD2DCreateSolidBrushError();
	}
}