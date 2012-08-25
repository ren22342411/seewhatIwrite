// MFC_MDI_TestView.cpp : CMFC_MDI_TestView 类的实现
//

#include "stdafx.h"
#include "MFC_MDI_Test.h"

#include "MFC_MDI_TestDoc.h"
#include "MFC_MDI_TestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_MDI_TestView

IMPLEMENT_DYNCREATE(CMFC_MDI_TestView, CView)

BEGIN_MESSAGE_MAP(CMFC_MDI_TestView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFC_MDI_TestView 构造/析构

CMFC_MDI_TestView::CMFC_MDI_TestView()
{
	// TODO: 在此处添加构造代码

}

CMFC_MDI_TestView::~CMFC_MDI_TestView()
{
}

BOOL CMFC_MDI_TestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFC_MDI_TestView 绘制

void CMFC_MDI_TestView::OnDraw(CDC* /*pDC*/)
{
	CMFC_MDI_TestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMFC_MDI_TestView 打印

BOOL CMFC_MDI_TestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFC_MDI_TestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFC_MDI_TestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CMFC_MDI_TestView 诊断

#ifdef _DEBUG
void CMFC_MDI_TestView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_MDI_TestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_MDI_TestDoc* CMFC_MDI_TestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_MDI_TestDoc)));
	return (CMFC_MDI_TestDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_MDI_TestView 消息处理程序
