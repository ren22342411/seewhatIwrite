// MyIM_DLL_CALLDoc.h : CMyIM_DLL_CALLDoc ��Ľӿ�
//


#pragma once


class CMyIM_DLL_CALLDoc : public CDocument
{
protected: // �������л�����
	CMyIM_DLL_CALLDoc();
	DECLARE_DYNCREATE(CMyIM_DLL_CALLDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CMyIM_DLL_CALLDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


