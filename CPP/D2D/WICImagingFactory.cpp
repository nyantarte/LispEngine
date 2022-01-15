#include"WICImagingFactory.h"
#include"D2D_error.h"

CWICImageingFactory::CWICImageingFactory()
	:m_pWICImagingFactory(nullptr)
{
	HRESULT hR= CoCreateInstance(CLSID_WICImagingFactory, 
	  							 NULL, 
								 CLSCTX_INPROC_SERVER, 
								 IID_IWICImagingFactory, 
								 reinterpret_cast<void**>(&m_pWICImagingFactory));

	if (FAILED(hR)) {
		throw CCoCreateInstanceFailed();

	}

}

