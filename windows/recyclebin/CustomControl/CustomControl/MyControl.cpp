// MyControl.cpp : 实现文件
//

#include "stdafx.h"
#include "CustomControl.h"
#include "MyControl.h"


// CMyControl

IMPLEMENT_DYNAMIC(CMyControl, CWnd)

CMyControl::CMyControl()
: m_strShow(_T("Old Text"))
{

}

CMyControl::~CMyControl()
{
}

void CMyControl::PostNcDestroy()
{
	delete this;
}

LRESULT CMyControl::OnSetControlText(WPARAM wParam,LPARAM lParam)
{
	m_strShow = (char*)lParam;
	this->Invalidate();
	return 0;
}

LRESULT CMyControl::OnGetControlText(WPARAM wParam,LPARAM lParam)
{
	return (LRESULT)(LPCTSTR(m_strShow));
}

LRESULT CALLBACK CMyControl::staticWndProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWnd* pControl = CWnd::FromHandlePermanent(hWnd);
	if (pControl == NULL)
	{
		pControl = new CMyControl();
		pControl->Attach(hWnd);
	}
	return AfxCallWndProc(pControl,hWnd,nMessage,wParam,lParam);
}

void CMyControl::RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.lpszClassName = L"MyCustomControl";
	wc.hInstance = hInstance;
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

BEGIN_MESSAGE_MAP(CMyControl, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_MESSAGE(WM_SETCONTROLTEXT,OnSetControlText)
	ON_MESSAGE(WM_GETCONTROLTEXT,OnGetControlText)
END_MESSAGE_MAP()



// CMyControl 消息处理程序



void CMyControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CWnd::OnLButtonDown(nFlags, point);
	GetParent()->SendMessage(WM_COMMAND,this->GetDlgCtrlID(),(LPARAM)this->GetSafeHwnd());
}

void CMyControl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	int nLen = m_strShow.GetLength();
	if (nLen > 0)
	{
		dc.TextOut(0,0,m_strShow);
	}
}
