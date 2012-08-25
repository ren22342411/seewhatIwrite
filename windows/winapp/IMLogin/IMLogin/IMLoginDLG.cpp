// IMLoginDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "IMLogin.h"
#include "IMLoginDLG.h"


// CIMLoginDLG 对话框

IMPLEMENT_DYNAMIC(CIMLoginDLG, CDialog)

CIMLoginDLG::CIMLoginDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CIMLoginDLG::IDD, pParent)
{

}

CIMLoginDLG::~CIMLoginDLG()
{
}

void CIMLoginDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIMLoginDLG, CDialog)

	ON_BN_CLICKED(IDC_HIDE, &CIMLoginDLG::OnBnClickedHide)
	ON_BN_CLICKED(IDC_KEEPINFO, &CIMLoginDLG::OnBnClickedKeepinfo)
	ON_CBN_SELCHANGE(IDC_USER_ID, &CIMLoginDLG::OnCbnSelchangeUserId)
	ON_EN_CHANGE(IDC_PASSWORD, &CIMLoginDLG::OnEnChangePassword)
	ON_BN_CLICKED(IDC_REGISTER, &CIMLoginDLG::OnBnClickedRegister)
	ON_BN_CLICKED(IDC_LOGIN, &CIMLoginDLG::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_CANCEL, &CIMLoginDLG::OnBnClickedCancel)
	ON_STN_CLICKED(IDC_LOGIN_PIC, &CIMLoginDLG::OnStnClickedLoginPic)
	ON_STN_CLICKED(IDC_USER_ID_TEXT, &CIMLoginDLG::OnStnClickedUserIdText)
	ON_STN_CLICKED(IDC_PASSWORD_TEXT, &CIMLoginDLG::OnStnClickedPasswordText)
END_MESSAGE_MAP()


// CIMLoginDLG 消息处理程序


void CIMLoginDLG::OnBnClickedHide()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CIMLoginDLG::OnBnClickedKeepinfo()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CIMLoginDLG::OnCbnSelchangeUserId()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CIMLoginDLG::OnEnChangePassword()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CIMLoginDLG::OnBnClickedRegister()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CIMLoginDLG::OnBnClickedLogin()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CIMLoginDLG::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CIMLoginDLG::OnStnClickedLoginPic()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CIMLoginDLG::OnStnClickedUserIdText()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CIMLoginDLG::OnStnClickedPasswordText()
{
	// TODO: 在此添加控件通知处理程序代码
}
