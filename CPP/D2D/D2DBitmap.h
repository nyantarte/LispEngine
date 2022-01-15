#pragma once

#include"framework.h"
#include"utils.h"
#include"D2DRenderTarget.h"
class CD2DBitmap
{
	ID2D1Bitmap* m_lpBitmap;
	
public:
	CD2DBitmap(CD2DHwndRenderTarget& t, int w, int h);
	CD2DBitmap(ID2D1Bitmap*);
	CD2DBitmap(ID2D1BitmapRenderTarget* irt);
	CD2DBitmap()
		:m_lpBitmap(nullptr)
	{}
	virtual ~CD2DBitmap() {
		Release();
	}

	void Release() {
		SAFE_RELEASE(m_lpBitmap);
	}
	

	D2D1_SIZE_U GetSize() {
		return  m_lpBitmap->GetPixelSize();
	}

	ID2D1Bitmap* GetBitmap() {
		return m_lpBitmap;
	}

};
typedef std::shared_ptr< CD2DBitmap> image_ptr;
