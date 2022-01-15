#include "DWTextFormat.h"
#include"D2D_error.h"
CDWTextFormat::CDWTextFormat(CDWriteFactory& f, const TCHAR* lpszFont, int size) 
	:m_lpTextFormat(nullptr)
{
    HRESULT hR=f.GetDWriteFactory()->CreateTextFormat(
        lpszFont
        , NULL
        , DWRITE_FONT_WEIGHT_NORMAL
        , DWRITE_FONT_STYLE_NORMAL
        , DWRITE_FONT_STRETCH_NORMAL
        , (FLOAT)size
        , L""
        , &m_lpTextFormat
    );

    if (FAILED(hR)) {
        throw CDWCreateTextFormatError();
    
    }

}