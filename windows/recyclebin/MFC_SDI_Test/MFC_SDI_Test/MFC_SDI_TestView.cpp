// MFC_SDI_TestView.cpp : CMFC_SDI_TestView 类的实现
//

#include "stdafx.h"
#include "MFC_SDI_Test.h"

#include "MFC_SDI_TestDoc.h"
#include "MFC_SDI_TestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_SDI_TestView

IMPLEMENT_DYNCREATE(CMFC_SDI_TestView, CView)

BEGIN_MESSAGE_MAP(CMFC_SDI_TestView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFC_SDI_TestView 构造/析构

CMFC_SDI_TestView::CMFC_SDI_TestView()
{
	// TODO: 在此处添加构造代码

}

CMFC_SDI_TestView::~CMFC_SDI_TestView()
{
}

BOOL CMFC_SDI_TestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFC_SDI_TestView 绘制

void CMFC_SDI_TestView::OnDraw(CDC* /*pDC*/)
{
	CMFC_SDI_TestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMFC_SDI_TestView 打印

BOOL CMFC_SDI_TestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFC_SDI_TestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFC_SDI_TestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CMFC_SDI_TestView 诊断

#ifdef _DEBUG
void CMFC_SDI_TestView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_SDI_TestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_SDI_TestDoc* CMFC_SDI_TestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_SDI_TestDoc)));
	return (CMFC_SDI_TestDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_SDI_TestView 消息处理程序
