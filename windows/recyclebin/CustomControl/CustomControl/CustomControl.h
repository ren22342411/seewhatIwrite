// CustomControl.h : CustomControl DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCustomControlApp
// �йش���ʵ�ֵ���Ϣ������� CustomControl.cpp
//

class CCustomControlApp : public CWinApp
{
public:
	CCustomControlApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
