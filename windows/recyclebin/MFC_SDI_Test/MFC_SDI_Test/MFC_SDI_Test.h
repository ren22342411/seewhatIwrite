// MFC_SDI_Test.h : MFC_SDI_Test Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMFC_SDI_TestApp:
// �йش����ʵ�֣������ MFC_SDI_Test.cpp
//

class CMFC_SDI_TestApp : public CWinApp
{
public:
	CMFC_SDI_TestApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFC_SDI_TestApp theApp;