// NatureParkPocketPC.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "NatureParkPocketPC.h"
#include <windows.h>
#include <commctrl.h>
#include <vector>
using namespace std;
#include "NP_BITMAP.h"
#include "block.h"


#define MAX_LOADSTRING 100
/*
BOOL IsExsit(TCHAR *szTarget)
{
HANDLE hFile = CreateFile(szTarget,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
if(hFile == INVALID_HANDLE_VALUE)
return FALSE;
else
{
CloseHandle(hFile);
return TRUE;
}
}*/

// Global Variables:
HINSTANCE			g_hInst;			// current instance
HWND				g_hWndMenuBar;		// menu bar handle

// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
NP_MSG npmsg=GAME_INITIAL;
NP_MSG gamestate;
BlockBoard BB;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	MSG msg;
	/*if(IsExsit(L"pic\\NP_BB.bmp")) {
		MessageBox(NULL,L"ok",L"ok",MB_OK);
		MessageBox(NULL,L"ok",L"ok",MB_OK);
	}
	else{
		MessageBox(NULL,L"error",L"error",MB_OK);
		MessageBox(NULL,L"error",L"error",MB_OK);
	}*/
	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NATUREPARKPOCKETPC));

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
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NATUREPARKPOCKETPC));
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
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
    TCHAR szTitle[MAX_LOADSTRING];		// title bar text
    TCHAR szWindowClass[MAX_LOADSTRING];	// main window class name

    g_hInst = hInstance; // Store instance handle in our global variable

    // SHInitExtraControls should be called once during your application's initialization to initialize any
    // of the device specific controls such as CAPEDIT and SIPPREF.
    SHInitExtraControls();

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_NATUREPARKPOCKETPC, szWindowClass, MAX_LOADSTRING);

    //If it is already running, then focus on the window, and exit
    hWnd = FindWindow(szWindowClass, szTitle);	
    if (hWnd) 
    {
        // set focus to foremost child window
        // The "| 0x00000001" is used to bring any owned windows to the foreground and
        // activate them.
        SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));
        return 0;
    } 

    if (!MyRegisterClass(hInstance, szWindowClass))
    {
    	return FALSE;
    }

    hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    // When the main window is created using CW_USEDEFAULT the height of the menubar (if one
    // is created is not taken into account). So we resize the window after creating it
    // if a menubar is present
    if (g_hWndMenuBar)
    {
        RECT rc;
        RECT rcMenuBar;

        GetWindowRect(hWnd, &rc);
        GetWindowRect(g_hWndMenuBar, &rcMenuBar);
        rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);
		
        MoveWindow(hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
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
	short  vkKey;

    static SHACTIVATEINFO s_sai;
	
    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
            switch (wmId)
            {
                case IDM_HELP_ABOUT:
                    DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, About);
                    break;
                case IDM_OK:
                    SendMessage (hWnd, WM_CLOSE, 0, 0);				
                    break;
				case ID_GAME_NEWGAME:
					{
						npmsg=GAME_START;
						KillTimer(hWnd,WM_TIMER);
						BB.BBInitial();
						hdc=GetDC(hWnd);
						BB.DisplayMotherBoard(hdc);
						SetTimer(hWnd,WM_TIMER,BB.getTime(),NULL);
						BB.GenerateMB(BB.CurrentMB);
						BB.GenerateMB(BB.NextMB);
						BB.DisplayNextMB(hdc);
						BB.DisplayCurrentMB(hdc);
						BB.DisplayScore(hdc);
						BB.DisplayLevel(hdc);
						ReleaseDC(hWnd,hdc);
					}
					break;
				case ID_GAME_SAVEGAME:
					if(npmsg == GAME_START)
							{
								HANDLE SAVE=CreateFile(L"np.sav",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
								if(SAVE!=(HANDLE)-1) BB.SAVE(SAVE);
								CloseHandle(SAVE);
							}
					break;
				case ID_GAME_LOADGAME:
					{
						HANDLE LOAD=CreateFile(L"np.sav",GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
						if(LOAD!=(HANDLE)-1) 
						{
							BB.LOAD(LOAD);
							SetTimer(hWnd,WM_TIMER,BB.getTime(),NULL);
							npmsg = GAME_START;
							hdc=GetDC(hWnd);
							BB.DisplayMotherBoard(hdc);
							BB.DisplayBKG(hdc);
							BB.DisplayBB(hdc);
							BB.DisplayCurrentMB(hdc);
							BB.DisplayNextMB(hdc);
							BB.DisplayScore(hdc);
							BB.DisplayLevel(hdc);
							ReleaseDC(hWnd,hdc);
						}
						CloseHandle(LOAD);	
					}
					break;
				case ID_GAME_EXITGAME:
					SendMessage (hWnd, WM_CLOSE, 0, 0);
					break;
				case ID_RECORD_HIGHSCORE:
					break;
 				case ID_RECORD_CLEARRECORD:
					break;
				case ID_HELP_INTRODUCTION:
					break;
				default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
		case WM_KEYDOWN:
			hdc=GetDC(hWnd);
			vkKey = (short)wParam;
			if(npmsg==GAME_START) switch(vkKey)
			{
			case VK_LEFT:
				if(BB.MBMoveLeft())
				{
					gamestate=GAME_LEFT;
					BB.DisplayBlank4MoveLeft(hdc);
					BB.DisplayCurrentMB(hdc);//
				}
				break;
			case VK_RIGHT:
				if(BB.MBMoveRight())
				{
					gamestate=GAME_RIGHT;
					BB.DisplayBlank4MoveRight(hdc);
					BB.DisplayCurrentMB(hdc);//
				}
				break;
			case VK_UP:
				gamestate=GAME_UP;
				BB.MBTransposition();
				break;
			case VK_DOWN:
				gamestate=GAME_FALLDOWN;
				BB.MBFallDown();
				BB.Display4FallDown(hdc);
				BB.DisplayNextMB(hdc);
				BB.DisplayCurrentMB(hdc);
				if(BB.isGameOver()) 
				{
					npmsg=GAME_OVER;
					KillTimer(hWnd,WM_TIMER);
					//Record rec(BB);/*********************************************/
					//rec.Record2File();
					BB.DisplayNextMB(hdc);
					BB.DisplayCurrentMB(hdc);
					BB.DisplayBB(hdc);
					BB.DisplayGameOver(hdc);
				}
				else{
					KillTimer(hWnd,WM_TIMER);
                         //
					if(BB.ReceiveWhat()==SB_BOMB) 
						BB.Find4BOMBErasing(BB.getDeadPosition());//////////////////
					if(BB.ReceiveWhat()==SB_LEVIN) 
						BB.Find4LEVINErasing(BB.getDeadPosition());
					if(BB.ReceiveWhat()==SB_PENTACLE) 
						BB.Find4PENTACLEErasing(BB.getDeadPosition());
						do
						{
							if(BB.Erasing()) 
							{
								BB.DisplayErasing(hdc);
								BB.DisplayScore(hdc);
								BB.DisplayLevel(hdc);
							}
							BB.DisplayBB(hdc);
							BB.Find4NormalErasing();
						}while(BB.isNeed_erase());
				BB.MB_Next2Current();
				BB.GenerateMB(BB.NextMB);
				BB.DisplayBB(hdc);
				BB.DisplayNextMB(hdc);
				BB.DisplayCurrentMB(hdc);
				SetTimer(hWnd,WM_TIMER,BB.getTime(),NULL);
				}
				break;
			default:break;
			}
			ReleaseDC(hWnd,hdc);
			break;
        case WM_CREATE:
            SHMENUBARINFO mbi;

            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize     = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hWnd;
            mbi.nToolBarId = IDR_MENU;
            mbi.hInstRes   = g_hInst;

            if (!SHCreateMenuBar(&mbi)) 
            {
                g_hWndMenuBar = NULL;
            }
            else
            {
                g_hWndMenuBar = mbi.hwndMB;
            }

            // Initialize the shell activate info structure
            memset(&s_sai, 0, sizeof (s_sai));
            s_sai.cbSize = sizeof (s_sai);
            break;
		case WM_TIMER:
			{
				hdc=GetDC(hWnd);
				if(BB.WillMBDead()){
					gamestate=GAME_FALLDOWN;
					BB.MBFallDown();
					BB.Display4FallDown(hdc);
					BB.DisplayNextMB(hdc);
					BB.DisplayCurrentMB(hdc);
					BB.DisplayBB(hdc);
					if(BB.isGameOver()) 
					{
						npmsg=GAME_OVER;
						KillTimer(hWnd,WM_TIMER);
						BB.DisplayNextMB(hdc);
						BB.DisplayCurrentMB(hdc);
						BB.DisplayBB(hdc);
						BB.DisplayGameOver(hdc);
					}
					else{
						KillTimer(hWnd,WM_TIMER);
						     //
						if(BB.ReceiveWhat()==SB_BOMB) 
							BB.Find4BOMBErasing(BB.getDeadPosition());//////////////////
						if(BB.ReceiveWhat()==SB_LEVIN) 
							BB.Find4LEVINErasing(BB.getDeadPosition());
						if(BB.ReceiveWhat()==SB_PENTACLE) 
							BB.Find4PENTACLEErasing(BB.getDeadPosition());
							do
							{
								if(BB.Erasing()) 
								{
									BB.DisplayErasing(hdc);
									BB.DisplayScore(hdc);
									BB.DisplayLevel(hdc);
								}
								BB.DisplayBB(hdc);
								BB.Find4NormalErasing();
							}while(BB.isNeed_erase());
						BB.DisplayNextMB(hdc);
						BB.DisplayCurrentMB(hdc);
						BB.DisplayBB(hdc);
						SetTimer(hWnd,WM_TIMER,BB.getTime(),NULL);
					}
					BB.MB_Next2Current();
					BB.GenerateMB(BB.NextMB);
					BB.DisplayNextMB(hdc);
					BB.DisplayCurrentMB(hdc);
				}
				else{
				BB.DisplayBlank4FallStep(hdc);
				gamestate=GAME_STEPDOWN;
				BB.MBFallStep();
				BB.DisplayCurrentMB(hdc);
				}
				ReleaseDC(hWnd,hdc);
			}
			break;
        case WM_PAINT:
			{
            hdc = BeginPaint(hWnd, &ps);
            switch(npmsg)
			{
			case GAME_INITIAL:
			BB.DisplayCover(hdc);		
			break;
			case GAME_START:
			BB.DisplayMotherBoard(hdc);
			BB.DisplayCurrentMB(hdc);
			BB.DisplayNextMB(hdc);
			switch(gamestate)
			{
			case GAME_STEPDOWN:
					{
						BB.DisplayBKG(hdc);
						BB.DisplayBB(hdc);
						BB.DisplayCurrentMB(hdc);
						BB.DisplayNextMB(hdc);
						BB.DisplayScore(hdc);
						BB.DisplayLevel(hdc);
					}
					break;
			case GAME_UP:
					{
						BB.DisplayCurrentMB(hdc);
						BB.DisplayNextMB(hdc);
						BB.DisplayBB(hdc);
						BB.DisplayBKG(hdc);
						BB.DisplayScore(hdc);
						BB.DisplayLevel(hdc);
					}
					break;	
			case GAME_RIGHT:
					{
						BB.DisplayBlank4MoveRight(hdc);
						BB.DisplayCurrentMB(hdc);
						BB.DisplayNextMB(hdc);
						BB.DisplayBB(hdc);
						BB.DisplayBKG(hdc);
						BB.DisplayScore(hdc);
						BB.DisplayLevel(hdc);
					}
					break;
			case GAME_LEFT:
					{
						BB.DisplayBlank4MoveLeft(hdc);
						BB.DisplayCurrentMB(hdc);
						BB.DisplayNextMB(hdc);
						BB.DisplayBB(hdc);
						BB.DisplayBKG(hdc);
						BB.DisplayScore(hdc);
						BB.DisplayLevel(hdc);
					}
					break;
			case GAME_FALLDOWN:
					{
						BB.Display4FallDown(hdc);
						BB.DisplayCurrentMB(hdc);
						BB.DisplayNextMB(hdc);
						BB.DisplayBB(hdc);
						BB.DisplayBKG(hdc);
						BB.DisplayScore(hdc);
						BB.DisplayLevel(hdc);
					}
					break;
			default:
				break;
			}
			break;
			case GAME_OVER:
					{
						BB.DisplayGameOver(hdc);
						BB.DisplayCurrentMB(hdc);
						BB.DisplayNextMB(hdc);
						BB.DisplayBB(hdc);
						BB.DisplayBKG(hdc);
						BB.DisplayScore(hdc);
						BB.DisplayLevel(hdc);
					}
			default:
				break;
			}
            EndPaint(hWnd, &ps);
			}
            break;
        case WM_DESTROY:
            CommandBar_Destroy(g_hWndMenuBar);
            PostQuitMessage(0);
            break;

        case WM_ACTIVATE:
            // Notify shell of our activate message
            SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
            break;
        case WM_SETTINGCHANGE:
            SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
            {
                // Create a Done button and size it.  
                SHINITDLGINFO shidi;
                shidi.dwMask = SHIDIM_FLAGS;
                shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_EMPTYMENU;
                shidi.hDlg = hDlg;
                SHInitDialog(&shidi);
            }
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hDlg, message);
            return TRUE;

#ifdef _DEVICE_RESOLUTION_AWARE
        case WM_SIZE:
            {
		DRA::RelayoutDialog(
			g_hInst, 
			hDlg, 
			DRA::GetDisplayMode() != DRA::Portrait ? MAKEINTRESOURCE(IDD_ABOUTBOX_WIDE) : MAKEINTRESOURCE(IDD_ABOUTBOX));
            }
            break;
#endif
    }
    return (INT_PTR)FALSE;
}
