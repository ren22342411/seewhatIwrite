// ZX_JQR.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CZX_JQRApp:
// �йش����ʵ�֣������ ZX_JQR.cpp
//

class CZX_JQRApp : public CWinApp
{
public:
	CZX_JQRApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CZX_JQRApp theApp;