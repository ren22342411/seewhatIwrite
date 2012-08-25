// WinSockDlg.h : header file
//

#if !defined(AFX_WINSOCKDLG_H__165316FE_0E27_4654_8F55_96353441D902__INCLUDED_)
#define AFX_WINSOCKDLG_H__165316FE_0E27_4654_8F55_96353441D902__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define RECV_BUFFER_SIZE 1024
#define SEND_BUFFER_SIZE 1024

#define UM_PRGRECV  WM_USER+10
#define UM_PRGSEND  WM_USER+11
#define UM_PRGROVER WM_USER+12
#define UM_PRGSOVER WM_USER+13
#define UM_PRGRRDY  WM_USER+14
#define UM_PRGRBGN  WM_USER+15
#define UM_PRGSBGN  WM_USER+17


/////////////////////////////////////////////////////////////////////////////
// CWinSockDlg dialog

struct sockrecv
{
	HWND hwnd;
};
struct socksend
{
	HWND hwnd;
	DWORD dwip;
	LPCSTR sendfname;
};

class CWinSockDlg : public CDialog
{
	// Construction
public:
	static DWORD WINAPI SocketSend(LPVOID lpParameter);
	static DWORD WINAPI SocketRecv(LPVOID lpParameter);
	CWinSockDlg(CWnd* pParent = NULL);	// standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CWinSockDlg)
	enum { IDD = IDD_WINSOCK_DIALOG };
	CProgressCtrl	m_prgsend;
	CProgressCtrl	m_prgrecv;
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinSockDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	HICON m_hIcon;
	
	// Generated message map functions
	//{{AFX_MSG(CWinSockDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpen();
	afx_msg void OnRecv();
	afx_msg void OnSend();
	//}}AFX_MSG
	afx_msg void onprgrecv(WPARAM wParam,LPARAM lParam);
	afx_msg void onprgrover(WPARAM wParam,LPARAM lParam);
	afx_msg void onprgsend(WPARAM wParam,LPARAM lParam);
	afx_msg void onprgsover(WPARAM wParam,LPARAM lParam);
	afx_msg void onprgrrdy(WPARAM wParam,LPARAM lParam);
	afx_msg void onprgrbgn(WPARAM wParam,LPARAM lParam);
	afx_msg void onprgsbgn(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	int m_prgsendpre;
	BOOL m_beginprgsend;
	int m_prgrecvpre;
	BOOL m_beginprgrecv;
	
	CString m_sendfname;
	
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINSOCKDLG_H__165316FE_0E27_4654_8F55_96353441D902__INCLUDED_)
