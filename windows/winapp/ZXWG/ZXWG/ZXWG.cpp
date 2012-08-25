// ZXWG.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ZXWG.h"
#include "MonsterRecognition.cpp"
#include "PlayerRecognition.cpp"
using namespace std;
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
LPCWSTR nameZx=L"Element Client";
HWND hZx=NULL;
DWORD dZx=NULL;
HANDLE hZxProcess=NULL;

vector<MonsterNamePicture> vMNP;

//Functions
/**/
void OutputColorArrayMaxMin(unsigned long *a,int size,wchar_t *name)
{
	long min = (long)(*a) , max = (long)(*a);
	long dmin = (long)(*(a+1)) - (long)(*a) , dmax = (long)(*(a+1)) - (long)(*a);
	for(int i=0;i<size;++i)
	{
		if((long)(*(a+i))>max) max = (long)(*(a+i));
		if((long)(*(a+i))<min) min = (long)(*(a+i));
	}
	for(int i=0;i<size-1;++i)
	{
		if(((long)(*(a+i+1))-(long)(*(a+i)))>dmax) dmax = (long)(*(a+i+1))-(long)(*(a+i));
		if(((long)(*(a+i+1))-(long)(*(a+i)))<dmin) dmin = (long)(*(a+i+1))-(long)(*(a+i));
	}
	wchar_t minStr[100],maxStr[100],dminStr[100],dmaxStr[100];
	wsprintf(minStr,L" min = (16)0x%x,(10)%d\t",min,min);
	wsprintf(maxStr,L" max = (16)0x%x,(10)%d\n",max,max);
	wsprintf(dminStr,L"dmin = (16)0x%x,(10)%d\t",dmin,dmin);
	wsprintf(dmaxStr,L"dmax = (16)0x%x,(10)%d\n",dmax,dmax);
	OutputDebugString(L"********************************************   ");
	OutputDebugString(name);
	OutputDebugString(L"   ********************************************\n");
	OutputDebugString(minStr);
	OutputDebugString(maxStr);
	OutputDebugString(dminStr);
	OutputDebugString(dmaxStr);
	OutputDebugString(L"********************************************   ");
	OutputDebugString(name);
	OutputDebugString(L"   ********************************************\n\n");
}

void OutputHpMpArray(unsigned long *a,int size,wchar_t *name)
{
	OutputDebugString(L"********************************************   ");
	OutputDebugString(name);
	OutputDebugString(L"   ********************************************");
	for(int i=0;i<size;++i)
	{
		if(i%12==0) OutputDebugString(L"\n");
		wchar_t temp[10];
		wsprintf(temp,L"%ld",*(a+i));
		OutputDebugString(temp);
		if(i!=size-1) OutputDebugString(L",");
	}
	OutputDebugString(L"\n********************************************   ");
	OutputDebugString(name);
	OutputDebugString(L"   ********************************************\n\n");
}

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

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
	LoadString(hInstance, IDC_ZXWG, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ZXWG));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ZXWG));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ZXWG);
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
      0, 0, 200,600, NULL, NULL, hInstance, NULL);

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
	case WM_CREATE:
		hZx=FindWindow(NULL,nameZx);
		if(hZx==NULL)
		{
			MessageBox(hWnd,L"ÇëÏÈÔËÐÐÓÎÏ·£¡",L"ERROR",MB_ICONERROR);
			SendMessage(hWnd,WM_CLOSE,0,0);
		}
		else
		{
			MonsterNamePicture mnp1(L"ºûµû",normal);
			MonsterNamePicture mnp2(L"É½Öí",normal);
			MonsterNamePicture mnp3(L"Ô©Áé",normal);
			MonsterNamePicture mnp4(L"Ð×Áé",normal);
			vMNP.push_back(mnp1);
			vMNP.push_back(mnp2);
			vMNP.push_back(mnp3);
			vMNP.push_back(mnp4);
		}
		break;
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
		case ID_OP_START:
			//::MessageBox(hWnd,nameZx,nameZx,MB_OK);

			::BringWindowToTop(hZx);

		
			Sleep(5000);
			for(int n=0;n<3;n++)
			{
			for(int i=0;i<2;i++)
			{
				keybd_event(VK_TAB,0,0,0); 
				Sleep(100);
				keybd_event(VK_TAB,0,KEYEVENTF_KEYUP,0);
				Sleep(2000);
			}
			for(int i=0;i<15;i++)
			{
				if(i%3==2)
				{
					keybd_event('2',0,0,0); 
					Sleep(100);
					keybd_event('2',0,KEYEVENTF_KEYUP,0);
					Sleep(1000);
				}else{
					keybd_event('1',0,0,0); 
					Sleep(100);
					keybd_event('1',0,KEYEVENTF_KEYUP,0);
					Sleep(1000);
				}
			}
			{
				Sleep(1000);
				keybd_event(VK_F7,0,0,0); 
				Sleep(100);
				keybd_event(VK_F7,0,KEYEVENTF_KEYUP,0);
				Sleep(10000);
				keybd_event(VK_F7,0,0,0); 
				Sleep(100);
				keybd_event(VK_F7,0,KEYEVENTF_KEYUP,0);
			}
			}
			break;
		case ID_OP_OPENPROCESS:
			GetWindowThreadProcessId(hZx,&dZx);
			hZxProcess =OpenProcess(PROCESS_ALL_ACCESS, false, dZx);
			if(hZxProcess!=NULL)
			{
				MessageBox(hWnd,L"Y",L"Y",MB_OK);
				char id[11];
				wchar_t name[6];
				LPCVOID ID_addr=NULL;
				LPCVOID NAME_addr=NULL;
				SIZE_T sign=0;
				ReadProcessMemory(hZxProcess,(LPCVOID)0x03B28264,&ID_addr,4,&sign);
				ReadProcessMemory(hZxProcess,ID_addr,id,11,&sign);
				ReadProcessMemory(hZxProcess,(LPCVOID)0x0091528C,&NAME_addr,4,&sign);
				ReadProcessMemory(hZxProcess,NAME_addr,name,12,&sign);
				::MessageBox(hWnd,name,name,MB_OK);
				::CloseHandle(hZxProcess);
			}
			else
			{
				MessageBox(hWnd,L"N",L"N",MB_OK);
			}
			break;
		case ID_OP_GETPIXEL:
			::BringWindowToTop(hZx);
			Sleep(3000);
			for(int n=0;n<10;n++)
			{
			HDC hdcZX;
			HDC hdcWG;
			::COLORREF CurrentMonster[118][14];
			::COLORREF _CurrentMonster[120][16];
			::COLORREF MonsterLife[118][8];
			::COLORREF _MonsterLife[120][11];
			hdcZX=GetDC(hZx);
			for(int i=0;i<118;i++)
				for(int j=0;j<14;j++)
				{
					CurrentMonster[i][j]=::GetPixel(hdcZX,431+i,30+j);
				}
			for(int i=0;i<120;i++)
				for(int j=0;j<16;j++)
				{
					_CurrentMonster[i][j]=::GetPixel(hdcZX,430+i,29+j);
				}
			for(int i=0;i<118;i++)
				for(int j=0;j<8;j++)
				{
					MonsterLife[i][j]=::GetPixel(hdcZX,431+i,53+j);
				}
			for(int i=0;i<120;i++)
				for(int j=0;j<11;j++)
				{
					_MonsterLife[i][j]=::GetPixel(hdcZX,430+i,51+j);
				}	
			ReleaseDC(hZx,hdcZX);
			hdcWG=GetDC(hWnd);
			for(int i=0;i<118;i++)
				for(int j=0;j<14;j++)
				{
					SetPixel(hdcWG,5+i,5+j,CurrentMonster[i][j]);
				}
			for(int i=0;i<120;i++)
				for(int j=0;j<16;j++)
				{
					SetPixel(hdcWG,5+i,30+j,_CurrentMonster[i][j]);
				}
			for(int i=0;i<118;i++)
				for(int j=0;j<8;j++)
				{
					SetPixel(hdcWG,5+i,55+j,MonsterLife[i][j]);
				}
			for(int i=0;i<120;i++)
				for(int j=0;j<11;j++)
				{
					SetPixel(hdcWG,5+i,80+j,_MonsterLife[i][j]);
				}
			ReleaseDC(hWnd,hdcWG);
			Sleep(10);
			
			WCHAR test[20];
			wsprintf(test,L"%d,0x%x\n",CurrentMonster[n][n],CurrentMonster[n][n]);
			::OutputDebugString(test);
			WCHAR test1[20];
			wsprintf(test1,L"%d,0x%x\n",_MonsterLife[n][n],_MonsterLife[n][n]);
			::OutputDebugString(test1);
			}
			break;
		case ID_OP_GETMAINPIXEL:
			{
				//long right=790032&0xff;
				long right=(790032<<24)>>24;
				//long center=(790032&0xff00)>>8;
				long center=(790032<<16)>>24;
				//long left=(790032)>>16;
				long left=(790032<<8)>>24;
				long stop=(790032)>>16;
			}
			break;
		case ID_OP_GETWILLDIEPIXEL:
			{
				::BringWindowToTop(hZx);
			Sleep(3000);

			HDC hdcZX;
			HDC hdcWG;
			::COLORREF temp[11][4];
			hdcZX=GetDC(hZx);
			for(int i=0;i<11;i++)
				for(int j=0;j<4;j++)
				{
					temp[i][j]=::GetPixel(hdcZX,430+j,51+i);
				}

			ReleaseDC(hZx,hdcZX);
			hdcWG=GetDC(hWnd);
			for(int i=0;i<11;i++)
				for(int j=0;j<4;j++)
				{
					SetPixel(hdcWG,5+j,200+i,temp[i][j]);
				}

			ReleaseDC(hWnd,hdcWG);
			Sleep(10);
			for(int i=0;i<11;i++){
			WCHAR test[80];
			wsprintf(test,L"0x%x,0x%x,0x%x,0x%x\n",temp[i][0],temp[i][1],temp[i][2],temp[i][3]);
			::OutputDebugString(test);
			}
			::MessageBox(NULL,L"ok",L"ok",MB_OK);
			}
			break;
		case ID_OP_GETPLAYERINFOPIXEL:
			{
				::BringWindowToTop(hZx);
				Sleep(3000);

				HDC hdcZX;
				HDC hdcWG;
				::COLORREF temp[11][118];
				hdcZX=GetDC(hZx);
				for(int i=0;i<11;i++)
					for(int j=0;j<118;j++)
					{
						temp[i][j]=::GetPixel(hdcZX,130+j,61+i);
					}

				ReleaseDC(hZx,hdcZX);
				hdcWG=GetDC(hWnd);
				for(int i=0;i<11;i++)
					for(int j=0;j<118;j++)
					{
						SetPixel(hdcWG,5+j,300+i,temp[i][j]);
					}

				ReleaseDC(hWnd,hdcWG);
				Sleep(10);
				for(int i=0;i<11;i++){
				WCHAR test[80];
				wsprintf(test,L"0x%x,0x%x,0x%x,0x%x\n",temp[i][0],temp[i][1],temp[i][2],temp[i][3]);
				::OutputDebugString(test);
				}
				::MessageBox(NULL,L"ok",L"ok",MB_OK);
			}
			break;

		case ID_OP_GETPLAYERHP:
			{
				::BringWindowToTop(hZx);
				Sleep(3000);

				HDC hdcZX;
				HDC hdcWG;
				::COLORREF temp[11][118];
				hdcZX=GetDC(hZx);
				for(int i=0;i<11;i++)
					for(int j=0;j<118;j++)
					{
						temp[i][j]=::GetPixel(hdcZX,123+j,41+i);
					}
				COLORREF HPUp[4][118];
				COLORREF HPDown[4][118];
				for(int n=0;n<118;n++)
				{//0x00bbggrr(0,128,255)[1][2][3]
					HPUp[0][n] = GetPixel(hdcZX,123+n,41);
					HPUp[1][n] = (HPUp[0][n]<<8)>>24;
					HPUp[2][n] = (HPUp[0][n]<<16)>>24;
					HPUp[3][n] = (HPUp[0][n]<<24)>>24;

					HPDown[0][n] = GetPixel(hdcZX,123+n,51);
					HPDown[1][n] = (HPDown[0][n]<<8)>>24;
					HPDown[2][n] = (HPDown[0][n]<<16)>>24;
					HPDown[3][n] = (HPDown[0][n]<<24)>>24;
				}
				OutputColorArrayMaxMin(&HPUp[0][0],108,L"HPUp A");
				OutputColorArrayMaxMin(&HPUp[1][0],108,L"HPUp R");
				OutputColorArrayMaxMin(&HPUp[2][0],108,L"HPUp G");
				OutputColorArrayMaxMin(&HPUp[3][0],108,L"HPUp B");
				OutputColorArrayMaxMin(&HPDown[0][0],108,L"HPDown A");
				OutputColorArrayMaxMin(&HPDown[1][0],108,L"HPDown R");
				OutputColorArrayMaxMin(&HPDown[2][0],108,L"HPDown G");
				OutputColorArrayMaxMin(&HPDown[3][0],108,L"HPDown B");

				ReleaseDC(hZx,hdcZX);
				hdcWG=GetDC(hWnd);
				for(int i=0;i<11;i++)
					for(int j=0;j<118;j++)
					{
						SetPixel(hdcWG,5+j,300+i,temp[i][j]);
					}

				ReleaseDC(hWnd,hdcWG);

				::MessageBox(NULL,L"ok",L"ok",MB_OK);
			}
			break;

		case ID_OP_GETPLAYERMP:
			{
				::BringWindowToTop(hZx);
				Sleep(3000);

				HDC hdcZX;
				HDC hdcWG;
				::COLORREF temp[11][118];
				hdcZX=GetDC(hZx);
				for(int i=0;i<11;i++)
					for(int j=0;j<118;j++)
					{
						temp[i][j]=::GetPixel(hdcZX,123+j,59+i);
					}

				COLORREF MPUp[4][118];
				COLORREF MPDown[4][118];
				for(int n=0;n<118;n++)
				{//0x00bbggrr(0,128,255)[1][2][3]
					MPUp[0][n] = GetPixel(hdcZX,123+n,59);
					MPUp[1][n] = (MPUp[0][n]<<8)>>24;
					MPUp[2][n] = (MPUp[0][n]<<16)>>24;
					MPUp[3][n] = (MPUp[0][n]<<24)>>24;

					MPDown[0][n] = GetPixel(hdcZX,123+n,69);
					MPDown[1][n] = (MPDown[0][n]<<8)>>24;
					MPDown[2][n] = (MPDown[0][n]<<16)>>24;
					MPDown[3][n] = (MPDown[0][n]<<24)>>24;
				}
				OutputColorArrayMaxMin(&MPUp[0][0],108,L"MPUp A");
				OutputColorArrayMaxMin(&MPUp[1][0],108,L"MPUp R");
				OutputColorArrayMaxMin(&MPUp[2][0],108,L"MPUp G");
				OutputColorArrayMaxMin(&MPUp[3][0],108,L"MPUp B");
				OutputColorArrayMaxMin(&MPDown[0][0],108,L"MPDown A");
				OutputColorArrayMaxMin(&MPDown[1][0],108,L"MPDown R");
				OutputColorArrayMaxMin(&MPDown[2][0],108,L"MPDown G");
				OutputColorArrayMaxMin(&MPDown[3][0],108,L"MPDown B");
				ReleaseDC(hZx,hdcZX);
				hdcWG=GetDC(hWnd);
				for(int i=0;i<11;i++)
					for(int j=0;j<118;j++)
					{
						SetPixel(hdcWG,5+j,300+i,temp[i][j]);
					}

				ReleaseDC(hWnd,hdcWG);
				::MessageBox(NULL,L"ok",L"ok",MB_OK);
			
			}
			break;

		case ID_OP_GETPLAYERHPMP:
			{
				::BringWindowToTop(hZx);
				Sleep(3000);

				HDC hdcZX;
				hdcZX=GetDC(hZx);
				COLORREF HPUp[4][118];
				COLORREF HPDown[4][118];
				for(int n=0;n<118;n++)
				{//0x00bbggrr(0,128,255)[1][2][3]
					HPUp[0][n] = GetPixel(hdcZX,123+n,41);
					HPUp[1][n] = (HPUp[0][n]<<8)>>24;
					HPUp[2][n] = (HPUp[0][n]<<16)>>24;
					HPUp[3][n] = (HPUp[0][n]<<24)>>24;

					HPDown[0][n] = GetPixel(hdcZX,123+n,51);
					HPDown[1][n] = (HPDown[0][n]<<8)>>24;
					HPDown[2][n] = (HPDown[0][n]<<16)>>24;
					HPDown[3][n] = (HPDown[0][n]<<24)>>24;
				}
				OutputColorArrayMaxMin(&HPUp[0][0],108,L"HPUp A");
				OutputColorArrayMaxMin(&HPUp[1][0],108,L"HPUp B");
				OutputColorArrayMaxMin(&HPUp[2][0],108,L"HPUp G");
				OutputColorArrayMaxMin(&HPUp[3][0],108,L"HPUp R");
				OutputColorArrayMaxMin(&HPDown[0][0],108,L"HPDown A");
				OutputColorArrayMaxMin(&HPDown[1][0],108,L"HPDown B");
				OutputColorArrayMaxMin(&HPDown[2][0],108,L"HPDown G");
				OutputColorArrayMaxMin(&HPDown[3][0],108,L"HPDown R");

				COLORREF MPUp[4][118];
				COLORREF MPDown[4][118];
				for(int n=0;n<118;n++)
				{//0x00bbggrr(0,128,255)[1][2][3]
					MPUp[0][n] = GetPixel(hdcZX,123+n,59);
					MPUp[1][n] = (MPUp[0][n]<<8)>>24;
					MPUp[2][n] = (MPUp[0][n]<<16)>>24;
					MPUp[3][n] = (MPUp[0][n]<<24)>>24;

					MPDown[0][n] = GetPixel(hdcZX,123+n,69);
					MPDown[1][n] = (MPDown[0][n]<<8)>>24;
					MPDown[2][n] = (MPDown[0][n]<<16)>>24;
					MPDown[3][n] = (MPDown[0][n]<<24)>>24;
				}
				OutputColorArrayMaxMin(&MPUp[0][0],108,L"MPUp A");
				OutputColorArrayMaxMin(&MPUp[1][0],108,L"MPUp B");
				OutputColorArrayMaxMin(&MPUp[2][0],108,L"MPUp G");
				OutputColorArrayMaxMin(&MPUp[3][0],108,L"MPUp R");
				OutputColorArrayMaxMin(&MPDown[0][0],108,L"MPDown A");
				OutputColorArrayMaxMin(&MPDown[1][0],108,L"MPDown B");
				OutputColorArrayMaxMin(&MPDown[2][0],108,L"MPDown G");
				OutputColorArrayMaxMin(&MPDown[3][0],108,L"MPDown R");

				ReleaseDC(hZx,hdcZX);

				::MessageBox(NULL,L"ok",L"ok",MB_OK);
			}
			break;

		case ID_OP_GETPLAYERHPMPARRAY:
			{
				::BringWindowToTop(hZx);
				Sleep(3000);

				HDC hdcZX;
				hdcZX=GetDC(hZx);
				COLORREF HPUp[4][118];
				COLORREF HPDown[4][118];
				for(int n=0;n<118;n++)
				{//0x00bbggrr(0,128,255)[1][2][3]
					HPUp[0][n] = GetPixel(hdcZX,123+n,41);
					HPUp[1][n] = (HPUp[0][n]<<8)>>24;
					HPUp[2][n] = (HPUp[0][n]<<16)>>24;
					HPUp[3][n] = (HPUp[0][n]<<24)>>24;

					HPDown[0][n] = GetPixel(hdcZX,123+n,51);
					HPDown[1][n] = (HPDown[0][n]<<8)>>24;
					HPDown[2][n] = (HPDown[0][n]<<16)>>24;
					HPDown[3][n] = (HPDown[0][n]<<24)>>24;
				}
				OutputHpMpArray(&HPUp[0][0],108,L"HPUp A");
				OutputHpMpArray(&HPUp[1][0],108,L"HPUp B");
				OutputHpMpArray(&HPUp[2][0],108,L"HPUp G");
				OutputHpMpArray(&HPUp[3][0],108,L"HPUp R");
				OutputHpMpArray(&HPDown[0][0],108,L"HPDown A");
				OutputHpMpArray(&HPDown[1][0],108,L"HPDown B");
				OutputHpMpArray(&HPDown[2][0],108,L"HPDown G");
				OutputHpMpArray(&HPDown[3][0],108,L"HPDown R");

				COLORREF MPUp[4][118];
				COLORREF MPDown[4][118];
				for(int n=0;n<118;n++)
				{//0x00bbggrr(0,128,255)[1][2][3]
					MPUp[0][n] = GetPixel(hdcZX,123+n,59);
					MPUp[1][n] = (MPUp[0][n]<<8)>>24;
					MPUp[2][n] = (MPUp[0][n]<<16)>>24;
					MPUp[3][n] = (MPUp[0][n]<<24)>>24;

					MPDown[0][n] = GetPixel(hdcZX,123+n,69);
					MPDown[1][n] = (MPDown[0][n]<<8)>>24;
					MPDown[2][n] = (MPDown[0][n]<<16)>>24;
					MPDown[3][n] = (MPDown[0][n]<<24)>>24;
				}
				OutputHpMpArray(&MPUp[0][0],108,L"MPUp A");
				OutputHpMpArray(&MPUp[1][0],108,L"MPUp B");
				OutputHpMpArray(&MPUp[2][0],108,L"MPUp G");
				OutputHpMpArray(&MPUp[3][0],108,L"MPUp R");
				OutputHpMpArray(&MPDown[0][0],108,L"MPDown A");
				OutputHpMpArray(&MPDown[1][0],108,L"MPDown B");
				OutputHpMpArray(&MPDown[2][0],108,L"MPDown G");
				OutputHpMpArray(&MPDown[3][0],108,L"MPDown R");

				ReleaseDC(hZx,hdcZX);

				::MessageBox(NULL,L"ok",L"ok",MB_OK);
			}
			break;

		case ID_TESTMR_ISSEEINGMONSTER:
			{
				::BringWindowToTop(hZx);
				Sleep(3000);
				if(MonsterRecognition::isSeeingMonster(hZx,HIGHEST)) ::MessageBox(NULL,L"y",L"y",MB_OK);
				else ::MessageBox(NULL,L"n",L"n",MB_OK);
			}
			break;
		case ID_TESTMR_ISMONSTER:
			{
				::BringWindowToTop(hZx);
				Sleep(3000);
				if(MonsterRecognition::isMonster(vMNP[0],hZx,HIGHEST)) ::MessageBox(NULL,L"ºûµû",L"ºûµû",MB_OK);
				else if(MonsterRecognition::isMonster(vMNP[1],hZx,HIGHEST)) ::MessageBox(NULL,L"É½Öí",L"É½Öí",MB_OK);
				else ::MessageBox(NULL,L"n",L"n",MB_OK);
			}
			break;

		case ID_TESTMR_ISMONSTERFULLHP:
			{
				::BringWindowToTop(hZx);
				Sleep(3000);
				if(MonsterRecognition::isMonsterFullHP(hZx,LOWEST)) ::MessageBox(NULL,L"y",L"y",MB_OK);
				else ::MessageBox(NULL,L"n",L"n",MB_OK);
			}
			break;

		case ID_TESTMR_WILLMONSTERDIE:
			{
				::BringWindowToTop(hZx);
				Sleep(3000);
				if(MonsterRecognition::willMonsterDie(hZx,LOWEST)) ::MessageBox(NULL,L"y",L"y",MB_OK);
				else ::MessageBox(NULL,L"n",L"n",MB_OK);
			}
			break;

		case ID_TESTMR_ISMONSTERDEAD:
			{
				::BringWindowToTop(hZx);
				Sleep(3000);
				if(MonsterRecognition::isMonsterDead(hZx,LOWEST)) ::MessageBox(NULL,L"y",L"y",MB_OK);
				else ::MessageBox(NULL,L"n",L"n",MB_OK);
			}
			break;

		case ID_TESTPR_ISFIGHTINGNOW:
		{
			::BringWindowToTop(hZx);
			Sleep(3000);
			PlayerRecognition pr;
			if(pr.isFightingNow(hZx,L)) MessageBox(NULL,L"y",L"y",MB_OK); 
			else MessageBox(NULL,L"n",L"n",MB_OK); 
		}
		break;
		
		case ID_TESTPR_GETPLAYERHPNUMNOTFIGHTING:
		{
			::BringWindowToTop(hZx);
			Sleep(3000);
			PlayerRecognition pr;
			int hp_nf = pr.getPlayerHpNumNotFighting(hZx,H);
			WCHAR test[80];
			wsprintf(test,L"%d",hp_nf);
			::OutputDebugString(test);
			MessageBox(NULL,test,test,MB_OK); 

		}
		break;

		case ID_TESTPR_GETPLAYERMPNUMNOTFIGHTING:
		{
			::BringWindowToTop(hZx);
			Sleep(3000);
			PlayerRecognition pr;
			int mp_nf = pr.getPlayerMpNumNotFighting(hZx,M);
			WCHAR test[80];
			wsprintf(test,L"%d",mp_nf);
			::OutputDebugString(test);
			MessageBox(NULL,test,test,MB_OK); 

		}
		break;

		case ID_TESTPR_GETPLAYERHPNUMWHENFIGHTING:
			{
			::BringWindowToTop(hZx);
			Sleep(3000);
			PlayerRecognition pr;
			int hp_wf = pr.getPlayerHpNumWhenFighting(hZx,H);
			WCHAR test[80];
			wsprintf(test,L"%d",hp_wf);
			::OutputDebugString(test);
			MessageBox(NULL,test,test,MB_OK); 
			}
			break;
		
		case ID_TESTPR_GETPLAYERMPNUMWHENFIGHTING:
			{
			::BringWindowToTop(hZx);
			Sleep(3000);
			PlayerRecognition pr;
			int hp_wf = pr.getPlayerMpNumWhenFighting(hZx,H);
			WCHAR test[80];
			wsprintf(test,L"%d",hp_wf);
			::OutputDebugString(test);
			MessageBox(NULL,test,test,MB_OK); 
			}
			break;

		case ID_TESTPR_GETPLAYERHPPERCENTAGE:
			{
			::BringWindowToTop(hZx);
			Sleep(3000);
			PlayerRecognition pr;
			double hp_wf = pr.getPlayerHpPercentage(hZx,L);
			WCHAR test[80];
			//wsprintf(test,L"%.2f",hp_wf);
			swprintf(test,L"%.2f",hp_wf);
			::OutputDebugString(test);
			MessageBox(NULL,test,test,MB_OK); 
			}
			break;

		case ID_TESTPR_GETPLAYERMPPERCENTAGE:
			{
			::BringWindowToTop(hZx);
			Sleep(3000);
			PlayerRecognition pr;
			double hp_wf = pr.getPlayerMpPercentage(hZx,L);
			WCHAR test[80];
			//wsprintf(test,L"%.2f",hp_wf);
			swprintf(test,L"%.2f",hp_wf);
			::OutputDebugString(test);
			MessageBox(NULL,test,test,MB_OK); 
			}
			break;

		case ID_TESTWG_BEGIN:
			{
				MonsterRecognitionAccuracy mra=MIDDLE;
				PlayerRecognitionAccuracy pra=M;
				PlayerRecognition pr;
				MonsterRecognition mr;
				::BringWindowToTop(hZx);
				Sleep(3000);
				do
				{
					if(pr.getPlayerHpPercentage(hZx,pra)<0.4||pr.getPlayerMpPercentage(hZx,pra)<0.2)
					{
						Sleep(500);
						keybd_event(VK_F1,0,0,0); 
						Sleep(100);
						keybd_event(VK_F1,0,KEYEVENTF_KEYUP,0);
						while(true)
						{
							if(pr.getPlayerHpPercentage(hZx,pra)>0.99&&pr.getPlayerMpPercentage(hZx,pra)>0.99)
							{
								Sleep(500);
								keybd_event(VK_F1,0,0,0); 
								Sleep(100);
								keybd_event(VK_F1,0,KEYEVENTF_KEYUP,0);
								break;
							}
							Sleep(500);
						}
					}
					else if(mr.isSeeingMonster(hZx,mra)&&mr.isMonsterFullHP(hZx,mra)/*&&mr.isMonster((MonsterNamePicture)(vMNP[2]),hZx,mra)*/)
					{
						int i=0;
						do
						{
							if(i==0) //·¨±¦¹¥»÷
							{
								keybd_event('2',0,0,0); 
								Sleep(100);
								keybd_event('2',0,KEYEVENTF_KEYUP,0);
								Sleep(2000);
							}
							else if(i%3==2)
							{
								keybd_event('3',0,0,0); 
								Sleep(100);
								keybd_event('3',0,KEYEVENTF_KEYUP,0);
								Sleep(2000);
							}else{
								keybd_event('1',0,0,0); 
								Sleep(100);
								keybd_event('1',0,KEYEVENTF_KEYUP,0);
								Sleep(1000);
							}
							++i;
							if(mr.isMonsterDead(hZx,mra))
							{
								for(int i=0;i<3;i++)
								{	
									keybd_event(VK_F2,0,0,0); 
									Sleep(100);
									keybd_event(VK_F2,0,KEYEVENTF_KEYUP,0);
									Sleep(800);
								}
							}
						}while(!mr.isMonsterDead(hZx,mra));		
						Sleep(1000);
					}
					else
					{
						keybd_event(VK_TAB,0,0,0); 
						Sleep(100);
						keybd_event(VK_TAB,0,KEYEVENTF_KEYUP,0);
						Sleep(1000);
					}
				}while(pr.getPlayerHpPercentage(hZx,pra)>0.01);

			}
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
