#pragma once
#include"../Win32/win32_error.h"
class CD2DFactoryCreateError :public CWin32Error {};
class CD2DRenderTargetCreateError :public CWin32Error {};
class CD2DCreateSolidBrushError :public CWin32Error {};
class CD2DCreateBitmapError :public CWin32Error {};
class CDWFactoryCreateError :public CWin32Error {};
class CDWCreateTextFormatError :public CWin32Error {};
class CWICCreateDecorderError :public CWin32Error {};
class CWICGetFrameError :public CWin32Error {};
class CWICCreateFormatConverterError :public CWin32Error {};
class CWICFormatConverterInitError :public CWin32Error {};
