// TestMyMFCDllView.cpp : CTestMyMFCDllView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTestMyMFCDllView ����/����

CTestMyMFCDllView::CTestMyMFCDllView()
{
	// TODO: �ڴ˴���ӹ������

}

CTestMyMFCDllView::~CTestMyMFCDllView()
{
}

BOOL CTestMyMFCDllView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CTestMyMFCDllView ����

void CTestMyMFCDllView::OnDraw(CDC* /*pDC*/)
{
	CTestMyMFCDllDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CTestMyMFCDllView ��ӡ

BOOL CTestMyMFCDllView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CTestMyMFCDllView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CTestMyMFCDllView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CTestMyMFCDllView ���

#ifdef _DEBUG
void CTestMyMFCDllView::AssertValid() const
{
	CView::AssertValid();
}

void CTestMyMFCDllView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestMyMFCDllDoc* CTestMyMFCDllView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestMyMFCDllDoc)));
	return (CTestMyMFCDllDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestMyMFCDllView ��Ϣ�������
