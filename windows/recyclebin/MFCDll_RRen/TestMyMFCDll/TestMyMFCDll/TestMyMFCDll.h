// TestMyMFCDll.h : TestMyMFCDll Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CTestMyMFCDllApp:
// �йش����ʵ�֣������ TestMyMFCDll.cpp
//

class CTestMyMFCDllApp : public CWinApp
{
public:
	CTestMyMFCDllApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTestMyMFCDllApp theApp;