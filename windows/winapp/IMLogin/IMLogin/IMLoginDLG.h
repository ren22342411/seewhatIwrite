#pragma once


// CIMLoginDLG 对话框

class CIMLoginDLG : public CDialog
{
	DECLARE_DYNAMIC(CIMLoginDLG)

public:
	CIMLoginDLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIMLoginDLG();

// 对话框数据
	enum { IDD = IDD_IMLOGINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit2();
public:
	afx_msg void OnBnClickedCheck2();
public:
	afx_msg void OnBnClickedHide();
public:
	afx_msg void OnBnClickedKeepinfo();
public:
	afx_msg void OnCbnSelchangeUserId();
public:
	afx_msg void OnEnChangePassword();
public:
	afx_msg void OnBnClickedRegister();
public:
	afx_msg void OnBnClickedLogin();
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnStnClickedLoginPic();
public:
	afx_msg void OnStnClickedUserIdText();
public:
	afx_msg void OnStnClickedPasswordText();
};
