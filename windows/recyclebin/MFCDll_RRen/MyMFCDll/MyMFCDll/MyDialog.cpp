// MyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MyDialog.h"


// CMyDialog dialog

IMPLEMENT_DYNAMIC(CMyDialog, CDialog)

CMyDialog::CMyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDialog::IDD, pParent)
{

}

CMyDialog::~CMyDialog()
{
}

void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CMyDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CMyDialog message handlers

void CMyDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
	::AfxMessageBox(L"OK Clicked");
}
