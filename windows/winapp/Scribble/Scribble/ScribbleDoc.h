// ScribbleDoc.h : CScribbleDoc ��Ľӿ�
//


#pragma once


class CScribbleDoc : public CDocument
{
protected: // �������л�����
	CScribbleDoc();
	DECLARE_DYNCREATE(CScribbleDoc)

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
	virtual ~CScribbleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


