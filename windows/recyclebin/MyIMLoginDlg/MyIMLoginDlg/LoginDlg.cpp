// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "LoginDlg.h"


// CLoginDlg �Ի���

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


// CLoginDlg ��Ϣ�������

void CLoginDlg::OnCbnSelchangeUserId()
{

}

void CLoginDlg::OnEnChangePassword()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	this->mCEPassword.GetWindowText(this->mPassword);
}

void CLoginDlg::OnBnClickedHide()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AfxMessageBox(L"����ע��Ի���");
	this->EndDialog(0);
}

void CLoginDlg::OnBnClickedLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AfxMessageBox(this->mUserIDStr);
	AfxMessageBox(this->mPassword);
	if (this->mBHide)
	{
		AfxMessageBox(L"�����¼");
	} 
	else
	{
		AfxMessageBox(L"ֱ�ӵ�¼");
	}
	if (this->mBKeepInfo)
	{
		AfxMessageBox(L"�����ʺ�");
	} 
	else
	{
		AfxMessageBox(L"�������ʺ�");
	}
}

void CLoginDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->EndDialog(0);
}

void CLoginDlg::OnStnClickedLoginPic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CLoginDlg::OnCbnEditchangeUserId()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->mCBXUserID.GetWindowText(this->mUserIDStr);
}
