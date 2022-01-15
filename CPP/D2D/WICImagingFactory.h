#pragma once
#include"framework.h"
#include"utils.h"


class CWICImageingFactory {
    IWICImagingFactory* m_pWICImagingFactory;
public:
    CWICImageingFactory();
    ~CWICImageingFactory() {
        SAFE_RELEASE(m_pWICImagingFactory);
    }
    IWICImagingFactory* GetWICImagingFactory() {
        return m_pWICImagingFactory;
    }
};