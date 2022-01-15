#pragma once
#include"framework.h"
#include"utils.h"
class CD2DFactory
{
	ID2D1Factory* m_lpD2D1Factory;

public:
	CD2DFactory(bool isSingleThread = true);
	~CD2DFactory() {
		Release();
	}

	ID2D1Factory* GetD2D1Factory() {
		return m_lpD2D1Factory;
	}

	void Release() {
		SAFE_RELEASE(m_lpD2D1Factory);
	}
};

