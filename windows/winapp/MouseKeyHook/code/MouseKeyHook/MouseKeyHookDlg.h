// MouseKeyHookDlg.h : header file
//

#if !defined(AFX_MOUSEKEYHOOKDLG_H__6229E46F_F2A7_4DD6_908A_D8054CFEC789__INCLUDED_)
#define AFX_MOUSEKEYHOOKDLG_H__6229E46F_F2A7_4DD6_908A_D8054CFEC789__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMouseKeyHookDlg dialog

class CMouseKeyHookDlg : public CDialog
{
// Construction
public:
	BOOL SetHook(INT nType,BOOL fInstall, DWORD dwThreadId);
	CMouseKeyHookDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMouseKeyHookDlg)
	enum { IDD = IDD_MOUSEKEYHOOK_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMouseKeyHookDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMouseKeyHookDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnhookmouse();
	afx_msg void OnBtnunhookmouse();
	virtual void OnCancel();
	afx_msg void OnBtnhookkey();
	afx_msg void OnBtnunhookkey();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOUSEKEYHOOKDLG_H__6229E46F_F2A7_4DD6_908A_D8054CFEC789__INCLUDED_)
