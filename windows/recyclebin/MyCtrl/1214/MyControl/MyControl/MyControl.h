// MyControl.h : main header file for the MyControl DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMyControlApp
// See MyControl.cpp for the implementation of this class
//

class CMyControlApp : public CWinApp
{
public:
	CMyControlApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
