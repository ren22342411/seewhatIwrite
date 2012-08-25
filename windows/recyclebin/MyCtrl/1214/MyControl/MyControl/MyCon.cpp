// MyCon.cpp : implementation file
//

#include "stdafx.h"
#include "MyControl.h"
#include "MyCon.h"


// CMyCon

IMPLEMENT_DYNAMIC(CMyCon, CWnd)

CMyCon::CMyCon()
: m_str(_T("Hello world"))
{

}

CMyCon::~CMyCon()
{
}


BEGIN_MESSAGE_MAP(CMyCon, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMyCon message handlers



void CMyCon::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	delete this;
	CWnd::PostNcDestroy();
}

LRESULT CMyCon::staticWndProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	CWnd* pControl = CWnd::FromHandlePermanent(hWnd); 
	if (pControl == NULL) 
	{ 
		pControl = new CMyCon(); 
		pControl->Attach(hWnd); 
	} 
	return AfxCallWndProc(pControl,hWnd,nMessage,wParam,lParam);
}

void CMyCon::RegisterWndClass(HINSTANCE hInst)
{
	WNDCLASS wc; 
	wc.lpszClassName = L"CMyCon"; 
	wc.hInstance = hInst; 
	wc.lpfnWndProc = staticWndProc; 
	wc.hbrBackground = (HBRUSH)::GetStockObject(LTGRAY_BRUSH); 
	wc.hCursor = ::LoadCursor(NULL,IDC_ARROW); 
	wc.hIcon = 0; 
	wc.lpszMenuName = 0; 
	wc.style = CS_GLOBALCLASS; 
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0; 
	::RegisterClass(&wc);
}

void CMyCon::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnLButtonDown(nFlags, point);
	GetParent()->SendMessage(WM_COMMAND,this->GetDlgCtrlID(), 
		(LPARAM)this->GetSafeHwnd());
}

void CMyCon::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	dc.TextOut(0,0,m_str);
}

LRESULT CMyCon::OnSetControlText(WPARAM wParam, LPARAM lParam)
{
	m_str = (wchar_t*)lParam;
	this->Invalidate();
	return 0;
}
LRESULT CMyCon::OnGetControlText(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)LPCTSTR(m_str);
}