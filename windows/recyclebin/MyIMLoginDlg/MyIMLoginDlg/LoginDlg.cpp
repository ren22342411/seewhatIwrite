// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "LoginDlg.h"


// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
	, mBHide(FALSE)
	, mBKeepInfo(FALSE)
	, mUserIDStr(_T(""))
	, mPassword(_T(""))
{
//	this->mBHide=this->mCBHide.GetCheck();
//	this->mBKeepInfo=this->mCBKeepInfo.GetCheck();
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_HIDE, mBHide);
	DDX_Control(pDX, IDC_HIDE, mCBHide);
	DDX_Control(pDX, IDC_KEEPINFO, mCBKeepInfo);
	DDX_Check(pDX, IDC_KEEPINFO, mBKeepInfo);
	DDX_CBString(pDX, IDC_USER_ID, mUserIDStr);
	DDV_MaxChars(pDX, mUserIDStr, 20);
	DDX_Control(pDX, IDC_USER_ID, mCBXUserID);
	DDX_Text(pDX, IDC_PASSWORD, mPassword);
	DDV_MaxChars(pDX, mPassword, 20);
	DDX_Control(pDX, IDC_PASSWORD, mCEPassword);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_USER_ID, &CLoginDlg::OnCbnSelchangeUserId)
	ON_EN_CHANGE(IDC_PASSWORD, &CLoginDlg::OnEnChangePassword)
	ON_BN_CLICKED(IDC_HIDE, &CLoginDlg::OnBnClickedHide)
	ON_BN_CLICKED(IDC_KEEPINFO, &CLoginDlg::OnBnClickedKeepinfo)
	ON_BN_CLICKED(IDC_REGISTER, &CLoginDlg::OnBnClickedRegister)
	ON_BN_CLICKED(IDC_LOGIN, &CLoginDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_CANCEL, &CLoginDlg::OnBnClickedCancel)
	ON_STN_CLICKED(IDC_LOGIN_PIC, &CLoginDlg::OnStnClickedLoginPic)
	ON_CBN_EDITCHANGE(IDC_USER_ID, &CLoginDlg::OnCbnEditchangeUserId)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序

void CLoginDlg::OnCbnSelchangeUserId()
{

}

void CLoginDlg::OnEnChangePassword()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	this->mCEPassword.GetWindowText(this->mPassword);
}

void CLoginDlg::OnBnClickedHide()
{
	// TODO: 在此添加控件通知处理程序代码
	if (this->mCBHide.GetCheck())
	{
		this->mBHide = TRUE;
	}
	else
	{
		this->mBHide = FALSE;
	}
}

void CLoginDlg::OnBnClickedKeepinfo()
{
	// TODO: 在此添加控件通知处理程序代码
	if (this->mCBKeepInfo.GetCheck())
	{
		this->mBKeepInfo = TRUE;
	}
	else
	{
		this->mBKeepInfo = FALSE;
	}
}

void CLoginDlg::OnBnClickedRegister()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(L"弹出注册对话框");
	this->EndDialog(0);
}

void CLoginDlg::OnBnClickedLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(this->mUserIDStr);
	AfxMessageBox(this->mPassword);
	if (this->mBHide)
	{
		AfxMessageBox(L"隐身登录");
	} 
	else
	{
		AfxMessageBox(L"直接登录");
	}
	if (this->mBKeepInfo)
	{
		AfxMessageBox(L"保留帐号");
	} 
	else
	{
		AfxMessageBox(L"不保留帐号");
	}
}

void CLoginDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->EndDialog(0);
}

void CLoginDlg::OnStnClickedLoginPic()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CLoginDlg::OnCbnEditchangeUserId()
{
	// TODO: 在此添加控件通知处理程序代码
	this->mCBXUserID.GetWindowText(this->mUserIDStr);
}
