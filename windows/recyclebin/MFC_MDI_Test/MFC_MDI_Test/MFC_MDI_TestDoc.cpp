// MFC_MDI_TestDoc.cpp : CMFC_MDI_TestDoc ���ʵ��
//

#include "stdafx.h"
#include "MFC_MDI_Test.h"

#include "MFC_MDI_TestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_MDI_TestDoc

IMPLEMENT_DYNCREATE(CMFC_MDI_TestDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFC_MDI_TestDoc, CDocument)
END_MESSAGE_MAP()


// CMFC_MDI_TestDoc ����/����

CMFC_MDI_TestDoc::CMFC_MDI_TestDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CMFC_MDI_TestDoc::~CMFC_MDI_TestDoc()
{
}

BOOL CMFC_MDI_TestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CMFC_MDI_TestDoc ���л�

void CMFC_MDI_TestDoc::Serialize(CArchive& ar)
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


// CMFC_MDI_TestDoc ���

#ifdef _DEBUG
void CMFC_MDI_TestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFC_MDI_TestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFC_MDI_TestDoc ����
