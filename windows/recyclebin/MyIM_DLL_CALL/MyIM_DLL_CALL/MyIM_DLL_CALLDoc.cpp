// MyIM_DLL_CALLDoc.cpp : CMyIM_DLL_CALLDoc ���ʵ��
//

#include "stdafx.h"
#include "MyIM_DLL_CALL.h"

#include "MyIM_DLL_CALLDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyIM_DLL_CALLDoc

IMPLEMENT_DYNCREATE(CMyIM_DLL_CALLDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyIM_DLL_CALLDoc, CDocument)
END_MESSAGE_MAP()


// CMyIM_DLL_CALLDoc ����/����

CMyIM_DLL_CALLDoc::CMyIM_DLL_CALLDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CMyIM_DLL_CALLDoc::~CMyIM_DLL_CALLDoc()
{
}

BOOL CMyIM_DLL_CALLDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CMyIM_DLL_CALLDoc ���л�

void CMyIM_DLL_CALLDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CMyIM_DLL_CALLDoc ���

#ifdef _DEBUG
void CMyIM_DLL_CALLDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyIM_DLL_CALLDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMyIM_DLL_CALLDoc ����
