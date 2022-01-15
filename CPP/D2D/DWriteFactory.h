#pragma once

#include"framework.h"
#include"utils.h"
class CDWriteFactory {
	IDWriteFactory* m_lpDWFactory;
public:
	CDWriteFactory();
	~CDWriteFactory() {
		Release();
	}

	void Release() {
		SAFE_RELEASE(m_lpDWFactory);
	}

	IDWriteFactory* GetDWriteFactory() {
		return m_lpDWFactory;
	}

};