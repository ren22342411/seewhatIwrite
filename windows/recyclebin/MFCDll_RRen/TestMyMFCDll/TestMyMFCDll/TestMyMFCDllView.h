// TestMyMFCDllView.h : CTestMyMFCDllView ��Ľӿ�
//


#pragma once


class CTestMyMFCDllView : public CView
{
protected: // �������л�����
	CTestMyMFCDllView();
	DECLARE_DYNCREATE(CTestMyMFCDllView)

// ����
public:
	CTestMyMFCDllDoc* GetDocument() const;

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
	virtual ~CTestMyMFCDllView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // TestMyMFCDllView.cpp �еĵ��԰汾
inline CTestMyMFCDllDoc* CTestMyMFCDllView::GetDocument() const
   { return reinterpret_cast<CTestMyMFCDllDoc*>(m_pDocument); }
#endif

