// MFC_MDI_TestDoc.cpp : CMFC_MDI_TestDoc 类的实现
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


// CMFC_MDI_TestDoc 构造/析构

CMFC_MDI_TestDoc::CMFC_MDI_TestDoc()
{
	// TODO: 在此添加一次性构造代码

}

CMFC_MDI_TestDoc::~CMFC_MDI_TestDoc()
{
}

BOOL CMFC_MDI_TestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CMFC_MDI_TestDoc 序列化

void CMFC_MDI_TestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CMFC_MDI_TestDoc 诊断

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


// CMFC_MDI_TestDoc 命令
