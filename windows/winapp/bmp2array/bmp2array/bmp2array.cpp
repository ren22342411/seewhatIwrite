// bmp2array.cpp : Defines the entry point for the application.
//


#include "stdafx.h"
#include "bmp2array.h"
#include "bmp.h"
#include "testArray.h"



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

int f(int z,char a[])
{
	if(z==0) 
	{
		a[0]='0';
		return 1;
	}
	else if( z > 0 && z < 256 )
	{
		int n=0;
		if(z>0 && z<10)
		{
			a[0]=z+'0';
			n=1;
		}
		if(z>=10 && z<100)
		{
			a[0]=z/10+'0';
			a[1]=z%10+'0';
			n=2;
		}
		if(z>=100 && z<256)
		{
			a[0]=z/100+'0';
			a[1]=(z/10)%10+'0';
			a[2]=z%10+'0';
			n=3;
		}
			return n;
	}
	else return -1;
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
	LoadString(hInstance, IDC_BMP2ARRAY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BMP2ARRAY));

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
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BMP2ARRAY));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BMP2ARRAY);
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

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 200, 200, NULL, NULL, hInstance, NULL);

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
		case ID_FILE_START:
			{
			LONG l;
			BYTE pBits[118][14][4];
			HBITMAP test=(HBITMAP)LoadImage(NULL,L"pic\\ºûµû4.bmp",IMAGE_BITMAP,118,14,LR_LOADFROMFILE);
			HANDLE testFile=CreateFile(L"headbmp\\ºûµû4.txt",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
			if(testFile!=(HANDLE)-1) MessageBox(hWnd,L"open",L"ok",MB_OK);
			l=GetBitmapBits(test,118*14*4,pBits);
			char use[4]={'{','}',',','\n'};
			unsigned long lWritten;
			WriteFile(testFile,&use[0],sizeof(char),&lWritten,NULL);
			WriteFile(testFile,&use[3],sizeof(char),&lWritten,NULL);
			for(int x=0;x<118;x++)
			{
				WriteFile(testFile,&use[0],sizeof(char),&lWritten,NULL);
				for(int y=0;y<14;y++)
				{
					WriteFile(testFile,&use[0],sizeof(char),&lWritten,NULL);
					for(int i=0;i<4;i++)
					{
						int z=pBits[x][y][i];
						char a[4];
						int n=f(z,a);
						for(int j=0;j<n;j++)
							WriteFile(testFile,&a[j],sizeof(char),&lWritten,NULL);
						if (i!=3) WriteFile(testFile,&use[2],sizeof(char),&lWritten,NULL);
					}
					WriteFile(testFile,&use[1],sizeof(char),&lWritten,NULL);
					if (y!=13) WriteFile(testFile,&use[2],sizeof(char),&lWritten,NULL);
				}
				WriteFile(testFile,&use[1],sizeof(char),&lWritten,NULL);
				if (x!=117) WriteFile(testFile,&use[2],sizeof(char),&lWritten,NULL);
				WriteFile(testFile,&use[3],sizeof(char),&lWritten,NULL);
			}
			WriteFile(testFile,&use[1],sizeof(char),&lWritten,NULL);
			if(CloseHandle(testFile)) MessageBox(hWnd,L"close",L"ok",MB_OK);
			}
			break;
		case ID_CMP:
			//MessageBox(NULL,L"",L"",MB_OK);
			{
			LONG l;
			BYTE pBits1[118][14][4];
			BYTE pBits2[118][14][4];
			HBITMAP test1=(HBITMAP)LoadImage(NULL,L"pic\\ºûµû3.bmp",IMAGE_BITMAP,118,14,LR_LOADFROMFILE);
			HBITMAP test2=(HBITMAP)LoadImage(NULL,L"pic\\ºûµû4.bmp",IMAGE_BITMAP,118,14,LR_LOADFROMFILE);
			l=GetBitmapBits(test1,118*14*4,pBits1);
			l=GetBitmapBits(test2,118*14*4,pBits2);
			long s=0;
			long d=0;
			long ss=0;
			long dd=0;
			long ss1=0;
			long dd1=0;
			long ss2=0;
			long dd2=0;
			for(int x=0;x<118;x++)
			{
				for(int y=0;y<14;y++)
				{
					for(int i=0;i<3;i++)
					{
						if(pBits1[x][y][i]!=pBits2[x][y][i])
						d++;
						else
						s++;
					}
					if(pBits1[x][y][0]==12&&pBits1[x][y][1]==14&&pBits1[x][y][2]==16)
						ss1++;
					if(pBits1[x][y][0]!=12||pBits1[x][y][1]!=14||pBits1[x][y][2]!=16)
						dd1++;
					if(pBits2[x][y][0]==12&&pBits2[x][y][1]==14&&pBits2[x][y][2]==16)
						ss2++;
					if(pBits2[x][y][0]!=12||pBits2[x][y][1]!=14||pBits2[x][y][2]!=16)
						dd2++;
					if(pBits1[x][y][0]==pBits2[x][y][0]&&pBits1[x][y][1]==pBits2[x][y][1]&&pBits1[x][y][2]==pBits2[x][y][2])
						ss++;
					else
						dd++;
				}
			}
			
			//if(s==118*14*3) MessageBox(NULL,L"s",L"s",MB_OK); 
			//if(d!=0) MessageBox(NULL,L"d",L"d",MB_OK); 
			WCHAR sStr[20];
			::wsprintf(sStr,L"s:%d\n",s);
			::OutputDebugString(sStr);
			WCHAR dStr[20];
			::wsprintf(dStr,L"d:%d\n",d);
			::OutputDebugString(dStr);
			WCHAR ssStr[20];
			::wsprintf(ssStr,L"ss:%d\n",ss);
			::OutputDebugString(ssStr);
			WCHAR ddStr[20];
			::wsprintf(ddStr,L"dd:%d\n",dd);
			::OutputDebugString(ddStr);
			WCHAR ss1Str[20];
			::wsprintf(ss1Str,L"ss1:%d\n",ss1);
			::OutputDebugString(ss1Str);
			WCHAR ss2Str[20];
			::wsprintf(ss2Str,L"ss2:%d\n",ss2);
			::OutputDebugString(ss2Str);
			WCHAR dd1Str[20];
			::wsprintf(dd1Str,L"dd1:%d\n",dd1);
			::OutputDebugString(dd1Str);
			WCHAR dd2Str[20];
			::wsprintf(dd2Str,L"dd2:%d\n",dd2);
			::OutputDebugString(dd2Str);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
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
