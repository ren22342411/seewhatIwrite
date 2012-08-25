// MFC_MDI_TestView.h : CMFC_MDI_TestView ��Ľӿ�
//


#pragma once


class CMFC_MDI_TestView : public CView
{
protected: // �������л�����
	CMFC_MDI_TestView();
	DECLARE_DYNCREATE(CMFC_MDI_TestView)

// ����
public:
	CMFC_MDI_TestDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMFC_MDI_TestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MFC_MDI_TestView.cpp �еĵ��԰汾
inline CMFC_MDI_TestDoc* CMFC_MDI_TestView::GetDocument() const
   { return reinterpret_cast<CMFC_MDI_TestDoc*>(m_pDocument); }
#endif

