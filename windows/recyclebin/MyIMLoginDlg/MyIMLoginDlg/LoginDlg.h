#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CLoginDlg 对话框

class AFX_EXT_CLASS CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();

// 对话框数据
	enum { IDD = IDD_LOGINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeUserId();
public:
	afx_msg void OnEnChangePassword();
public:
	afx_msg void OnBnClickedHide();
public:
	afx_msg void OnBnClickedKeepinfo();
public:
	afx_msg void OnBnClickedRegister();
public:
	afx_msg void OnBnClickedLogin();
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnStnClickedLoginPic();

private:
	BOOL mBHide;
	CButton mCBHide;
	CButton mCBKeepInfo;
	BOOL mBKeepInfo;
private:
	CString mUserIDStr;
	CComboBoxEx mCBXUserID;
private:
	CString mPassword;
	CEdit mCEPassword;
	afx_msg void OnCbnEditchangeUserId();
};
