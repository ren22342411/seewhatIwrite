#pragma once

#define WM_SETCONTROLTEXT (WM_USER + 0)
#define WM_GETCONTROLTEXT (WM_USER + 1)

// CMyCon

class CMyCon : public CWnd
{
	DECLARE_DYNAMIC(CMyCon)

public:
	CMyCon();
	virtual ~CMyCon();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	static LRESULT CALLBACK staticWndProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam);
public:
	static void RegisterWndClass(HINSTANCE hInst);
public:
	CString m_str;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnPaint();
	afx_msg LRESULT OnSetControlText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetControlText(WPARAM wParam, LPARAM lParam);
};


