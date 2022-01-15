#include "D2DFactory.h"
#include "D2D_error.h"
CD2DFactory::CD2DFactory(bool isSingleThread)
	:m_lpD2D1Factory(nullptr)
{
	HRESULT hr = D2D1CreateFactory(
		isSingleThread ? D2D1_FACTORY_TYPE_SINGLE_THREADED: 
			D2D1_FACTORY_TYPE_MULTI_THREADED,
		&m_lpD2D1Factory);

	if (FAILED(hr)) {
		throw CD2DFactoryCreateError();
	}
}