#ifndef PSDLG
#define PSDLG
#include "Resource.h"
#include "GraphicsObject.cpp"
static int iRadioPS = 0;
static bool bChoosePenStyle = false;

INT_PTR CALLBACK ChoosePSProc(HWND , UINT , WPARAM , LPARAM );
bool ChoosePenStyleDLG( int & , HINSTANCE , HWND );
#endif