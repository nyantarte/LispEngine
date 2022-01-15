#pragma once
#include"Win32Framework.h"
#include"OnIdleCallback.h"
class CWin32App
{
	

public:
	static CWin32App* s_Self;
public:
	CWin32App();
	virtual int run();
	virtual void ProcEvents();

};

