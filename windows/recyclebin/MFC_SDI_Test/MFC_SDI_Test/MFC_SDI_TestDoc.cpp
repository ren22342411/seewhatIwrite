// MFC_SDI_TestDoc.cpp : CMFC_SDI_TestDoc 类的实现
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


// CMFC_SDI_TestDoc 构造/析构

CMFC_SDI_TestDoc::CMFC_SDI_TestDoc()
{
	// TODO: 在此添加一次性构造代码

}

CMFC_SDI_TestDoc::~CMFC_SDI_TestDoc()
{
}

BOOL CMFC_SDI_TestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CMFC_SDI_TestDoc 序列化

void CMFC_SDI_TestDoc::Serialize(CArchive& ar)
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


// CMFC_SDI_TestDoc 诊断

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


// CMFC_SDI_TestDoc 命令
