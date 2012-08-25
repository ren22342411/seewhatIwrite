// BasicGraphics.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BasicGraphics.h"
//#include "GraphicsObject.cpp"
#include "ChooseFontDLG.h"
#include "ChooseColorDLG.h"
#include "ChoosePenStyleDLG.h"
#include "OpenFileDLG.h"
#include "BGState.h"
#include "math.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
BGState BGS;
CurrentGraphicsObjects CGO;
std::list<GraphicsObject *>::iterator it;
wchar_t my_text[128];
POINT MovingPt;
//GraphicsObject Static Variables Initialize:
COLORREF GraphicsObject::DefaltColor = 0x0;
int GraphicsObject::SelectingSensitivity = 10;
int GraphicsObject::DefaltPenStyle = PS_SOLID;
int GraphicsObject::DefaltLineWidth = 1;
bool GraphicsObject::DefaultSelectedFlag = false;
LOGFONT MyText::DefaultLogFont;
wchar_t OnlyFileName[1024];
wchar_t PathFileName[1024];
HBITMAP hBMP;
RECT ClientRect4Zoom;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	InputText(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	SelectSS(HWND, UINT, WPARAM, LPARAM);
void GetClientPT(HWND,POINT &);
void OnLBUTTONDBLCLK(HWND);
void OnPAINT(HWND, HDC);
void OnDESTROY();
void OnCREATE(HWND);
void OnLBUTTONDOWN(HWND,LPARAM);
void OnRBUTTONDOWN(HWND,LPARAM);
void OnMOUSEMOVE(HWND);
void OnCOPYDATA(HWND ,WPARAM ,LPARAM );
void CancelDrawing(HWND);
void EnableMenuSomeItems(HWND);
void SendCopyDataMessage(HWND hWnd,WPARAM wParam,LPARAM lParam);

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
	LoadString(hInstance, IDC_BASICGRAPHICS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BASICGRAPHICS));

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

	wcex.style			= CS_DBLCLKS;//CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BASICGRAPHICS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BASICGRAPHICS);
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW/*|WS_HSCROLL|WS_VSCROLL*/,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
	case WM_COPYDATA:
		{
			if(BGS.getES() == ZOOM) 
			{
				InvalidateRect(hWnd,NULL,true);
				BGS.setES(SELECT);
			}
			OnCOPYDATA(hWnd,wParam,lParam);
		}
		break;
	case WM_COMMAND:
		CancelDrawing(hWnd);
		if(BGS.getES() == ZOOM) InvalidateRect(hWnd,NULL,true);
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_NEW:
			{
				for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					delete *it;
				}
				CGO.LpGO.clear();
				InvalidateRect(hWnd,NULL,true);
				EnableMenuSomeItems(hWnd);
			}
			break;

		case ID_FILE_SAVE:
			{
				if(SaveFileDLG(hWnd,PathFileName,OnlyFileName))
				{
					HANDLE hFile=CreateFile(PathFileName,GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
					if(hFile == INVALID_HANDLE_VALUE) MessageBox(NULL,L"Failed to save file!",OnlyFileName,MB_OK);
					else
					{
						CGO.SaveFile(hFile);
						CloseHandle(hFile);
					}
				}
				//::MessageBox(NULL,PathFileName,OnlyFileName,MB_OK);
			}
			break;

		case ID_FILE_LOAD:
			{
				if(LoadFileDLG(hWnd,PathFileName,OnlyFileName))
				{
					HANDLE hFile=CreateFile(PathFileName,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
					if(hFile == INVALID_HANDLE_VALUE) MessageBox(NULL,L"Failed to load file!",OnlyFileName,MB_OK);
					else
					{
						if(CGO.LoadFile(hFile))
						{
							InvalidateRect(hWnd,NULL,true);
							EnableMenuSomeItems(hWnd);
						}
						else MessageBox(NULL,L"Failed to load file!",OnlyFileName,MB_OK);
						CloseHandle(hFile);
					}
				}
				//::MessageBox(NULL,PathFileName,OnlyFileName,MB_OK);
			}
			break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		case ID_EDIT_DRAWLINE:
			{
				BGS.setES(DRAW_LINE);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWLINE, MF_CHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWCIRCLE, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWRECTANGULAR, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWPOLYGON, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWTEXT, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_SELECT, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_ZOOMOUT_ZOOMIN, MF_UNCHECKED);
			}
			break;
		
		case ID_EDIT_DRAWCIRCLE:
			{
				BGS.setES(DRAW_CIRCLE);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWLINE, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWCIRCLE, MF_CHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWRECTANGULAR, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWPOLYGON, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWTEXT, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_SELECT, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_ZOOMOUT_ZOOMIN, MF_UNCHECKED);
			}
			break;
		
		case ID_EDIT_DRAWRECTANGULAR:
			{
				BGS.setES(DRAW_RECTANGULAR);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWLINE, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWCIRCLE, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWRECTANGULAR, MF_CHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWPOLYGON, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWTEXT, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_SELECT, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_ZOOMOUT_ZOOMIN, MF_UNCHECKED);
			}
			break;
		
		case ID_EDIT_DRAWPOLYGON:
			{
				BGS.setES(DRAW_POLYGON);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWLINE, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWCIRCLE, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWRECTANGULAR, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWPOLYGON, MF_CHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWTEXT, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_SELECT, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_ZOOMOUT_ZOOMIN, MF_UNCHECKED);
			}
			break;
		
		case ID_EDIT_DRAWTEXT:
			{
				BGS.setES(DRAW_TEXT);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWLINE, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWCIRCLE, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWRECTANGULAR, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWPOLYGON, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWTEXT, MF_CHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_SELECT, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_ZOOMOUT_ZOOMIN, MF_UNCHECKED);
			}
			break;

		case ID_EDIT_CHANGECOLOR:
			{
				std::list<GraphicsObject *>::iterator it_tmp = CGO.LpGO.end() ;
				for(it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					if((*it)->isSelected())
					{
						it_tmp = it;
					}
				}
				if(it_tmp != CGO.LpGO.end())
				{
						COLORREF temClr = (*it_tmp)->getColor();
						if(ChooseColorDLG(hWnd,temClr))
						{
							(*it_tmp)->setColor(temClr);
							(*it_tmp)->FreshMyArea(hWnd,true);
						}
				}

			}
			break;

		case ID_EDIT_CHANGELINESTYLE:
			{
				std::list<GraphicsObject *>::iterator it_tmp = CGO.LpGO.end() ;
				for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					if((*it)->isSelected() && (*it)->getGraphicsType() != TEXT)
					{
						it_tmp = it;
					}
				}
				if(it_tmp != CGO.LpGO.end())
				{
					int tmpPS = (*it_tmp)->getPenStyle();
					if(ChoosePenStyleDLG(tmpPS,hInst,hWnd)){
						(*it_tmp)->setPenStyle(tmpPS);
						(*it_tmp)->FreshMyArea(hWnd,true);
					}
				}
			}
			break;

		case ID_EDIT_CHANGETEXTFONT:			
			{
				std::list<GraphicsObject *>::iterator it_tmp = CGO.LpGO.end() ;
				for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					if((*it)->isSelected() && (*it)->getGraphicsType() == TEXT)
					{
						it_tmp = it;
					}
				}
				if(it_tmp != CGO.LpGO.end())
				{
					LOGFONT temLf = ((MyText *)(*it_tmp))->getLogFont();
					if(ChooseFontDLG(hWnd,temLf)){
						((MyText *)(*it_tmp))->setLogFont(temLf);
						((MyText *)(*it_tmp))->FreshMyArea(hWnd,true);
					}
				}
			}
			break;

		case ID_EDIT_ZOOMOUT_ZOOMIN:
			{
				{
					Sleep(50);
					::GetClientRect(hWnd,&ClientRect4Zoom);

					HDC hdc = ::GetDC(hWnd);
					HDC hdcMem = CreateCompatibleDC (hdc);
					
					if(hBMP != INVALID_HANDLE_VALUE) DeleteObject (hBMP) ;
					hBMP = CreateCompatibleBitmap(hdc, ClientRect4Zoom.right - ClientRect4Zoom.left, ClientRect4Zoom.bottom - ClientRect4Zoom.top );
					SelectObject(hdcMem, hBMP);
					BitBlt(hdcMem, 0, 0, ClientRect4Zoom.right - ClientRect4Zoom.left, ClientRect4Zoom.bottom - ClientRect4Zoom.top, hdc, 0, 0, SRCCOPY);

					ReleaseDC(hWnd, hdc) ;
					DeleteDC (hdcMem) ;

				}
				BGS.setES(ZOOM);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWLINE, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWCIRCLE, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWRECTANGULAR, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWPOLYGON, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWTEXT, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_SELECT, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_ZOOMOUT_ZOOMIN, MF_CHECKED);
			}
			break;

		case ID_EDIT_SELECT:
			{
				BGS.setES(SELECT);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWLINE, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWCIRCLE, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWRECTANGULAR, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWPOLYGON, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWTEXT, MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_SELECT, MF_CHECKED);
				CheckMenuItem(GetMenu(hWnd),ID_EDIT_ZOOMOUT_ZOOMIN, MF_UNCHECKED);
			}
			break;

		case ID_EDIT_DELETE:
			{
				for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					if((*it)->isSelected())
					{
						RECT tempRect;
						if((*it)->getGraphicsType() == TEXT)
							(*it)->getFreshRect(tempRect,hWnd);
						else
							(*it)->getFreshRect(tempRect);
						delete *it;
						it = CGO.LpGO.erase(it);
						::InvalidateRect(hWnd,&tempRect,true);
						break;
					}
				}
				EnableMenuSomeItems(hWnd);
			}
		break;

		case ID_EDIT_DELETEALL:
			{
				for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					delete *it;
				}
				CGO.LpGO.clear();
				InvalidateRect(hWnd,NULL,true);
				EnableMenuSomeItems(hWnd);
			}
			break;

		case ID_CONFIG_SETDEFAULTCOLOR:
			{
				COLORREF tmpClr = GraphicsObject::getDefaultColor();
				if(ChooseColorDLG(hWnd,tmpClr)){
					GraphicsObject::setDefaultColor(tmpClr);
				}
			}
			break;

		case ID_CONFIG_SETDEFAULTLINESTYLE:
			{
				int tmpPS = GraphicsObject::getDefaultPenStyle();
				if(ChoosePenStyleDLG(tmpPS,hInst,hWnd)){
					GraphicsObject::setDefaultPenStyle(tmpPS);
				}
			}
			break;
		
		case ID_CONFIG_SETDEFAULTTEXTFONT:
			{
				LOGFONT tmpLf = MyText::getDefaultLogFont();
				if(ChooseFontDLG(hWnd,tmpLf)){
					MyText::setDefaultLogFont(tmpLf);
				}
			}
			break;
		
		case ID_CONFIG_SETSELECTINGSENSITIVITY:
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_PROPPAGE_SMALL1), hWnd, SelectSS);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		{
			OnPAINT(hWnd,hdc);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		{
			OnDESTROY();
		}
		break;
	case WM_CREATE:
		{
			OnCREATE(hWnd);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			OnLBUTTONDOWN(hWnd,lParam);
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			OnLBUTTONDBLCLK(hWnd);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			OnRBUTTONDOWN(hWnd,lParam);
		}
		break;
	case WM_MOUSEMOVE:
		{
			OnMOUSEMOVE(hWnd);
		}
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

INT_PTR CALLBACK InputText(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON2://Cancel
			my_text[0]=L'\0';
			BGS.ESReset();
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;
		case IDC_BUTTON1://OK
			GetDlgItemText(hDlg,IDC_EDIT1,my_text,128);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK SelectSS(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//UNREFERENCED_PARAMETER(lParam);
	static int iRadioSS = 0;
	switch (message)
	{
		
	case WM_INITDIALOG:
		{	
			if( 5 == GraphicsObject::getSelectingSensitivity())
				iRadioSS = IDC_RADIO1;
			else if( 10 == GraphicsObject::getSelectingSensitivity())
				iRadioSS = IDC_RADIO2;
			else if( 15 == GraphicsObject::getSelectingSensitivity())
				iRadioSS = IDC_RADIO3;
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, iRadioSS) ;
			SetFocus(GetDlgItem (hDlg, iRadioSS)) ;
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON2://Cancel
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;
		case IDC_BUTTON1://OK
			//temp=::GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO3);   
			if(iRadioSS==IDC_RADIO1) GraphicsObject::setSelectingSensitivity(5);
			else if(iRadioSS==IDC_RADIO2) GraphicsObject::setSelectingSensitivity(10);
			else if(iRadioSS==IDC_RADIO3) GraphicsObject::setSelectingSensitivity(15);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;

		case   IDC_RADIO1:
		case   IDC_RADIO2:
		case   IDC_RADIO3:
			iRadioSS =  LOWORD (wParam);
			CheckRadioButton (hDlg, IDC_RADIO1, IDC_RADIO3, iRadioSS) ;
			return (INT_PTR)TRUE;
			break;

		}
		break;
	}
	return (INT_PTR)FALSE;
}


void GetClientPT(HWND hWnd,POINT &mvPt)
{
	GetCursorPos(&mvPt);
	ScreenToClient(hWnd,&mvPt);
}

void OnLBUTTONDBLCLK(HWND hWnd)
{
	if(DRAW_POLYGON == BGS.getES() && BGS.isLBDMoreThan3())
		{
			std::vector<POINT> vP;
			BGS.CopyPointV(vP);
			if((int)vP.size()>=3){
				::MyPolygon *pmp = new MyPolygon(&vP[0],(int)vP.size());
				CGO.LpGO.push_back(pmp);
					
				for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					if((*it)->isSelected())
					{
						(*it)->setSelectedFlag(false);
						(*it)->FreshMyArea(hWnd,true);
						break;
					}
				}

				pmp->setSelectedFlag(true);
				pmp->FreshMyArea(hWnd,true);
				BGS.ESReset();
				EnableMenuSomeItems(hWnd);
				MyPolyStruct mps;
				pmp->ExportStruct(mps);
				COPYDATASTRUCT cds;
				cds.dwData = (DWORD)POLYGON;
				cds.cbData = (DWORD)sizeof(MyPolyStruct);
				cds.lpData = (PVOID)&mps;
				//HWND to_hWnd =FindWindowEx(NULL,hWnd,szWindowClass,NULL);
				//SendMessage(to_hWnd,WM_COPYDATA,(WPARAM)to_hWnd,(LPARAM)&cds);
				SendCopyDataMessage(hWnd,(WPARAM)hWnd,(LPARAM)&cds);
			}
		}
}

void OnPAINT(HWND hWnd,HDC hdc)
{
	if(BGS.getES() != ZOOM){
		for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
		{
			(*it)->display(hdc);
		}

		switch(BGS.getES())
		{
		case DRAW_LINE:
			{
				if(BGS.isLBD1())
				{
					POINT s;
					::GetClientPT(hWnd,MovingPt);
					BGS.getLastPoint(s);
					MyLine mlTmp(s,MovingPt);
					mlTmp.display(hdc);
				}
			}
			break;

		case DRAW_CIRCLE:
			{
				if(BGS.isLBD1())
				{
					POINT c;
					::GetClientPT(hWnd,MovingPt);
					BGS.getLastPoint(c);
					int tmpR = (int)sqrt((double)((c.x - MovingPt.x)*(c.x - MovingPt.x)+(c.y - MovingPt.y)*(c.y - MovingPt.y)));
					MyCircle mcTmp(c,tmpR);
					mcTmp.display(hdc);
				}
			}
			break;
				
		case DRAW_RECTANGULAR:
			{
				if(BGS.isLBD1())
				{
					POINT s;
					::GetClientPT(hWnd,MovingPt);
					BGS.getLastPoint(s);
					MyRectangular mrTmp(s.x,s.y,MovingPt.x,MovingPt.y);
					mrTmp.display(hdc);
				}
			}
			break;
		
		case DRAW_POLYGON:
			{
				if(BGS.TimesOfLBD() > 0)
				{
					if(BGS.TimesOfLBD() > 1)
					{
						for(int i = 0; i < BGS.TimesOfLBD() - 1; i++)
						{
							POINT s,e;
							BGS.getPointByIndex(s,i);
							BGS.getPointByIndex(e,i+1);
							MyLine tmpMl(s,e);
							tmpMl.display(hdc);
						}
					}
					POINT ms;
					::GetClientPT(hWnd,MovingPt);
					BGS.getLastPoint(ms);
					MyLine tmpmMl(ms,MovingPt);
					tmpmMl.display(hdc);
				}
			}
			break;

		default:
			break;
		}
	}
	else
	{
		switch(BGS.getZoomValue())
		{
		case ZOOM_25:
			{
				HDC hdcMem = CreateCompatibleDC (hdc);
				SelectObject(hdcMem, hBMP);
				StretchBlt (hdc, 0, 0, (ClientRect4Zoom.right - ClientRect4Zoom.left)/4, (ClientRect4Zoom.bottom - ClientRect4Zoom.top)/4,
					hdcMem, 0, 0, ClientRect4Zoom.right - ClientRect4Zoom.left, ClientRect4Zoom.bottom - ClientRect4Zoom.top, SRCCOPY);
				DeleteDC (hdcMem) ;
			}
			break;
		
		case ZOOM_50:
			{
				HDC hdcMem = CreateCompatibleDC (hdc);
				SelectObject(hdcMem, hBMP);
				StretchBlt (hdc, 0, 0, (ClientRect4Zoom.right - ClientRect4Zoom.left)/2, (ClientRect4Zoom.bottom - ClientRect4Zoom.top)/2,
					hdcMem, 0, 0, ClientRect4Zoom.right - ClientRect4Zoom.left, ClientRect4Zoom.bottom - ClientRect4Zoom.top, SRCCOPY);
				DeleteDC (hdcMem) ;
			}
			break;
					
		case ZOOM_75:
			{
				HDC hdcMem = CreateCompatibleDC (hdc);
				SelectObject(hdcMem, hBMP);
				StretchBlt (hdc, 0, 0, 3*(ClientRect4Zoom.right - ClientRect4Zoom.left)/4, 3*(ClientRect4Zoom.bottom - ClientRect4Zoom.top)/4,
					hdcMem, 0, 0, ClientRect4Zoom.right - ClientRect4Zoom.left, ClientRect4Zoom.bottom - ClientRect4Zoom.top, SRCCOPY);
				DeleteDC (hdcMem) ;
			}
			break;
								
		case ZOOM_100:
			{
				HDC hdcMem = CreateCompatibleDC (hdc);
				SelectObject(hdcMem, hBMP);
				StretchBlt (hdc, 0, 0, (ClientRect4Zoom.right - ClientRect4Zoom.left), (ClientRect4Zoom.bottom - ClientRect4Zoom.top),
					hdcMem, 0, 0, ClientRect4Zoom.right - ClientRect4Zoom.left, ClientRect4Zoom.bottom - ClientRect4Zoom.top, SRCCOPY);
				DeleteDC (hdcMem) ;
			}
			break;
					
		case ZOOM_150:
			{
				HDC hdcMem = CreateCompatibleDC (hdc);
				SelectObject(hdcMem, hBMP);
				StretchBlt (hdc, 0, 0, 3*(ClientRect4Zoom.right - ClientRect4Zoom.left)/2, 3*(ClientRect4Zoom.bottom - ClientRect4Zoom.top)/2,
					hdcMem, 0, 0, ClientRect4Zoom.right - ClientRect4Zoom.left, ClientRect4Zoom.bottom - ClientRect4Zoom.top, SRCCOPY);
				DeleteDC (hdcMem) ;
			}
			break;
					
		case ZOOM_200:
			{
				HDC hdcMem = CreateCompatibleDC (hdc);
				SelectObject(hdcMem, hBMP);
				StretchBlt (hdc, 0, 0, 2*(ClientRect4Zoom.right - ClientRect4Zoom.left), 2*(ClientRect4Zoom.bottom - ClientRect4Zoom.top),
					hdcMem, 0, 0, ClientRect4Zoom.right - ClientRect4Zoom.left, ClientRect4Zoom.bottom - ClientRect4Zoom.top, SRCCOPY);
				DeleteDC (hdcMem) ;
			}
			break;

		case ZOOM_400:
			{
				HDC hdcMem = CreateCompatibleDC (hdc);
				SelectObject(hdcMem, hBMP);
				StretchBlt (hdc, 0, 0, 4*(ClientRect4Zoom.right - ClientRect4Zoom.left),4*(ClientRect4Zoom.bottom - ClientRect4Zoom.top),
					hdcMem, 0, 0, ClientRect4Zoom.right - ClientRect4Zoom.left, ClientRect4Zoom.bottom - ClientRect4Zoom.top, SRCCOPY);
				DeleteDC (hdcMem) ;
			}
			break;
		}
	}
}

void OnDESTROY()
{
	//
	if(hBMP != INVALID_HANDLE_VALUE) DeleteObject (hBMP) ;
	//
	unsigned long lWritten;
	HANDLE hFile=CreateFile(L"bg.cfg",GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		::MessageBox(NULL,L"Error!",szTitle,MB_OK);
	}
	else
	{
		COLORREF dftclr = GraphicsObject::getDefaultColor();
		LOGFONT dftlf = MyText::getDefaultLogFont();
		int dftps = GraphicsObject::getDefaultPenStyle();
		int ss = GraphicsObject::getSelectingSensitivity();
		WriteFile(hFile,&dftclr,sizeof(COLORREF),&lWritten,NULL);
		WriteFile(hFile,&dftlf,sizeof(LOGFONT),&lWritten,NULL);
		WriteFile(hFile,&dftps,sizeof(int),&lWritten,NULL);
		WriteFile(hFile,&ss,sizeof(int),&lWritten,NULL);
		CloseHandle(hFile);
	}
	
	PostQuitMessage(0);		
}

void OnCREATE(HWND hWnd)
{
	CheckMenuItem(GetMenu(hWnd),ID_EDIT_DRAWLINE, MF_CHECKED);

	unsigned long lWritten;
	unsigned long lRead;
	HANDLE hFile=CreateFile(L"bg.cfg",GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		HANDLE hNewFile =CreateFile(L"bg.cfg",GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,0);
		if(hNewFile == INVALID_HANDLE_VALUE)
		{
			::MessageBox(NULL,L"Error!",szTitle,MB_OK);
		}
		else
		{
			COLORREF dftclr = GraphicsObject::getDefaultColor();
			LOGFONT dftlf = MyText::getDefaultLogFont();
			int dftps = GraphicsObject::getDefaultPenStyle();
			int ss = GraphicsObject::getSelectingSensitivity();
			WriteFile(hNewFile,&dftclr,sizeof(COLORREF),&lWritten,NULL);
			WriteFile(hNewFile,&dftlf,sizeof(LOGFONT),&lWritten,NULL);
			WriteFile(hNewFile,&dftps,sizeof(int),&lWritten,NULL);
			WriteFile(hNewFile,&ss,sizeof(int),&lWritten,NULL);
			CloseHandle(hNewFile);
		}
	}
	else
	{
		COLORREF dftclr;
		LOGFONT dftlf;
		int dftps;
		int ss;
		ReadFile(hFile,&dftclr,sizeof(COLORREF),&lRead,NULL);
		ReadFile(hFile,&dftlf,sizeof(LOGFONT),&lRead,NULL);
		ReadFile(hFile,&dftps,sizeof(int),&lRead,NULL);
		ReadFile(hFile,&ss,sizeof(int),&lRead,NULL);
		GraphicsObject::setDefaultColor(dftclr);
		MyText::setDefaultLogFont(dftlf);
		GraphicsObject::setDefaultPenStyle(dftps);
		GraphicsObject::setSelectingSensitivity(ss);
		CloseHandle(hFile);
	}
	EnableMenuSomeItems(hWnd);
}
void OnLBUTTONDOWN(HWND hWnd,LPARAM lParam)
{
	switch(BGS.getES())
	{
	case DRAW_LINE:
		{
			if(BGS.isLBD0()) BGS.LBD(LOWORD(lParam),HIWORD(lParam));
			else if(BGS.isLBD1())
			{
				BGS.LBD(LOWORD(lParam),HIWORD(lParam));
				POINT start,end;
				BGS.getPointByIndex(start,0);
				BGS.getPointByIndex(end,1);
				MyLine *pml = new MyLine(start,end);
				CGO.LpGO.push_back(pml);

				for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					if((*it)->isSelected())
					{
						(*it)->setSelectedFlag(false);
						(*it)->FreshMyArea(hWnd,true);
						break;
					}
				}

				pml->setSelectedFlag(true);
				pml->FreshMyArea(hWnd,true);
				BGS.ESReset();
				EnableMenuSomeItems(hWnd);

				MyLineStruct mls;
				pml->ExportStruct(mls);
				COPYDATASTRUCT cds;
				cds.dwData = (DWORD)LINE;
				cds.cbData = (DWORD)sizeof(MyLineStruct);
				cds.lpData = (PVOID)&mls;

				//HWND to_hWnd =FindWindowEx(NULL,hWnd,szWindowClass,NULL);
				//SendMessage(to_hWnd,WM_COPYDATA,(WPARAM)to_hWnd,(LPARAM)&cds);
				SendCopyDataMessage(hWnd,(WPARAM)hWnd,(LPARAM)&cds);
			}
		}
		break;

	case DRAW_CIRCLE:
		{
			if(BGS.isLBD0()) BGS.LBD(LOWORD(lParam),HIWORD(lParam));
			else if(BGS.isLBD1())
			{
				BGS.LBD(LOWORD(lParam),HIWORD(lParam));
				POINT c,r;
				BGS.getPointByIndex(c,0);
				BGS.getPointByIndex(r,1);
				int lr = (int)sqrt((double)((c.x - r.x)*(c.x - r.x)+(c.y - r.y)*(c.y - r.y)));
				MyCircle *pmc = new MyCircle(c,lr);
				CGO.LpGO.push_back(pmc);

				for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					if((*it)->isSelected())
					{
						(*it)->setSelectedFlag(false);
						(*it)->FreshMyArea(hWnd,true);
						break;
					}
				}

				pmc->setSelectedFlag(true);
				pmc->FreshMyArea(hWnd,true);
				BGS.ESReset();
				EnableMenuSomeItems(hWnd);

				MyCircleStruct mcs;
				pmc->ExportStruct(mcs);
				COPYDATASTRUCT cds;
				cds.dwData = (DWORD)CIRCLE;
				cds.cbData = (DWORD)sizeof(MyCircleStruct);
				cds.lpData = (PVOID)&mcs;
				//HWND to_hWnd =FindWindowEx(NULL,hWnd,szWindowClass,NULL);
				//SendMessage(to_hWnd,WM_COPYDATA,(WPARAM)to_hWnd,(LPARAM)&cds);
				SendCopyDataMessage(hWnd,(WPARAM)hWnd,(LPARAM)&cds);
			}
		}
		break;

	case DRAW_RECTANGULAR:				
		{
			if(BGS.isLBD0()) BGS.LBD(LOWORD(lParam),HIWORD(lParam));
			else if(BGS.isLBD1())
			{
				BGS.LBD(LOWORD(lParam),HIWORD(lParam));
				POINT lt,rb;
				BGS.getPointByIndex(lt,0);
				BGS.getPointByIndex(rb,1);
				::MyRectangular *pmr = new MyRectangular(lt.x,lt.y,rb.x,rb.y);
				CGO.LpGO.push_back(pmr);

				for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					if((*it)->isSelected())
					{
						(*it)->setSelectedFlag(false);
						(*it)->FreshMyArea(hWnd,true);
						break;
					}
				}

				pmr->setSelectedFlag(true);
				pmr->FreshMyArea(hWnd,true);
				BGS.ESReset();
				EnableMenuSomeItems(hWnd);

				MyRectStruct mrs;
				pmr->ExportStruct(mrs);
				COPYDATASTRUCT cds;
				cds.dwData = (DWORD)RECTANGULAR;
				cds.cbData = (DWORD)sizeof(MyRectStruct);
				cds.lpData = (PVOID)&mrs;
				//HWND to_hWnd =FindWindowEx(NULL,hWnd,szWindowClass,NULL);
				//SendMessage(to_hWnd,WM_COPYDATA,(WPARAM)to_hWnd,(LPARAM)&cds);
				SendCopyDataMessage(hWnd,(WPARAM)hWnd,(LPARAM)&cds);
			}
		}
		break;
				
	case DRAW_POLYGON:
		{
			BGS.LBD(LOWORD(lParam),HIWORD(lParam));
		}
		break;
				
	case DRAW_TEXT:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_PROPPAGE_SMALL), hWnd, InputText);
			if(wcslen(my_text)>0){
				HDC hdc = ::GetDC(hWnd);
				::MyText *pmt = new MyText(my_text,LOWORD(lParam),HIWORD(lParam),(int)wcslen(my_text),hdc);
				::ReleaseDC(hWnd,hdc);
				CGO.LpGO.push_back(pmt);

				for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					if((*it)->isSelected())
					{
						(*it)->setSelectedFlag(false);
						(*it)->FreshMyArea(hWnd,true);
						break;
					}
				}

				pmt->setSelectedFlag(true);
				pmt->FreshMyArea(hWnd,true);
				BGS.ESReset();
				EnableMenuSomeItems(hWnd);

				MyTextStruct mts;
				pmt->ExportStruct(mts);
				COPYDATASTRUCT cds;
				cds.dwData = (DWORD)TEXT;
				cds.cbData = (DWORD)sizeof(MyTextStruct);
				cds.lpData = (PVOID)&mts;
				//HWND to_hWnd =FindWindowEx(NULL,hWnd,szWindowClass,NULL);
				//SendMessage(to_hWnd,WM_COPYDATA,(WPARAM)to_hWnd,(LPARAM)&cds);
				SendCopyDataMessage(hWnd,(WPARAM)hWnd,(LPARAM)&cds);
			}
		}
		break;

	case SELECT:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
			{
				if((*it)->isSelected())
				{
					(*it)->setSelectedFlag(false);
					(*it)->FreshMyArea(hWnd,true);
					break;
				}
			}

			for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
			{
				if((*it)->didSelectMe(pt))
				{			
					(*it)->setSelectedFlag(true);
					(*it)->FreshMyArea(hWnd,true);
					break;
				}
			}
			
			EnableMenuSomeItems(hWnd);
		}
		break;

	case ZOOM:
		{
			if(BGS.ZoomIn()) ::InvalidateRect(hWnd,NULL,true);
		}
		break;
	}
}

void OnRBUTTONDOWN(HWND hWnd,LPARAM lParam)
{
	switch(BGS.getES())
	{
	case DRAW_LINE:
	case DRAW_CIRCLE:
	case DRAW_RECTANGULAR:
	case DRAW_POLYGON:
	case DRAW_TEXT:
		{
			if( BGS.TimesOfLBD() > 0 )
			{
				RECT fRect;
				switch(BGS.getES())
				{
				case DRAW_LINE:
					{
						POINT s;
						BGS.getLastPoint(s);
						MyLine mlTmp(s,MovingPt);
						mlTmp.getFreshRect(fRect);
					}
					break;

				case DRAW_CIRCLE:
					{
						POINT c;
						BGS.getLastPoint(c);
						int tmpR = (int)sqrt((double)((c.x - MovingPt.x)*(c.x - MovingPt.x)+(c.y - MovingPt.y)*(c.y - MovingPt.y)));
						MyCircle mcTmp(c,tmpR);
						mcTmp.getFreshRect(fRect);
					}
					break;

				case DRAW_RECTANGULAR:
					{
						POINT s;
						BGS.getLastPoint(s);
						::MyRectangular mrTmp(s.x,s.y,MovingPt.x,MovingPt.y);
						mrTmp.getFreshRect(fRect);
					}
					break;

				case DRAW_POLYGON:
					{
						std::vector<POINT> vP;
						BGS.CopyPointV(vP);
						vP.push_back(MovingPt);
						::MyPolygon mpTmp(&vP[0],(int)vP.size());
						mpTmp.getFreshRect(fRect);
					}
					break;

				default:
					break;
				}
				BGS.ESReset();
				::InvalidateRect(hWnd,&fRect,true);
			}
	
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
			{
				if((*it)->isSelected())
				{
					(*it)->setSelectedFlag(false);
					(*it)->FreshMyArea(hWnd,true);
					break;
				}
			}

			for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
			{
				if((*it)->didSelectMe(pt))
				{			
					(*it)->setSelectedFlag(true);
					(*it)->FreshMyArea(hWnd,true);
					break;
				}
			}
			
			EnableMenuSomeItems(hWnd);

		}
		break;

	case SELECT:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
			{
				if((*it)->isSelected())
				{
					(*it)->setSelectedFlag(false);
					(*it)->FreshMyArea(hWnd,true);
					break;
				}
			}

			for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
			{
				if((*it)->didSelectMe(pt))
				{			
					(*it)->setSelectedFlag(true);
					(*it)->FreshMyArea(hWnd,true);
					break;
				}
			}

			EnableMenuSomeItems(hWnd);

		}
		break;

	case ZOOM:
		{
			if(BGS.ZoomOut()) ::InvalidateRect(hWnd,NULL,true);
		}
		break;
	}
	
}

void OnMOUSEMOVE(HWND hWnd)
{
	switch(BGS.getES())
	{
	case DRAW_LINE:
		{
			if(BGS.isLBD1())
			{
				POINT s;
				BGS.getLastPoint(s);
				MyLine mlTmp(s,MovingPt);
				RECT tmpRect;
				mlTmp.getFreshRect(tmpRect);
				::InvalidateRect(hWnd,&tmpRect,true);
			}
		}
		break;

	case DRAW_CIRCLE:
		{
			if(BGS.isLBD1())
			{
				POINT c;
				BGS.getLastPoint(c);
				int tmpR = (int)sqrt((double)((c.x - MovingPt.x)*(c.x - MovingPt.x)+(c.y - MovingPt.y)*(c.y - MovingPt.y)));
				MyCircle mcTmp(c,tmpR);
				RECT tmpRect;
				mcTmp.getFreshRect(tmpRect);
				::InvalidateRect(hWnd,&tmpRect,true);
			}
		}
		break;
	case DRAW_RECTANGULAR:
		{
			if(BGS.isLBD1())
			{
				POINT s;
				BGS.getLastPoint(s);
				::MyRectangular mrTmp(s.x,s.y,MovingPt.x,MovingPt.y);
				RECT tmpRect;
				mrTmp.getFreshRect(tmpRect);
				::InvalidateRect(hWnd,&tmpRect,true);
			}
		}
		break;
	case DRAW_POLYGON:
		{
			if(BGS.TimesOfLBD() > 0)
			{
				POINT s;
				BGS.getLastPoint(s);
				MyLine mlTmp(s,MovingPt);
				RECT tmpRect;
				mlTmp.getFreshRect(tmpRect);
				::InvalidateRect(hWnd,&tmpRect,true);
			}
		}
		break;
	default:
		break;
	}
}

void CancelDrawing(HWND hWnd)
{
	if( BGS.TimesOfLBD() > 0 )
	{
		RECT fRect;
		switch(BGS.getES())
		{
		case DRAW_LINE:
			{
				POINT s;
				BGS.getLastPoint(s);
				MyLine mlTmp(s,MovingPt);
				mlTmp.getFreshRect(fRect);
			}
			break;

		case DRAW_CIRCLE:
			{
				POINT c;
				BGS.getLastPoint(c);
				int tmpR = (int)sqrt((double)((c.x - MovingPt.x)*(c.x - MovingPt.x)+(c.y - MovingPt.y)*(c.y - MovingPt.y)));
				MyCircle mcTmp(c,tmpR);
				mcTmp.getFreshRect(fRect);
			}
			break;

		case DRAW_RECTANGULAR:
			{
				POINT s;
				BGS.getLastPoint(s);
				::MyRectangular mrTmp(s.x,s.y,MovingPt.x,MovingPt.y);
				mrTmp.getFreshRect(fRect);
			}
			break;

		case DRAW_POLYGON:
			{
				std::vector<POINT> vP;
				BGS.CopyPointV(vP);
				vP.push_back(MovingPt);
				::MyPolygon mpTmp(&vP[0],(int)vP.size());
				mpTmp.getFreshRect(fRect);
			}
			break;

		default:
			break;
		}
		BGS.ESReset();
		::InvalidateRect(hWnd,&fRect,true);
	}
}

void EnableMenuSomeItems(HWND hWnd)
{
	for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
	{
		if((*it)->isSelected())
		{
			if((*it)->getGraphicsType() == TEXT)
			{
				EnableMenuItem(GetMenu(hWnd),ID_EDIT_DELETE, MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd),ID_EDIT_CHANGECOLOR, MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd),ID_EDIT_CHANGELINESTYLE, MF_GRAYED);
				EnableMenuItem(GetMenu(hWnd),ID_EDIT_CHANGETEXTFONT, MF_ENABLED);		
			}
			else
			{
				EnableMenuItem(GetMenu(hWnd),ID_EDIT_DELETE, MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd),ID_EDIT_CHANGECOLOR, MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd),ID_EDIT_CHANGELINESTYLE, MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd),ID_EDIT_CHANGETEXTFONT, MF_GRAYED);
			}
			break;
		}
		else
		{
			EnableMenuItem(GetMenu(hWnd),ID_EDIT_DELETE, MF_GRAYED);
			EnableMenuItem(GetMenu(hWnd),ID_EDIT_CHANGECOLOR, MF_GRAYED);
			EnableMenuItem(GetMenu(hWnd),ID_EDIT_CHANGELINESTYLE, MF_GRAYED);
			EnableMenuItem(GetMenu(hWnd),ID_EDIT_CHANGETEXTFONT, MF_GRAYED);
		}
	}
	if(CGO.LpGO.begin()== CGO.LpGO.end())
	{
			EnableMenuItem(GetMenu(hWnd),ID_EDIT_DELETE, MF_GRAYED);
			EnableMenuItem(GetMenu(hWnd),ID_EDIT_CHANGECOLOR, MF_GRAYED);
			EnableMenuItem(GetMenu(hWnd),ID_EDIT_CHANGELINESTYLE, MF_GRAYED);
			EnableMenuItem(GetMenu(hWnd),ID_EDIT_CHANGETEXTFONT, MF_GRAYED);
	}
}

void OnCOPYDATA(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
		COPYDATASTRUCT *pcds = (COPYDATASTRUCT *)lParam;
		switch((GraphicsType)pcds->dwData)
		{
		case OBJECT:
			break;
		
		case LINE:
			{
				MyLine *pml = new MyLine();
				pml->ImportStruct((MyLineStruct *)(pcds->lpData));
				CGO.LpGO.push_back(pml);
				
				for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					if((*it)->isSelected())
					{
						(*it)->setSelectedFlag(false);
						(*it)->FreshMyArea(hWnd,true);
						break;
					}
				}

				pml->FreshMyArea(hWnd,true);
				EnableMenuSomeItems(hWnd);				
			}
			break;
		
		case CIRCLE:
			{
				MyCircle *pmc = new MyCircle();
				pmc->ImportStruct((MyCircleStruct *)(pcds->lpData));
				CGO.LpGO.push_back(pmc);
				
				for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					if((*it)->isSelected())
					{
						(*it)->setSelectedFlag(false);
						(*it)->FreshMyArea(hWnd,true);
						break;
					}
				}

				pmc->FreshMyArea(hWnd,true);
				EnableMenuSomeItems(hWnd);
			}
			break;
		
		case RECTANGULAR:
			{
				MyRectangular *pmr = new MyRectangular();
				pmr->ImportStruct((MyRectStruct *)(pcds->lpData));
				CGO.LpGO.push_back(pmr);
				
				for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					if((*it)->isSelected())
					{
						(*it)->setSelectedFlag(false);
						(*it)->FreshMyArea(hWnd,true);
						break;
					}
				}

				pmr->FreshMyArea(hWnd,true);
				EnableMenuSomeItems(hWnd);
			}
			break;
		
		case POLYGON:
			{
				MyPolygon *pmp = new MyPolygon();
				pmp->ImportStruct((MyPolyStruct *)(pcds->lpData));
				CGO.LpGO.push_back(pmp);
				
				for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					if((*it)->isSelected())
					{
						(*it)->setSelectedFlag(false);
						(*it)->FreshMyArea(hWnd,true);
						break;
					}
				}

				pmp->FreshMyArea(hWnd,true);
				EnableMenuSomeItems(hWnd);
			}
			break;

		case TEXT:
			{
				MyText *pmt = new MyText();
				pmt->ImportStruct((MyTextStruct *)(pcds->lpData));
				CGO.LpGO.push_back(pmt);
				
				for (it= CGO.LpGO.begin(); it!= CGO.LpGO.end(); ++it)
				{
					if((*it)->isSelected())
					{
						(*it)->setSelectedFlag(false);
						(*it)->FreshMyArea(hWnd,true);
						break;
					}
				}

				pmt->FreshMyArea(hWnd,true);
				EnableMenuSomeItems(hWnd);
			}
			break;

		default:
			break;
	}
/*
HDC hdc = ::GetDC(hWnd);
TextOutA(hdc,0,0,(char*)ps->lpData,ps->dwData);
ReleaseDC(hWnd,hdc);
		WM_COPYDATA wParam = (WPARAM)(HWND) hwnd; 
    lParam = (LPARAM)(PCOPYDATASTRUCT) pcds;

typedef struct tagCOPYDATASTRUCT {
  DWORD dwData; //ID
  DWORD cbData; //SIZE
  PVOID lpData; //pCONTENT
} COPYDATASTRUCT; 

*/
}
void SendCopyDataMessage(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HWND to_hWnd =FindWindowEx(NULL,hWnd,szWindowClass,NULL);
	if(to_hWnd != NULL)
	{
		SendMessage(to_hWnd,WM_COPYDATA,wParam,lParam);
		SendCopyDataMessage(to_hWnd,wParam,lParam);
	}
	else return;
}