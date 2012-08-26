#include <windows.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
#include "resource.h"
#include "block.h"
#define MAX_LOADSTRING 100

HINSTANCE hInst;
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
NP_MSG npmsg=GAME_INITIAL;
BlockBoard BB;

HBITMAP NP_BB;
HBITMAP NP_COVER;
HBITMAP NP_BLANK;
HBITMAP NP_RED;
HBITMAP NP_YELLOW;
HBITMAP NP_BLUE;
HBITMAP NP_GREEN;
HBITMAP NP_PURPLE;
HBITMAP NP_STONE;
HBITMAP NP_BOMB;
HBITMAP NP_LEVIN;
HBITMAP NP_PENTACLE;
HBITMAP NP_ERASING_1;
HBITMAP NP_ERASING_2;
HBITMAP NP_ERASING_3;
HBITMAP NP_ERASING_4;
HBITMAP NP_DEADLINE;
HBITMAP NP_SMBB;
HBITMAP NP_GAMEOVER;
HBITMAP NP_ERASINGTIMES;
HBITMAP NP_GOOD;
HBITMAP NP_BKG;

int WINAPI WinMain( HINSTANCE hInstance,HINSTANCE,LPSTR,int nCmdShow )
{
	NP_BB=(HBITMAP)LoadImage(NULL,L"NP_BB.bmp",IMAGE_BITMAP,345,350,LR_LOADFROMFILE);
	NP_COVER=(HBITMAP)LoadImage(NULL,L"NP_COVER.bmp",IMAGE_BITMAP,345,350,LR_LOADFROMFILE);
	NP_BLANK=(HBITMAP)LoadImage(NULL,L"NP_BLANK.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
	NP_RED=(HBITMAP)LoadImage(NULL,L"NP_RED.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
	NP_YELLOW=(HBITMAP)LoadImage(NULL,L"NP_YELLOW.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
	NP_BLUE=(HBITMAP)LoadImage(NULL,L"NP_BLUE.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
	NP_GREEN=(HBITMAP)LoadImage(NULL,L"NP_GREEN.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
	NP_PURPLE=(HBITMAP)LoadImage(NULL,L"NP_PURPLE.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
	NP_STONE=(HBITMAP)LoadImage(NULL,L"NP_STONE.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
	NP_BOMB=(HBITMAP)LoadImage(NULL,L"NP_BOMB.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
	NP_LEVIN=(HBITMAP)LoadImage(NULL,L"NP_LEVIN.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
	NP_PENTACLE=(HBITMAP)LoadImage(NULL,L"NP_PENTACLE.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
	NP_ERASING_1=(HBITMAP)LoadImage(NULL,L"NP_ERASING_1.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
	NP_ERASING_2=(HBITMAP)LoadImage(NULL,L"NP_ERASING_2.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
	NP_ERASING_3=(HBITMAP)LoadImage(NULL,L"NP_ERASING_3.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
	NP_ERASING_4=(HBITMAP)LoadImage(NULL,L"NP_ERASING_4.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
	NP_DEADLINE=(HBITMAP)LoadImage(NULL,L"NP_DEADLINE.bmp",IMAGE_BITMAP,180,25,LR_LOADFROMFILE);
	NP_SMBB=(HBITMAP)LoadImage(NULL,L"NP_SMBB.bmp",IMAGE_BITMAP,90,90,LR_LOADFROMFILE);
	NP_GAMEOVER=(HBITMAP)LoadImage(NULL,L"NP_GAMEOVER.bmp",IMAGE_BITMAP,110,70,LR_LOADFROMFILE);
	NP_ERASINGTIMES=(HBITMAP)LoadImage(NULL,L"NP_ERASINGTIMES.bmp",IMAGE_BITMAP,50,50,LR_LOADFROMFILE);
	NP_GOOD=(HBITMAP)LoadImage(NULL,L"NP_GOOD.bmp",IMAGE_BITMAP,45,25,LR_LOADFROMFILE);
	NP_BKG=(HBITMAP)LoadImage(NULL,L"NP_BKG.bmp",IMAGE_BITMAP,345,350,LR_LOADFROMFILE);

	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}
	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	ZeroMemory(&wcex,sizeof(WNDCLASSEX));
	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style			= 0;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= (HICON)LoadImage(NULL,L"icon_np.ico",IMAGE_ICON,90,90,LR_LOADFROMFILE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= ::CreateSolidBrush(RGB(255,255,255));//(HBRUSH) GetStockObject (WHITE_BRUSH);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU);
	wcex.lpszClassName	= L"NaturePark";
	wcex.hIconSm		= LoadIcon(wcex.hInstance,MAKEINTRESOURCE(IDI_ICON_NPSM) );

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; 

	hWnd = CreateWindow(L"NaturePark", L"NaturePark", WS_OVERLAPPEDWINDOW,
		300, 100, 361/**/, 408/**/, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}
////////////£¯£¯£¯£¯£¯£¯£¯£¯£¯£¯£¯£¯£¯£¯£¯£¯£¯
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
//////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		switch (wmId)
		{
		case ID_NATUREPARK_NEWGAME:
			{
				npmsg=GAME_START;
				KillTimer(hWnd,WM_TIMER);
				hdc=GetDC(hWnd);
				DrawState(hdc,NULL,NULL,(LPARAM)NP_BB,0,0,0,345,350,DST_BITMAP);
				ReleaseDC(hWnd,hdc);
				BB.BBInitial();
				SetTimer(hWnd,WM_TIMER,BB.GetSpeed(),NULL);
				BB.GenerateMB(BB.CurrentMB);
				BB.GenerateMB(BB.NextMB);
			}
			break;
		case ID_NATUREPARK_SAVEGAME:
			{
				if(npmsg > GAME_INITIAL && npmsg < GAME_OVER)
				{
					HANDLE SAVE=CreateFile(L"np.sav",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
					if(SAVE!=(HANDLE)-1) BB.SAVE(SAVE);
					CloseHandle(SAVE);
				}
			}
			break;
		case ID_NATUREPARK_LOADGAME:
			{
				HANDLE LOAD=CreateFile(L"np.sav",GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
				if(LOAD!=(HANDLE)-1) BB.LOAD(LOAD);
				CloseHandle(LOAD);
				SetTimer(hWnd,WM_TIMER,BB.GetSpeed(),NULL);	
			}
			break;
		case ID_NATUREPARK_EXIT:
			::DestroyWindow(hWnd);
			break;
		case ID_SET_SCORERANK:
			{
				KillTimer(hWnd,WM_TIMER);
				npmsg=LOOK_REC;
				hdc=GetDC(hWnd);
				DrawState(hdc,NULL,NULL,(LPARAM)NP_BKG,0,0,0,345,350,DST_BITMAP);
				BB.Read_Dispaly_Record(hdc);
				ReleaseDC(hWnd,hdc);
			}
			break;
		case ID_SET_DELRANK:
			{
				KillTimer(hWnd,WM_TIMER);
				BB.DelRecord();
				npmsg=DEl_REC;
				hdc=GetDC(hWnd);
				DrawState(hdc,NULL,NULL,(LPARAM)NP_BKG,0,0,0,345,350,DST_BITMAP);
				BB.Read_Dispaly_Record(hdc);
				ReleaseDC(hWnd,hdc);
			}
			break;
		case ID_HELP_INTRO:
			{						
				KillTimer(hWnd,WM_TIMER);
				npmsg=LOOK_INTRO;
				hdc=GetDC(hWnd);
				DrawState(hdc,NULL,NULL,(LPARAM)NP_BKG,0,0,0,345,350,DST_BITMAP);
				BB.DisplayIntro(hdc);
				ReleaseDC(hWnd,hdc);
			}
			break;
		case ID_HELP_HELP:
			{						
				KillTimer(hWnd,WM_TIMER);
				npmsg=LOOK_HELP;
				hdc=GetDC(hWnd);
				DrawState(hdc,NULL,NULL,(LPARAM)NP_BKG,0,0,0,345,350,DST_BITMAP);
				BB.DisplayHelp(hdc);
				ReleaseDC(hWnd,hdc);
			}
			break;
		case ID_HELP_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		break;
	case WM_RBUTTONDOWN: 
		break; 
	case WM_LBUTTONDOWN:
		break; 
	case WM_MBUTTONDOWN:	
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_UP:
			npmsg=GAME_UP;
			BB.MBTransposition();
			break;
		case VK_DOWN:
			hdc=GetDC(hWnd);
			BB.DisplayNextMB(hdc);
			BB.DisplayCurrentMB(hdc);
			BB.DisplayBB(hdc);
			npmsg=GAME_FALLDOWN;
			BB.MBFallDown();
			if(BB.IsGameOver()) 
			{
				npmsg=GAME_OVER;
				KillTimer(hWnd,WM_TIMER);
				Record rec(BB);/*********************************************/
				rec.Record2File();
				BB.DisplayNextMB(hdc);
				BB.DisplayCurrentMB(hdc);
				BB.DisplayBB(hdc);
				BB.DisplayGameOver(hdc);
			}
			else{
				KillTimer(hWnd,WM_TIMER);
				//
				if(BB.ReceiveWhat(0)==SB_BOMB) 
					BB.Find4BOMBErasing(BB.GetDeadPosition());//////////////////
				if(BB.ReceiveWhat(0)==SB_LEVIN) 
					BB.Find4LEVINErasing(BB.GetDeadPosition());
				if(BB.ReceiveWhat(0)==SB_PENTACLE) 
					BB.Find4PENTACLEErasing(BB.GetDeadPosition());
				do
				{
					BB.DisplayBB(hdc);
					BB.DisplayNextMB(hdc);
					BB.DisplayCurrentMB(hdc);
					BB.DisplayErasing(hdc);
					BB.Erasing();
					BB.DisplayBB(hdc);
					BB.DisplayNextMB(hdc);
					BB.DisplayCurrentMB(hdc);
					BB.Find4NormalErasing();
				}while(BB.NeedErasing());

				//
				BB.MB_Next2Current();
				BB.GenerateMB(BB.NextMB);
				BB.DisplayBB(hdc);
				BB.DisplayNextMB(hdc);
				BB.DisplayCurrentMB(hdc);
				SetTimer(hWnd,WM_TIMER,BB.GetSpeed(),NULL);
			}
			ReleaseDC(hWnd,hdc);
			break;
		case VK_RIGHT:
			npmsg=GAME_RIGHT;
			BB.MBMoveRight();
			break;
		case VK_LEFT:
			npmsg=GAME_LEFT;
			BB.MBMoveLeft();
			break;
		case VK_ESCAPE:
			::DestroyWindow(hWnd);
			break;
		default:
			break;
		}
		break;
	case WM_TIMER:
		{

			hdc=GetDC(hWnd);
			DrawState(hdc,NULL,NULL,(LPARAM)NP_BB,0,0,0,345,350,DST_BITMAP);
			if(BB.WillMBDead()) 
			{
				BB.DisplayNextMB(hdc);
				BB.DisplayCurrentMB(hdc);
				BB.DisplayBB(hdc);
				npmsg=GAME_FALLDOWN;
				BB.MBFallDown();
				if(BB.IsGameOver()) 
				{
					npmsg=GAME_OVER;
					KillTimer(hWnd,WM_TIMER);
					Record rec(BB);/*************************************/
					rec.Record2File();
					BB.DisplayNextMB(hdc);
					BB.DisplayCurrentMB(hdc);
					BB.DisplayBB(hdc);
					BB.DisplayGameOver(hdc);
				}
				else
				{
					KillTimer(hWnd,WM_TIMER);

					//
					if(BB.ReceiveWhat(0)==SB_BOMB) 
						BB.Find4BOMBErasing(BB.GetDeadPosition());//////////////////
					if(BB.ReceiveWhat(0)==SB_LEVIN) 
						BB.Find4LEVINErasing(BB.GetDeadPosition());
					if(BB.ReceiveWhat(0)==SB_PENTACLE) 
						BB.Find4PENTACLEErasing(BB.GetDeadPosition());
					do
					{
						BB.DisplayBB(hdc);
						BB.DisplayNextMB(hdc);
						BB.DisplayCurrentMB(hdc);
						BB.DisplayErasing(hdc);
						BB.Erasing();
						BB.DisplayBB(hdc);
						BB.DisplayNextMB(hdc);
						BB.DisplayCurrentMB(hdc);
						BB.Find4NormalErasing();
					}while(BB.NeedErasing());

					//
					BB.MB_Next2Current();
					BB.GenerateMB(BB.NextMB);
					BB.DisplayBB(hdc);
					BB.DisplayNextMB(hdc);
					BB.DisplayCurrentMB(hdc);
					SetTimer(hWnd,WM_TIMER,BB.GetSpeed(),NULL);
				}
			}
			else
			{
				npmsg=GAME_STEPDOWN;
				BB.MBFallStep();
				BB.DisplayBB(hdc);
				BB.DisplayNextMB(hdc);
				BB.DisplayCurrentMB(hdc);
			}
			ReleaseDC(hWnd,hdc);

		}
		break;
	case WM_PAINT:
		{
			hdc=::BeginPaint(hWnd,&ps);
			switch(npmsg)
			{
			case GAME_INITIAL:
				{
					DrawState(hdc,NULL,NULL,(LPARAM)NP_COVER,0,0,0,345,350,DST_BITMAP);
				}
				break;
			case GAME_START:
				{
					DrawState(hdc,NULL,NULL,(LPARAM)NP_BB,0,0,0,345,350,DST_BITMAP);
				}
				break;
			case GAME_STEPDOWN:case GAME_UP:case GAME_RIGHT:case GAME_LEFT:case GAME_FALLDOWN:
				{
					BB.DisplayBB(hdc);
					BB.DisplayNextMB(hdc);
					BB.DisplayCurrentMB(hdc);
				}
				break;
			case GAME_OVER:
				{
					DrawState(hdc,NULL,NULL,(LPARAM)NP_BB,0,0,0,345,350,DST_BITMAP);
					BB.DisplayBB(hdc);
					BB.DisplayNextMB(hdc);
					BB.DisplayCurrentMB(hdc);
					BB.DisplayGameOver(hdc);
				}
			case LOOK_REC:case DEl_REC:
				DrawState(hdc,NULL,NULL,(LPARAM)NP_BKG,0,0,0,345,350,DST_BITMAP);
				BB.Read_Dispaly_Record(hdc);
				break;
			case LOOK_INTRO:
				DrawState(hdc,NULL,NULL,(LPARAM)NP_BKG,0,0,0,345,350,DST_BITMAP);
				BB.DisplayIntro(hdc);
				break;
			case LOOK_HELP:
				DrawState(hdc,NULL,NULL,(LPARAM)NP_BKG,0,0,0,345,350,DST_BITMAP);
				BB.DisplayHelp(hdc);
				break;
			default:
				break;
			}
			::EndPaint(hWnd,&ps);
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

