// MFC_SDI_TestView.h : CMFC_SDI_TestView 类的接口
//


#pragma once


class CMFC_SDI_TestView : public CView
{
protected: // 仅从序列化创建
	CMFC_SDI_TestView();
	DECLARE_DYNCREATE(CMFC_SDI_TestView)

// 属性
public:
	CMFC_SDI_TestDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFC_SDI_TestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MFC_SDI_TestView.cpp 中的调试版本
inline CMFC_SDI_TestDoc* CMFC_SDI_TestView::GetDocument() const
   { return reinterpret_cast<CMFC_SDI_TestDoc*>(m_pDocument); }
#endif

