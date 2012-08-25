// MFC_SDI_TestView.cpp : CMFC_SDI_TestView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFC_SDI_TestView ����/����

CMFC_SDI_TestView::CMFC_SDI_TestView()
{
	// TODO: �ڴ˴���ӹ������

}

CMFC_SDI_TestView::~CMFC_SDI_TestView()
{
}

BOOL CMFC_SDI_TestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMFC_SDI_TestView ����

void CMFC_SDI_TestView::OnDraw(CDC* /*pDC*/)
{
	CMFC_SDI_TestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CMFC_SDI_TestView ��ӡ

BOOL CMFC_SDI_TestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMFC_SDI_TestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMFC_SDI_TestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CMFC_SDI_TestView ���

#ifdef _DEBUG
void CMFC_SDI_TestView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_SDI_TestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_SDI_TestDoc* CMFC_SDI_TestView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_SDI_TestDoc)));
	return (CMFC_SDI_TestDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_SDI_TestView ��Ϣ�������
