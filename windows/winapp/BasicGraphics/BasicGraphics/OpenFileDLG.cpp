#include "stdafx.h"
#include "OpenFileDLG.h"

BOOL LoadFileDLG (HWND hWnd, PTSTR pstrFileName, PTSTR pstrTitleName)//载入文件
{
	ofn4load.lStructSize = sizeof(OPENFILENAME);
	ofn4load.hwndOwner = hWnd;
	ofn4load.hInstance = NULL;
	ofn4load.lpTemplateName = NULL;
	ofn4load.lpstrFilter = L"BasicGraphics File\0*.fbg\0\0";
	ofn4load.lpstrCustomFilter = NULL;
	ofn4load.nMaxCustFilter = 0;
	ofn4load.nFilterIndex = 1;
	ofn4load.lpstrFile = NULL;
	ofn4load.nMaxFile = MAX_PATH;
	ofn4load.lpstrFileTitle = NULL;
	ofn4load.nMaxFileTitle = MAX_PATH;
	ofn4load.lpstrInitialDir = NULL;
	ofn4load.lpstrTitle = NULL;
	ofn4load.Flags = 0;
	ofn4load.nFileOffset = 0;
	ofn4load.nFileExtension = 0;
	ofn4load.lpstrDefExt = L"fbg";
	ofn4load.lCustData = 0;
	ofn4load.lpfnHook = NULL;
	ofn4load.dwReserved = 0;
	ofn4load.FlagsEx =0;
	ofn4load.hwndOwner = hWnd ;
	ofn4load.lpstrFile = pstrFileName ;
	ofn4load.lpstrFileTitle = pstrTitleName ;
	ofn4load.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT ;
	return GetOpenFileName (&ofn4load) ;
}

BOOL SaveFileDLG (HWND hWnd, PTSTR pstrFileName, PTSTR pstrTitleName)//保存文件
{
	ofn4save.lStructSize = sizeof(OPENFILENAME);
	ofn4save.hwndOwner = hWnd;
	ofn4save.hInstance = NULL;
	ofn4save.lpTemplateName = NULL;
	ofn4save.lpstrFilter = L"BasicGraphics File\0*.fbg\0\0";
	ofn4save.lpstrCustomFilter = NULL;
	ofn4save.nMaxCustFilter = 0;
	ofn4save.nFilterIndex = 1;
	ofn4save.lpstrFile = NULL;
	ofn4save.nMaxFile = MAX_PATH;
	ofn4save.lpstrFileTitle = NULL;
	ofn4save.nMaxFileTitle = MAX_PATH;
	ofn4save.lpstrInitialDir = NULL;
	ofn4save.lpstrTitle = NULL;
	ofn4save.Flags = 0;
	ofn4save.nFileOffset = 0;
	ofn4save.nFileExtension = 0;
	ofn4save.lpstrDefExt = L"fbg";
	ofn4save.lCustData = 0;
	ofn4save.lpfnHook = NULL;
	ofn4save.dwReserved = 0;
	ofn4save.FlagsEx =0;
	ofn4save.hwndOwner = hWnd ;
	ofn4save.lpstrFile = pstrFileName ;
	ofn4save.lpstrFileTitle = pstrTitleName ;
	ofn4save.Flags = OFN_OVERWRITEPROMPT ;
	return GetSaveFileName (&ofn4save) ;
}