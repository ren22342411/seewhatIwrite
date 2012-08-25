// TestCustomControlDlg.h : 头文件
//
#define WM_SETCONTROLTEXT (WM_USER + 0)
#define WM_GETCONTROLTEXT (WM_USER + 1)

#pragma once


// CTestCustomControlDlg 对话框
class CTestCustomControlDlg : public CDialog
{
// 构造
public:
	CTestCustomControlDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTCUSTOMCONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClickCustomControl();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGettext();
public:
	afx_msg void OnBnClickedSettext();
public:
	afx_msg void OnBnClickedReset();
};
