// MFC_SDI_TestDoc.cpp : CMFC_SDI_TestDoc ���ʵ��
//

#include "stdafx.h"
#include "MFC_SDI_Test.h"

#include "MFC_SDI_TestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_SDI_TestDoc

IMPLEMENT_DYNCREATE(CMFC_SDI_TestDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFC_SDI_TestDoc, CDocument)
END_MESSAGE_MAP()


// CMFC_SDI_TestDoc ����/����

CMFC_SDI_TestDoc::CMFC_SDI_TestDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CMFC_SDI_TestDoc::~CMFC_SDI_TestDoc()
{
}

BOOL CMFC_SDI_TestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CMFC_SDI_TestDoc ���л�

void CMFC_SDI_TestDoc::Serialize(CArchive& ar)
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


// CMFC_SDI_TestDoc ���

#ifdef _DEBUG
void CMFC_SDI_TestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFC_SDI_TestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFC_SDI_TestDoc ����
