// MFC_MDI_Test.h : MFC_MDI_Test Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMFC_MDI_TestApp:
// �йش����ʵ�֣������ MFC_MDI_Test.cpp
//

class CMFC_MDI_TestApp : public CWinApp
{
public:
	CMFC_MDI_TestApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFC_MDI_TestApp theApp;