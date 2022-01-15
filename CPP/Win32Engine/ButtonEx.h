#pragma once
#include"Win32EngineFramework.h"
class CButtonEx :public D2DLib::CButton ,IOnIdleCallback{

public:
	CButtonEx(std::string caption, int tag = -1);
	~CButtonEx();

	virtual void OnIdle();

};