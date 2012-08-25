// MyIM_DLL_CALLView.h : CMyIM_DLL_CALLView 类的接口
//


#pragma once


class CMyIM_DLL_CALLView : public CView
{
protected: // 仅从序列化创建
	CMyIM_DLL_CALLView();
	DECLARE_DYNCREATE(CMyIM_DLL_CALLView)

// 属性
public:
	CMyIM_DLL_CALLDoc* GetDocument() const;

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
	virtual ~CMyIM_DLL_CALLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MyIM_DLL_CALLView.cpp 中的调试版本
inline CMyIM_DLL_CALLDoc* CMyIM_DLL_CALLView::GetDocument() const
   { return reinterpret_cast<CMyIM_DLL_CALLDoc*>(m_pDocument); }
#endif

