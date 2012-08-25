// MyIM_DLL_CALLDoc.cpp : CMyIM_DLL_CALLDoc 类的实现
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


// CMyIM_DLL_CALLDoc 构造/析构

CMyIM_DLL_CALLDoc::CMyIM_DLL_CALLDoc()
{
	// TODO: 在此添加一次性构造代码

}

CMyIM_DLL_CALLDoc::~CMyIM_DLL_CALLDoc()
{
}

BOOL CMyIM_DLL_CALLDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CMyIM_DLL_CALLDoc 序列化

void CMyIM_DLL_CALLDoc::Serialize(CArchive& ar)
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


// CMyIM_DLL_CALLDoc 诊断

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


// CMyIM_DLL_CALLDoc 命令
