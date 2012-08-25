// MFC_MDI_Test.h : MFC_MDI_Test 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CMFC_MDI_TestApp:
// 有关此类的实现，请参阅 MFC_MDI_Test.cpp
//

class CMFC_MDI_TestApp : public CWinApp
{
public:
	CMFC_MDI_TestApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFC_MDI_TestApp theApp;