// MyIM_DLL_CALL.h : MyIM_DLL_CALL 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CMyIM_DLL_CALLApp:
// 有关此类的实现，请参阅 MyIM_DLL_CALL.cpp
//

class CMyIM_DLL_CALLApp : public CWinApp
{
public:
	CMyIM_DLL_CALLApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMyIM_DLL_CALLApp theApp;