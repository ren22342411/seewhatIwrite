// MFC_DLG_TESTDlg.h : ͷ�ļ�
//

#pragma once


// CMFC_DLG_TESTDlg �Ի���
class CMFC_DLG_TESTDlg : public CDialog
{
// ����
public:
	CMFC_DLG_TESTDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC_DLG_TEST_DIALOG };

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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeComboboxex3();
};
