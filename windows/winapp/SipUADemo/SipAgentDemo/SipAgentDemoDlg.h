// SipAgentDemoDlg.h : header file
//

#if !defined(AFX_SIPAGENTDEMODLG_H__8F6A73CA_3588_42C8_BE05_F1A1D9D5D03B__INCLUDED_)
#define AFX_SIPAGENTDEMODLG_H__8F6A73CA_3588_42C8_BE05_F1A1D9D5D03B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipDll.h"
#include "BmpStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CSipAgentDemoDlg dialog

class CSipAgentDemoDlg : public CDialog
{
// Construction
public:
	CSipAgentDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSipAgentDemoDlg)
	enum { IDD = IDD_SIPAGENTDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSipAgentDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSipAgentDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonRegister();
	afx_msg void OnButtonMakecall();
	afx_msg void OnButtonHungup();
	afx_msg void OnButtonAnswer();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	enum SIP_REGISTER_STATUS {Status_Initialization, Status_Registered, Status_Unregistered, Status_Unknown};
	
	int m_iSipLineNumber; //the line number
	CString m_csNumber; //the number you want call

	int m_iAudioInDevice;
	int m_iAudioOutDevice;
	
	void InitCodec();	//initialize the supported codec
	BOOL InitAudioDevices();

	static	void CALLBACK SipEventCallBack(int iLineNumber, int iEventType,  WPARAM wParam, 
		LPARAM lParam, void* pAppInstance);

	// a thread proxy used to register
	static UINT RegisterSIPProc(void* );

	//process the incoming sip message
	void SipMessageComing(int iLineNumber, int iEventType,  WPARAM wParam, LPARAM lParam);

	//display current sip status
	void SetSIPPhoneStatus(SIP_REGISTER_STATUS srs, CString& csStatus = CString(""));
public:

	CBmpStatic m_staticMailTo;
	CBmpStatic m_staticWebsite;

	void SetContact();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIPAGENTDEMODLG_H__8F6A73CA_3588_42C8_BE05_F1A1D9D5D03B__INCLUDED_)
