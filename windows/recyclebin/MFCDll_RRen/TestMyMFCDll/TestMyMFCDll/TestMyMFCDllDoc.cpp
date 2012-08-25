// TestMyMFCDllDoc.cpp : CTestMyMFCDllDoc 类的实现
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


// CTestMyMFCDllDoc 构造/析构

CTestMyMFCDllDoc::CTestMyMFCDllDoc()
{
	// TODO: 在此添加一次性构造代码

}

CTestMyMFCDllDoc::~CTestMyMFCDllDoc()
{
}

BOOL CTestMyMFCDllDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CTestMyMFCDllDoc 序列化

void CTestMyMFCDllDoc::Serialize(CArchive& ar)
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


// CTestMyMFCDllDoc 诊断

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


// CTestMyMFCDllDoc 命令
