// IMLoginDLG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IMLogin.h"
#include "IMLoginDLG.h"


// CIMLoginDLG �Ի���

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


// CIMLoginDLG ��Ϣ�������


void CIMLoginDLG::OnBnClickedHide()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CIMLoginDLG::OnBnClickedKeepinfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CIMLoginDLG::OnCbnSelchangeUserId()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CIMLoginDLG::OnEnChangePassword()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CIMLoginDLG::OnBnClickedRegister()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CIMLoginDLG::OnBnClickedLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CIMLoginDLG::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CIMLoginDLG::OnStnClickedLoginPic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CIMLoginDLG::OnStnClickedUserIdText()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CIMLoginDLG::OnStnClickedPasswordText()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
