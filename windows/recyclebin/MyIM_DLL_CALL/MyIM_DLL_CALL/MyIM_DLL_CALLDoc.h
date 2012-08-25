// MyIM_DLL_CALLDoc.h : CMyIM_DLL_CALLDoc 类的接口
//


#pragma once


class CMyIM_DLL_CALLDoc : public CDocument
{
protected: // 仅从序列化创建
	CMyIM_DLL_CALLDoc();
	DECLARE_DYNCREATE(CMyIM_DLL_CALLDoc)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CMyIM_DLL_CALLDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};


