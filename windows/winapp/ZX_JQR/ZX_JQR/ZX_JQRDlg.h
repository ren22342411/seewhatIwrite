// ZX_JQRDlg.h : ͷ�ļ�
//

#pragma once


// CZX_JQRDlg �Ի���
class CZX_JQRDlg : public CDialog
{
// ����
public:
	CZX_JQRDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ZX_JQR_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMOutofmemoryHotkey1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnLbnSelchangeList3();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeCombo2();
};
