// MFC_MDI_TestDoc.h : CMFC_MDI_TestDoc ��Ľӿ�
//


#pragma once


class CMFC_MDI_TestDoc : public CDocument
{
protected: // �������л�����
	CMFC_MDI_TestDoc();
	DECLARE_DYNCREATE(CMFC_MDI_TestDoc)

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
	virtual ~CMFC_MDI_TestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


