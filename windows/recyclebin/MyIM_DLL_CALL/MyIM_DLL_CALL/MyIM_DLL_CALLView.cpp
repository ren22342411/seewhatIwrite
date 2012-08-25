// MyIM_DLL_CALLView.cpp : CMyIM_DLL_CALLView 类的实现
//

#include "stdafx.h"
#include "MyIM_DLL_CALL.h"

#include "MyIM_DLL_CALLDoc.h"
#include "MyIM_DLL_CALLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyIM_DLL_CALLView

IMPLEMENT_DYNCREATE(CMyIM_DLL_CALLView, CView)

BEGIN_MESSAGE_MAP(CMyIM_DLL_CALLView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMyIM_DLL_CALLView 构造/析构

CMyIM_DLL_CALLView::CMyIM_DLL_CALLView()
{
	// TODO: 在此处添加构造代码

}

CMyIM_DLL_CALLView::~CMyIM_DLL_CALLView()
{
}

BOOL CMyIM_DLL_CALLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMyIM_DLL_CALLView 绘制

void CMyIM_DLL_CALLView::OnDraw(CDC* /*pDC*/)
{
	CMyIM_DLL_CALLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMyIM_DLL_CALLView 打印

BOOL CMyIM_DLL_CALLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMyIM_DLL_CALLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMyIM_DLL_CALLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CMyIM_DLL_CALLView 诊断

#ifdef _DEBUG
void CMyIM_DLL_CALLView::AssertValid() const
{
	CView::AssertValid();
}

void CMyIM_DLL_CALLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyIM_DLL_CALLDoc* CMyIM_DLL_CALLView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyIM_DLL_CALLDoc)));
	return (CMyIM_DLL_CALLDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyIM_DLL_CALLView 消息处理程序
