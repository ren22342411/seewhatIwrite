// TestMyMFCDllView.cpp : CTestMyMFCDllView 类的实现
//

#include "stdafx.h"
#include "TestMyMFCDll.h"

#include "TestMyMFCDllDoc.h"
#include "TestMyMFCDllView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMyMFCDllView

IMPLEMENT_DYNCREATE(CTestMyMFCDllView, CView)

BEGIN_MESSAGE_MAP(CTestMyMFCDllView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTestMyMFCDllView 构造/析构

CTestMyMFCDllView::CTestMyMFCDllView()
{
	// TODO: 在此处添加构造代码

}

CTestMyMFCDllView::~CTestMyMFCDllView()
{
}

BOOL CTestMyMFCDllView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTestMyMFCDllView 绘制

void CTestMyMFCDllView::OnDraw(CDC* /*pDC*/)
{
	CTestMyMFCDllDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CTestMyMFCDllView 打印

BOOL CTestMyMFCDllView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTestMyMFCDllView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTestMyMFCDllView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CTestMyMFCDllView 诊断

#ifdef _DEBUG
void CTestMyMFCDllView::AssertValid() const
{
	CView::AssertValid();
}

void CTestMyMFCDllView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestMyMFCDllDoc* CTestMyMFCDllView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestMyMFCDllDoc)));
	return (CTestMyMFCDllDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestMyMFCDllView 消息处理程序
