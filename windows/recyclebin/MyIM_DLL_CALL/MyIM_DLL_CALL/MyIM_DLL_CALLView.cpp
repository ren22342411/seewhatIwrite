// MyIM_DLL_CALLView.cpp : CMyIM_DLL_CALLView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMyIM_DLL_CALLView ����/����

CMyIM_DLL_CALLView::CMyIM_DLL_CALLView()
{
	// TODO: �ڴ˴���ӹ������

}

CMyIM_DLL_CALLView::~CMyIM_DLL_CALLView()
{
}

BOOL CMyIM_DLL_CALLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMyIM_DLL_CALLView ����

void CMyIM_DLL_CALLView::OnDraw(CDC* /*pDC*/)
{
	CMyIM_DLL_CALLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CMyIM_DLL_CALLView ��ӡ

BOOL CMyIM_DLL_CALLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMyIM_DLL_CALLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMyIM_DLL_CALLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CMyIM_DLL_CALLView ���

#ifdef _DEBUG
void CMyIM_DLL_CALLView::AssertValid() const
{
	CView::AssertValid();
}

void CMyIM_DLL_CALLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyIM_DLL_CALLDoc* CMyIM_DLL_CALLView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyIM_DLL_CALLDoc)));
	return (CMyIM_DLL_CALLDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyIM_DLL_CALLView ��Ϣ�������
