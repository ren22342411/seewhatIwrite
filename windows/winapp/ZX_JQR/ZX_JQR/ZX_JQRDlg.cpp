// ZX_JQRDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZX_JQR.h"
#include "ZX_JQRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CZX_JQRDlg �Ի���




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


// CZX_JQRDlg ��Ϣ�������

BOOL CZX_JQRDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CZX_JQRDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
			::MessageBox(NULL,L"����������Ϸ��",L"ERROR",MB_OK);
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
