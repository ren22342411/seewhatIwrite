// TestMyMFCDllView.h : CTestMyMFCDllView 类的接口
//


#pragma once


class CTestMyMFCDllView : public CView
{
protected: // 仅从序列化创建
	CTestMyMFCDllView();
	DECLARE_DYNCREATE(CTestMyMFCDllView)

// 属性
public:
	CTestMyMFCDllDoc* GetDocument() const;

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
	virtual ~CTestMyMFCDllView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // TestMyMFCDllView.cpp 中的调试版本
inline CTestMyMFCDllDoc* CTestMyMFCDllView::GetDocument() const
   { return reinterpret_cast<CTestMyMFCDllDoc*>(m_pDocument); }
#endif

