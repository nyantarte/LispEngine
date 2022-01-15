#include "DWriteFactory.h"
#include"D2D_error.h"
CDWriteFactory::CDWriteFactory() 
	:m_lpDWFactory(nullptr)
{
	HRESULT hR = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown**)&m_lpDWFactory);
	if (FAILED(hR)) {
		throw CDWFactoryCreateError();
	}
}