// MyDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

void _declspec(dllexport) JustSoSo() 
{ 
MessageBox(NULL,L"It's so easy!",L"Hahaha......",MB_OK); 
} 

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
   switch( ul_reason_for_call ) { 
	case DLL_PROCESS_ATTACH:
		JustSoSo();
		break;
	case DLL_THREAD_ATTACH: 
		JustSoSo();
		break;
	case DLL_THREAD_DETACH: 
		JustSoSo();
		break;
	case DLL_PROCESS_DETACH:
		JustSoSo();
		break;
} 
return TRUE; 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

