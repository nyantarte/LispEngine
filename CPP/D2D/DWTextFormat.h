#pragma once

#include"framework.h"
#include"utils.h"
#include"DWriteFactory.h"
class CDWTextFormat
{
	IDWriteTextFormat* m_lpTextFormat;

public:
	CDWTextFormat(CDWriteFactory& f,const TCHAR* lpsz, int size);
	~CDWTextFormat() {
		Release();
	}

	void Release() {
		SAFE_RELEASE(m_lpTextFormat);
	}

	IDWriteTextFormat* GetDWTextFormat() {
		return m_lpTextFormat;
	}
};

