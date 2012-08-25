// CBTdll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "MsgHook.h"
#include "dbg.h"


HMODULE hInst=NULL;

//Function defines
BOOL GetCurrentProcessName(LPTSTR lpstrName);

BOOL APIENTRY DllMain( HANDLE hModule, 
		      DWORD  ul_reason_for_call, 
		      LPVOID lpReserved
		      )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ShowUp("dll load!");
		hInst=(HMODULE)hModule;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		ShowUp("dll unload!");
		break;
	}
	return TRUE;
}

LRESULT WINAPI CallBackMouseMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (HC_ACTION == code || HC_NOREMOVE == code)
	{
		if (WM_MOUSEMOVE == wParam)	//if is mouse move we skip this message!
		{
			return(CallNextHookEx(NULL,code,wParam,lParam));	
		}
		//check if the message is from MouseKeyHook.exe
		TCHAR lpStrName[1024]=TEXT("");
		GetCurrentProcessName(lpStrName);
		//ShowUp(lpStrName);
		if (lstrcmpi(lpStrName,TEXT("MouseKeyHook.exe")) == 0)	//main module,not hook!
		{
			return(CallNextHookEx(NULL,code,wParam,lParam));
		}
	}
	return TRUE;	//prevent message to continue loop!! must return TRUE 
}

LRESULT WINAPI CallBackKeyMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (HC_ACTION == code || HC_NOREMOVE == code)
	{
		//check if the message is from MouseKeyHook.exe
		TCHAR lpStrName[1024]=TEXT("");
		GetCurrentProcessName(lpStrName);
		//ShowUp(lpStrName);
		if (lstrcmpi(lpStrName,TEXT("MouseKeyHook.exe")) == 0)	//main module,not hook!
		{
			return(CallNextHookEx(NULL,code,wParam,lParam));
		}
	}
	return TRUE;	//prevent message to continue loop!!
}


//get current process name(only file name!!)!
BOOL GetCurrentProcessName(LPTSTR lpstrName)
{
	int i=0;
	TCHAR lpstrFullPath[1024]=TEXT("");
	GetModuleFileName(GetModuleHandle(NULL),lpstrFullPath,1024);
	for(i=lstrlen(lpstrFullPath);i>0;i--)
	{
		if (lpstrFullPath[i] == '\\')
		{
			break;
		}
	}
	lstrcpy(lpstrName,lpstrFullPath+i+1);
	return TRUE;
}
////////////////////////////////end of file////////////////