// TestCustomControl.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestCustomControlApp:
// �йش����ʵ�֣������ TestCustomControl.cpp
//

class CTestCustomControlApp : public CWinApp
{
public:
	CTestCustomControlApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestCustomControlApp theApp;