// SipAgentDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SipAgentDemo.h"
#include "SipAgentDemoDlg.h"
#include "SipDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



 UserConfig g_UserConfig; //user config, 
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSipAgentDemoDlg dialog

CSipAgentDemoDlg::CSipAgentDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSipAgentDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSipAgentDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSipAgentDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSipAgentDemoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSipAgentDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CSipAgentDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, OnButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_MAKECALL, OnButtonMakecall)
	ON_BN_CLICKED(IDC_BUTTON_HUNGUP, OnButtonHungup)
	ON_BN_CLICKED(IDC_BUTTON_ANSWER, OnButtonAnswer)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSipAgentDemoDlg message handlers

BOOL CSipAgentDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	//Initialize the codec
	InitCodec();

	SetSIPPhoneStatus(Status_Unregistered);

#ifdef _DEBUG
	CEdit* pEdit;
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PROXY);
	pEdit->SetWindowText("192.168.88.102:5060");

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_USERNAME);
	pEdit->SetWindowText("99200");

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PASSWORD);
	pEdit->SetWindowText("99200");

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CALLNUMBER);
	pEdit->SetWindowText("1204258204");
#endif

	SetContact();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSipAgentDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSipAgentDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSipAgentDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CALLBACK CSipAgentDemoDlg::SipEventCallBack(int iLineNumber, int iEventType, WPARAM wParam, LPARAM lParam, void* pThis)
{
	if(pThis != NULL)
	{
		((CSipAgentDemoDlg*)pThis)->SipMessageComing(iLineNumber, iEventType, wParam, lParam);
	}
}
void CSipAgentDemoDlg::SetSIPPhoneStatus(SIP_REGISTER_STATUS srs, CString& csOtherStatus)
{
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_STATUS);
	if(pStatic != NULL)
	{
		CString csStatus;
		if(srs == Status_Registered)
		{
			csStatus = "Registered" ;
		}
		else if(srs == Status_Unregistered)
		{
			csStatus = "Register failed";
		}
		else if(srs == Status_Initialization)
		{
			csStatus = "Registering";

		}
		else
		{
			csStatus = csOtherStatus;
		}
		pStatic->SetWindowText(csStatus);
	}
}
void CSipAgentDemoDlg::SipMessageComing(int iLineNumber, int iEventType,  WPARAM wParam, LPARAM lParam)
{
	char szEventType[48] = {0};
	CString csText;
	FormateXosipEventMessage(iEventType, szEventType, 48);
	switch(iEventType)
	{
	case EXOSIP_REGISTRATION_SUCCESS:
		{
			SetSIPPhoneStatus(Status_Registered);
		}
		break;
	case EXOSIP_REGISTRATION_FAILURE:
		{
			csText = "Register Failed";
			char* pReason = (char*)wParam;
			if(pReason != NULL)
			{
				if(strlen(pReason) > 0)
				{
					csText += "(";  
					csText += CString(pReason);
					csText += ")";
				}
			}
			SetSIPPhoneStatus(Status_Unregistered);
		}
		break;
	case EXOSIP_CALL_INVITE:
		{
			csText = "Incoming: ";
			char* szNumber = (char*)wParam;
			char* szName = (char*)lParam;
			CString csTemp;
			if(szName != NULL)
			{
				if(strlen(szName) > 0)
				{
					csTemp = szName;
				}
			}
			else if(szNumber != NULL)
			{
				if(strlen(szNumber) > 0)
				{
					csTemp = szNumber;
				}
			}
			else
			{
				csTemp = "No Number";
			}
			csTemp.Replace("\"", "");

			if(csTemp.GetLength() != 0)
			{
				CString csStatus = "Incoming call ";
				csStatus += csTemp;
				SetSIPPhoneStatus(Status_Unknown, csStatus);
			}

			m_iSipLineNumber = iLineNumber;

			SetForegroundWindow();			
		}
		break;
	case EXOSIP_CALL_ANSWERED:
		{

			csText = "Talking...";
			SetSIPPhoneStatus(Status_Unknown, csText);
		}
		break;
	case  EXOSIP_CALL_REQUESTFAILURE:
		{
			if(wParam != 0)
			{
				csText = (char*)wParam;
			}
			SetSIPPhoneStatus(Status_Unknown, csText);
		}
		break;
	case EXOSIP_CALL_NOANSWER:
		{
			csText = "No Answer";
			SetSIPPhoneStatus(Status_Unknown, csText);
		}
		break;
	case EXOSIP_CALL_CANCELLED:
		{
			m_iSipLineNumber = -1;
			csText = "Cancelled";
			SetSIPPhoneStatus(Status_Unknown, csText);
		}
		break;
	case EXOSIP_CALL_CLOSED:
		{
			m_iSipLineNumber = -1;
			csText = "Closed";
			SetSIPPhoneStatus(Status_Unknown, csText);
		}
		break;
	case EXOSIP_CALL_REINVITE:
		{

		}
		break;
	case EXOSIP_CALL_RINGING:
		{
			if(wParam == 180)
			{
				csText = "Ringing...";
			}
			else if(wParam == 183)
			{
				csText = "Proceeding...";
			}
		    SetSIPPhoneStatus(Status_Unknown, csText);
		}
		break;
	case EXOSIP_CALL_PROCEEDING:
		{
			csText = "Proceeding...";
			SetSIPPhoneStatus(Status_Unknown, csText);
		}
		break;
	case EXOSIP_CALL_GLOBALFAILURE:
		{
			if(wParam != 0)
			{
				csText = (char*)wParam;
			}
			if(csText.CompareNoCase("Decline") == 0)
			{
				csText = "Call Attempt Failed";
			}
			SipHangup(m_iSipLineNumber);
			m_iSipLineNumber = -1;	
			SetSIPPhoneStatus(Status_Unknown, csText);
		}
		break;
	case EXOSIP_CALL_SERVERFAILURE:
		{
			csText = "Server Failure";

			SipHangup(m_iSipLineNumber);
			m_iSipLineNumber = -1;	
			SetSIPPhoneStatus(Status_Unknown, csText);

		}
		break;
	default:
		break;
	}
}

void CSipAgentDemoDlg::OnButtonRegister() 
{
	// TODO: Add your control notification handler code here

	CEdit* pEdit;
	CString  csUserName, csPassword, csProxy;
	
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PROXY);
	pEdit->GetWindowText(csProxy);

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_USERNAME);
	pEdit->GetWindowText(csUserName);

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PASSWORD);
	pEdit->GetWindowText(csPassword);

	if(csProxy.GetLength() == 0)
	{
		AfxMessageBox("Please Input Proxy");
	}

	if(csUserName.GetLength() == 0)
	{
		AfxMessageBox("Please input user name");
	}

	strcpy(g_UserConfig.SipConfig.szPhoneNumber, (LPCTSTR)csUserName);
	strcpy(g_UserConfig.SipConfig.szUserName, (LPCTSTR)csUserName);
	strcpy(g_UserConfig.SipConfig.szPassword, (LPCTSTR)csPassword);
	
	if(csProxy.Find(':') < 0)
	{
		strcpy(g_UserConfig.SipConfig.szDomain, (LPCTSTR)csProxy);
		csProxy += ":5060";
		strcpy(g_UserConfig.SipConfig.szProxy, (LPCTSTR)csProxy);
	}
	else
	{
		strcpy(g_UserConfig.SipConfig.szProxy, (LPCTSTR)csProxy);
		int iFound = csProxy.Find(':');
		csProxy = csProxy.Left(iFound);
		strcpy(g_UserConfig.SipConfig.szDomain, (LPCTSTR)csProxy);
	}

	g_UserConfig.SipConfig.bAutoRegister = true;
	g_UserConfig.SipConfig.bUseStun = false;

	//config your local sip port and rtp port
	g_UserConfig.SipConfig.iSipPort = 10060;
	g_UserConfig.SipConfig.iRTPProt = 10066;


	AfxBeginThread(RegisterSIPProc, this);
}

void CSipAgentDemoDlg::InitCodec()
{
	//currently, we only support A Law, ULaw and g729

	g_UserConfig.SupportCodec.iSupportCodecNumbers = 3;
	
	strcpy(g_UserConfig.SupportCodec.CodecInfos[0].szName, "G729") ;
	g_UserConfig.SupportCodec.CodecInfos[0].iSampleRate = 8000;
	g_UserConfig.SupportCodec.CodecInfos[0].iPayLoadType = 18;		

	strcpy(g_UserConfig.SupportCodec.CodecInfos[0].szName, "PCMU") ;
	g_UserConfig.SupportCodec.CodecInfos[0].iSampleRate = 8000;
	g_UserConfig.SupportCodec.CodecInfos[0].iPayLoadType = 0;		

	strcpy(g_UserConfig.SupportCodec.CodecInfos[0].szName, "PCMA") ;
	g_UserConfig.SupportCodec.CodecInfos[0].iSampleRate = 8000;
	g_UserConfig.SupportCodec.CodecInfos[0].iPayLoadType = 8;		

	
}

UINT CSipAgentDemoDlg::RegisterSIPProc(void* pThis)
{
	SipRegister(&g_UserConfig, SipEventCallBack, pThis);
	return 1;
}

void CSipAgentDemoDlg::OnButtonMakecall() 
{
	// TODO: Add your control notification handler code here
	CEdit* pEdit;
	
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CALLNUMBER);
	pEdit->GetWindowText(m_csNumber);

	if(FALSE == InitAudioDevices())
	{
		AfxMessageBox("We can not find any audio devices, please check it.");
		return ;
	}
	m_iSipLineNumber = SipDialCall((char*)(LPCTSTR)m_csNumber, m_iAudioInDevice, m_iAudioOutDevice);

	if(m_iSipLineNumber < 0)
	{
		SetSIPPhoneStatus(Status_Unknown, CString("Call failed."));
	}
	else
	{
		CString csMsg = "Calling " + m_csNumber;
		SetSIPPhoneStatus(Status_Unknown, csMsg);
	}
}

void CSipAgentDemoDlg::OnButtonHungup() 
{
	// TODO: Add your control notification handler code here
	SipHangup(m_iSipLineNumber);
	SetSIPPhoneStatus(Status_Unknown, CString("Hangup."));
}

void CSipAgentDemoDlg::OnButtonAnswer() 
{
	// TODO: Add your control notification handler code here
	if(FALSE == InitAudioDevices())
	{
		AfxMessageBox("We can not find any audio devices, please check it.");
	}
	SipAnswer(m_iSipLineNumber, m_iAudioInDevice, m_iAudioOutDevice);
}

void CSipAgentDemoDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	SipExit();
	// TODO: Add your message handler code here
	
}
/*
这个地方演示如何使用AudioXXXX系列函数
*/
BOOL CSipAgentDemoDlg::InitAudioDevices()
{
	/*
	取得当前系统一共有的Playbacke设备和Capture设备的数目

	调用SipDial和SipAnswer时候需要传入的ID的值就是[0, iAudioInDeviceCount) 
	区间和[0, iAudioOutDeviceCount]区间的值 	

    调用 AudioGetCaptureDeviceNameByID(i)函数(其中i即为上面两个区间的值),可以得到相对应设备的
	名称
	*/
	int iAudioOutDevCount = AudioGetPlaybackDeviceNb();
	int iAudioInDevCount = AudioGetCaptureDeviceNb();
	
	if(iAudioInDevCount == 0 || iAudioOutDevCount == 0)
	{
		return FALSE;
	}
	else
	{
		m_iAudioOutDevice = 0;
		m_iAudioInDevice = 0;
	}

	return TRUE;
}

void CSipAgentDemoDlg::SetContact()
{
	CRect r1(53, 392, 218, 408);
	m_staticMailTo.Create("", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, r1, this, 30000);			
	m_staticMailTo.GetParent()->ClientToScreen(r1);
	m_staticMailTo.GetParent()->ScreenToClient(r1);
	m_staticMailTo.MoveWindow(r1);
	m_staticMailTo.m_strIDName = "STATIC_MAILTO";
	m_staticMailTo.SetLinkMode(TRUE);
	m_staticMailTo.SetGrayMode(FALSE);
	m_staticMailTo.SetTextFont(15, "Arial", FW_NORMAL, FALSE, TRUE, FALSE);
	m_staticMailTo.SetLinkText("mailto:rectsoft@qq.com");
	CString csURL = "mailto://rectsoft@qq.com";
	m_staticMailTo.SetLinkUrl(csURL);
	m_staticMailTo.SetLinkColor(RGB(0,6,20));
	m_staticMailTo.SetVisitedColor(RGB(0,6,20));
    m_staticMailTo.SetHoverColor(RGB(0,0,220));
	m_staticMailTo.SetDTFormat(DT_LEFT|DT_VCENTER);

	CRect r2(233, 392, 388, 408);
	m_staticWebsite.Create("", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, r2, this, 30000);			
	m_staticWebsite.GetParent()->ClientToScreen(r2);
	m_staticWebsite.GetParent()->ScreenToClient(r2);
	m_staticWebsite.MoveWindow(r2);
	m_staticWebsite.m_strIDName = "STATIC_WEBSITE";
	m_staticWebsite.SetLinkMode(TRUE);
	m_staticWebsite.SetGrayMode(FALSE);
	m_staticWebsite.SetTextFont(15, "Arial", FW_NORMAL, FALSE, TRUE, FALSE);
	m_staticWebsite.SetLinkText("website:www.rectsoft.com");
	csURL = "www.rectsoft.com";
	m_staticWebsite.SetLinkUrl(csURL);
	m_staticWebsite.SetLinkColor(RGB(0,6,20));
	m_staticWebsite.SetVisitedColor(RGB(0,6,20));
    m_staticWebsite.SetHoverColor(RGB(0,0,220));
	m_staticWebsite.SetDTFormat(DT_LEFT|DT_VCENTER);
}