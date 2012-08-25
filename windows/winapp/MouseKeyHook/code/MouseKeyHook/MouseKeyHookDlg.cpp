// MouseKeyHookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MouseKeyHook.h"
#include "MouseKeyHookDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "dbg.h"


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
// CMouseKeyHookDlg dialog

CMouseKeyHookDlg::CMouseKeyHookDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMouseKeyHookDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMouseKeyHookDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMouseKeyHookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMouseKeyHookDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMouseKeyHookDlg, CDialog)
	//{{AFX_MSG_MAP(CMouseKeyHookDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNHOOKMOUSE, OnBtnhookmouse)
	ON_BN_CLICKED(IDC_BTNUNHOOKMOUSE, OnBtnunhookmouse)
	ON_BN_CLICKED(IDC_BTNHOOKKEY, OnBtnhookkey)
	ON_BN_CLICKED(IDC_BTNUNHOOKKEY, OnBtnunhookkey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMouseKeyHookDlg message handlers

BOOL CMouseKeyHookDlg::OnInitDialog()
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
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMouseKeyHookDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMouseKeyHookDlg::OnPaint() 
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
HCURSOR CMouseKeyHookDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMouseKeyHookDlg::OnBtnhookmouse() 
{
	SetHook(0,TRUE,0);
}

void CMouseKeyHookDlg::OnBtnunhookmouse() 
{
	SetHook(0,FALSE,0);
}

void CMouseKeyHookDlg::OnCancel() 
{
	SetHook(0,FALSE,0);	
	SetHook(1,FALSE,0);	
	CDialog::OnCancel();
}

void CMouseKeyHookDlg::OnBtnhookkey() 
{
	SetHook(1,TRUE,0);	
}

void CMouseKeyHookDlg::OnBtnunhookkey() 
{
	SetHook(1,FALSE,0);		
}


//nType ,0-mouse,1-key!
//fInstall,TRUE-install hook FALSE-uninstall hook
//dwThreadId,0-hook all process! other-hook specified process!
HHOOK hMouseHook=NULL,hKeyHook=NULL;

BOOL CMouseKeyHookDlg::SetHook(INT nType, BOOL fInstall, DWORD dwThreadId)
{
	BOOL fOk=TRUE;

	if(0 == nType)
	{
		if(fInstall)
		{
			//Install the windows' hook

			HINSTANCE hInst=LoadLibrary((LPCTSTR) "MsgHook.dll");	//MsgHook.dll is in the same path
			
			hMouseHook = SetWindowsHookEx(WH_MOUSE,(HOOKPROC)GetProcAddress(hInst, "CallBackMouseMsgProc"),hInst,dwThreadId);
			
			fOk=(hMouseHook != NULL);
		}
		else
		{
			fOk=UnhookWindowsHookEx(hMouseHook);
			hMouseHook = NULL;
		}	
	}
	else if (1 == nType)
	{
		if(fInstall)
		{
			//Install the windows' hook
			HINSTANCE hInst=LoadLibrary((LPCTSTR) "MsgHook.dll");	//MsgHook.dll is in the same path
			
			hKeyHook = SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)GetProcAddress(hInst, "CallBackKeyMsgProc"),hInst,dwThreadId);
			
			fOk=(hKeyHook != NULL);
		}
		else
		{
			fOk=UnhookWindowsHookEx(hKeyHook);
			hKeyHook = NULL;
		}	

	}
	return (fOk);
}
