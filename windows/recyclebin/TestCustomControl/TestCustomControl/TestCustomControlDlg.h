// TestCustomControlDlg.h : ͷ�ļ�
//
#define WM_SETCONTROLTEXT (WM_USER + 0)
#define WM_GETCONTROLTEXT (WM_USER + 1)

#pragma once


// CTestCustomControlDlg �Ի���
class CTestCustomControlDlg : public CDialog
{
// ����
public:
	CTestCustomControlDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTCUSTOMCONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClickCustomControl();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGettext();
public:
	afx_msg void OnBnClickedSettext();
public:
	afx_msg void OnBnClickedReset();
};
