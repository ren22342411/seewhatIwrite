// MFC_MDI_TestView.cpp : CMFC_MDI_TestView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFC_MDI_TestView ����/����

CMFC_MDI_TestView::CMFC_MDI_TestView()
{
	// TODO: �ڴ˴���ӹ������

}

CMFC_MDI_TestView::~CMFC_MDI_TestView()
{
}

BOOL CMFC_MDI_TestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMFC_MDI_TestView ����

void CMFC_MDI_TestView::OnDraw(CDC* /*pDC*/)
{
	CMFC_MDI_TestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CMFC_MDI_TestView ��ӡ

BOOL CMFC_MDI_TestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMFC_MDI_TestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMFC_MDI_TestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CMFC_MDI_TestView ���

#ifdef _DEBUG
void CMFC_MDI_TestView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_MDI_TestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_MDI_TestDoc* CMFC_MDI_TestView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_MDI_TestDoc)));
	return (CMFC_MDI_TestDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_MDI_TestView ��Ϣ�������
