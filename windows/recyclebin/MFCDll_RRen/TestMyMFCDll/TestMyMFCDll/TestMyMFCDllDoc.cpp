// TestMyMFCDllDoc.cpp : CTestMyMFCDllDoc ���ʵ��
//

#include "stdafx.h"
#include "TestMyMFCDll.h"

#include "TestMyMFCDllDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMyMFCDllDoc

IMPLEMENT_DYNCREATE(CTestMyMFCDllDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestMyMFCDllDoc, CDocument)
END_MESSAGE_MAP()


// CTestMyMFCDllDoc ����/����

CTestMyMFCDllDoc::CTestMyMFCDllDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CTestMyMFCDllDoc::~CTestMyMFCDllDoc()
{
}

BOOL CTestMyMFCDllDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CTestMyMFCDllDoc ���л�

void CTestMyMFCDllDoc::Serialize(CArchive& ar)
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


// CTestMyMFCDllDoc ���

#ifdef _DEBUG
void CTestMyMFCDllDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestMyMFCDllDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTestMyMFCDllDoc ����
