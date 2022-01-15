#pragma once
#include"D2DBitmap.h"
#include"../Win32/OnIdleCallback.h"
class CD2DBitmapEx :public CD2DBitmap,COnIdleCallback{
	std::string m_filePath;

public:
	CD2DBitmapEx(std::string filePath)
		:m_filePath(filePath)
	{}



	void OnIdle();
};
