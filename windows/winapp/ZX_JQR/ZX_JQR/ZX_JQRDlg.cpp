// ZX_JQRDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ZX_JQR.h"
#include "ZX_JQRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CZX_JQRDlg 对话框




CZX_JQRDlg::CZX_JQRDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZX_JQRDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZX_JQRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CZX_JQRDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CZX_JQRDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_HOTKEY1, &CZX_JQRDlg::OnNMOutofmemoryHotkey1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CZX_JQRDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_RADIO2, &CZX_JQRDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON2, &CZX_JQRDlg::OnBnClickedButton2)
	ON_LBN_SELCHANGE(IDC_LIST3, &CZX_JQRDlg::OnLbnSelchangeList3)
	ON_BN_CLICKED(IDC_RADIO1, &CZX_JQRDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO4, &CZX_JQRDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO3, &CZX_JQRDlg::OnBnClickedRadio3)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CZX_JQRDlg::OnLvnItemchangedList2)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CZX_JQRDlg::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CZX_JQRDlg 消息处理程序

BOOL CZX_JQRDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CZX_JQRDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CZX_JQRDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CZX_JQRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CZX_JQRDlg::OnBnClickedButton1()
{
	::MessageBox(NULL,L"",L"",MB_OK);
}

void CZX_JQRDlg::OnNMOutofmemoryHotkey1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CZX_JQRDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}

void CZX_JQRDlg::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
}

void CZX_JQRDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
		LPCWSTR nameZx=L"Element Client";
		CWnd *hZx=FindWindow(NULL,nameZx);
		if(hZx==NULL)
		{
			::MessageBox(NULL,L"请先运行游戏！",L"ERROR",MB_OK);
		}
}

void CZX_JQRDlg::OnLbnSelchangeList3()
{
	// TODO: Add your control notification handler code here
}

void CZX_JQRDlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
}

void CZX_JQRDlg::OnBnClickedRadio4()
{
	// TODO: Add your control notification handler code here
}

void CZX_JQRDlg::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
}

void CZX_JQRDlg::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CZX_JQRDlg::OnCbnSelchangeCombo2()
{
	// TODO: Add your control notification handler code here
}
