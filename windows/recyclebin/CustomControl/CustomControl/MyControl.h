#pragma once

#define WM_SETCONTROLTEXT (WM_USER + 0)
#define WM_GETCONTROLTEXT (WM_USER + 1)
// CMyControl

class CMyControl : public CWnd
{
	DECLARE_DYNAMIC(CMyControl)

public:
	CMyControl();
	virtual ~CMyControl();

public:
	static LRESULT CALLBACK staticWndProc(HWND hWnd,UINT nMessage,WPARAM wParam,LPARAM lParam);
	static void RegisterWndClass(HINSTANCE hInstance);

protected:
	afx_msg LRESULT OnSetControlText(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnGetControlText(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	virtual void PostNcDestroy();
public:
	CString m_strShow;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnPaint();
};


