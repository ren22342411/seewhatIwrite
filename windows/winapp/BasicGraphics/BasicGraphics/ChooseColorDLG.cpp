#include "stdafx.h"
#include "ChooseColorDLG.h"

BOOL ChooseColorDLG(HWND hWnd,COLORREF &clr)
{

	CHOOSECOLOR dlgColor;
	dlgColor.lStructSize = sizeof(CHOOSECOLOR);
	dlgColor.hwndOwner = hWnd;
	dlgColor.hInstance = NULL;
	dlgColor.lpTemplateName = NULL;
	dlgColor.rgbResult =  clr;
	dlgColor.lpCustColors =  CommonColors;
	dlgColor.Flags = CC_ANYCOLOR|CC_RGBINIT;
	dlgColor.lCustData = 0;
	dlgColor.lpfnHook = NULL;

	BOOL flag = ChooseColor(&dlgColor);

	if (flag == TRUE)
	{
		clr = dlgColor.rgbResult;
	}

	return flag;
}