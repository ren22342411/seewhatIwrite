// MyIM_DLL_CALLView.h : CMyIM_DLL_CALLView ��Ľӿ�
//


#pragma once


class CMyIM_DLL_CALLView : public CView
{
protected: // �������л�����
	CMyIM_DLL_CALLView();
	DECLARE_DYNCREATE(CMyIM_DLL_CALLView)

// ����
public:
	CMyIM_DLL_CALLDoc* GetDocument() const;

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
	virtual ~CMyIM_DLL_CALLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MyIM_DLL_CALLView.cpp �еĵ��԰汾
inline CMyIM_DLL_CALLDoc* CMyIM_DLL_CALLView::GetDocument() const
   { return reinterpret_cast<CMyIM_DLL_CALLDoc*>(m_pDocument); }
#endif

