// TestMyMFCDllDoc.h : CTestMyMFCDllDoc ��Ľӿ�
//


#pragma once


class CTestMyMFCDllDoc : public CDocument
{
protected: // �������л�����
	CTestMyMFCDllDoc();
	DECLARE_DYNCREATE(CTestMyMFCDllDoc)

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
	virtual ~CTestMyMFCDllDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


