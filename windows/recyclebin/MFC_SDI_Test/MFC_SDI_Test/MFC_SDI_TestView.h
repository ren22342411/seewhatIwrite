// MFC_SDI_TestView.h : CMFC_SDI_TestView ��Ľӿ�
//


#pragma once


class CMFC_SDI_TestView : public CView
{
protected: // �������л�����
	CMFC_SDI_TestView();
	DECLARE_DYNCREATE(CMFC_SDI_TestView)

// ����
public:
	CMFC_SDI_TestDoc* GetDocument() const;

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
	virtual ~CMFC_SDI_TestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MFC_SDI_TestView.cpp �еĵ��԰汾
inline CMFC_SDI_TestDoc* CMFC_SDI_TestView::GetDocument() const
   { return reinterpret_cast<CMFC_SDI_TestDoc*>(m_pDocument); }
#endif

