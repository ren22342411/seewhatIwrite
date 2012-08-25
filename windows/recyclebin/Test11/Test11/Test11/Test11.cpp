// Test11.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Test11.h"
#include <map>


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

using namespace std;

class CMyWnd;
typedef map<HWND,CMyWnd*> MAPHWND2WND;
MAPHWND2WND  mapHWND2Wnd;

CMyWnd* tmpMyWnd = NULL;

class CMyWnd
{
public:
	static LRESULT CALLBACK WndProc(HWND hWnd,UINT nMessage,WPARAM wParam,LPARAM lParam);
	
	LRESULT WindowProc(UINT nMessage,WPARAM wParam,LPARAM lParam);


	CMyWnd()
	{
		m_hWnd = NULL;
	}
	bool Create()
	{
		tmpMyWnd  = this;
		
		m_hWnd  = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, 100,100, NULL, NULL, hInst, NULL);
		
		if (!m_hWnd)
			return false;

		
		return true;
	}

	BOOL ShowWindow(int nCmdShow)
	{
		return ::ShowWindow(m_hWnd,nCmdShow);
	}

	BOOL UpdataWindow()
	{
		return ::UpdateWindow(m_hWnd);
	}

	virtual void OnPaint()
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hWnd, &ps);
		::TextOut(hdc,0,0,L"123456",6);
		EndPaint(m_hWnd, &ps);

	}

	HWND m_hWnd;
};

class CMyWnd2 : public CMyWnd
{
public:
	virtual void OnPaint()
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hWnd, &ps);
		::TextOut(hdc,0,0,L"6789",4);
		EndPaint(m_hWnd, &ps);
	}

	string m_str;
};

class CMyWnd3 : public CMyWnd
{
public:
	virtual void OnPaint()
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hWnd, &ps);
		::TextOut(hdc,0,0,L"000000",6);
		EndPaint(m_hWnd, &ps);
	}

	string m_str;
};

CMyWnd* findCMyWnd(HWND hWnd)
{
	MAPHWND2WND::iterator it = mapHWND2Wnd.find(hWnd);
	if (it != mapHWND2Wnd.end())
	{
		return it->second;
	}
	else
	{
		mapHWND2Wnd.insert(make_pair(hWnd,tmpMyWnd));
		tmpMyWnd->m_hWnd = hWnd;
		return tmpMyWnd;
		//DebugBreak();
	}
	return NULL;
}

LRESULT CALLBACK CMyWnd::WndProc(HWND hWnd,UINT nMessage,WPARAM wParam,LPARAM lParam)
{
	CMyWnd* pWnd = findCMyWnd(hWnd);
	if (pWnd != NULL)
	{
		return pWnd->WindowProc( nMessage, wParam, lParam);
	}
	else
	{
		DebugBreak();
	}
	return 0;
}

LRESULT CMyWnd::WindowProc(UINT nMessage,WPARAM wParam,LPARAM lParam)
{
	if (nMessage == WM_PAINT)
	{
		OnPaint();
		return 0;
	}
	return ::DefWindowProc(m_hWnd, nMessage, wParam, lParam);;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEST11, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST11));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= CMyWnd::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST11));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TEST11);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   //HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   CMyWnd* pWnd = new CMyWnd3();

   if (!pWnd->Create())
	   return FALSE;

   /*
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   */

   pWnd->ShowWindow(nCmdShow);
   pWnd->UpdataWindow();
   
   //ShowWindow(hWnd, nCmdShow);
   //UpdateWindow(hWnd);

   pWnd = new CMyWnd2();

   if (!pWnd->Create())
	   return FALSE;
   pWnd->ShowWindow(nCmdShow);
   pWnd->UpdataWindow();

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
