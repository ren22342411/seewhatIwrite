// Scribble.h : Scribble Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CScribbleApp:
// �йش����ʵ�֣������ Scribble.cpp
//

class CScribbleApp : public CWinApp
{
public:
	CScribbleApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CScribbleApp theApp;