// IMLogin.h : IMLogin DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CIMLoginApp
// �йش���ʵ�ֵ���Ϣ������� IMLogin.cpp
//

class CIMLoginApp : public CWinApp
{
public:
	CIMLoginApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
