#include "stdafx.h"
#include "ChooseFontDLG.h"

BOOL ChooseFontDLG (HWND hWnd,LOGFONT &lf)
{
	CHOOSEFONT cf ;
	cf.lStructSize    = sizeof (CHOOSEFONT) ;
	cf.hwndOwner      = hWnd ;
	cf.hDC            = NULL ;
	cf.lpLogFont      = &lf ;
	cf.iPointSize     = 0 ;
	cf.Flags          = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
	cf.rgbColors      = 0;
	cf.lCustData      = 0 ;
	cf.lpfnHook       = NULL ;
	cf.lpTemplateName = NULL ;
	cf.hInstance      = NULL ;
	cf.lpszStyle      = NULL ;
	cf.nFontType      = 0 ;               // Returned from ChooseFont
	cf.nSizeMin       = 0 ;
	cf.nSizeMax       = 0 ;

	return (BOOL)ChooseFont (&cf) ; 
}