// MFC_DLG_TEST.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFC_DLG_TESTApp:
// �йش����ʵ�֣������ MFC_DLG_TEST.cpp
//

class CMFC_DLG_TESTApp : public CWinApp
{
public:
	CMFC_DLG_TESTApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFC_DLG_TESTApp theApp;