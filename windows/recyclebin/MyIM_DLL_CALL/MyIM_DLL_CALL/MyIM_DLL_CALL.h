// MyIM_DLL_CALL.h : MyIM_DLL_CALL Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMyIM_DLL_CALLApp:
// �йش����ʵ�֣������ MyIM_DLL_CALL.cpp
//

class CMyIM_DLL_CALLApp : public CWinApp
{
public:
	CMyIM_DLL_CALLApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMyIM_DLL_CALLApp theApp;