// RecordCommand.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#if _DEBUG
#define dbgprintf printf
#define dbgtprintf _tprintf
#else
#define dbgprintf
#define dbgtprintf
#endif

typedef struct _PROCESS_WAITING_THREAD_CONTEXT
{
	HANDLE hWritePipe;
	HANDLE hReadPipe;
	LPPROCESS_INFORMATION ppi;
}PROCESS_WAITING_THREAD_CONTEXT, * LPPROCESS_WAITING_THREAD_CONTEXT;

DWORD WINAPI ProcessWaitingThread( LPVOID lpThreadParameter);

TCHAR * BASE_CMD = TEXT("CMD.EXE /C");
const ULONG CMD_BUF_SIZE = 1024;
const ULONG CMD_PARAMETER_LENGTH_LIMIT = 256;

int _tmain(int argc, _TCHAR* argv[])
{
	dbgtprintf(_T("%d parameter(s)\n"), argc - 1 );

	TCHAR command[CMD_BUF_SIZE] = {0};

	for(int i = 0; i< argc; i++)
	{
		dbgtprintf(_T("%s\n"),argv[i]);
	}
	
	if( argc == 1 )
	{
		_tprintf(_T("Usage:\n"));
		_tprintf(_T("\t%s <command>\n"), argv[0]);
		return 0;
	}
	else if(argc >= 2)
	{
		// Append commands
		ULONG position = 0;
		for(int i = 0; i < argc; i++)
		{
			if( 0 == i )
			{
				_stprintf_s(command + position, CMD_BUF_SIZE-position-1, _T("%s "), BASE_CMD);
				position += _tcsnlen(BASE_CMD, CMD_PARAMETER_LENGTH_LIMIT) + 1;
			}
			else
			{
				_stprintf_s(command + position, CMD_BUF_SIZE-position-1, _T("%s "), argv[i]);
				position += _tcsnlen(argv[i], CMD_PARAMETER_LENGTH_LIMIT) + 1;
			}
		}
		dbgtprintf(_T("%s\n"),command);

	}
	else
	{
		_tprintf(_T("Invalid parameter(s)!\n"));
		return -1;
	}

	SECURITY_ATTRIBUTES sa={sizeof ( sa ),NULL,TRUE};  

	HANDLE hWritePipe = INVALID_HANDLE_VALUE;
	HANDLE hReadPipe = INVALID_HANDLE_VALUE;

	BOOL bResult = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);

	if ( !bResult || hWritePipe == INVALID_HANDLE_VALUE || hReadPipe == INVALID_HANDLE_VALUE) 
	{
		_tprintf(TEXT("CreatePipe failed, GLE=%d.\n"), GetLastError()); 
		return -1;
	}
	else
	{
		dbgtprintf(TEXT("CreatePipe is successful.\n")); 
	}

	// CreateProcess
	dbgtprintf(TEXT("Try to CreateProcess.\n")); 


	STARTUPINFO si={0};

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags =STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;  //使用标准柄和显示窗口  
	si.hStdOutput = hWritePipe;//将文件作为标准输出句柄  
	si.hStdError = hWritePipe;//将文件作为标准输出句柄  
	si.wShowWindow = SW_HIDE;//隐藏控制台窗口  

	PROCESS_INFORMATION pi={0};  
	
	PROCESS_WAITING_THREAD_CONTEXT pwi = {0};

	pwi.hReadPipe = hReadPipe;
	pwi.hWritePipe = hWritePipe;

	if ( CreateProcess ( NULL, command, &sa, &sa, TRUE, NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi) )  
	{  
		pwi.ppi = &pi;
		dbgtprintf(TEXT("OK to CreateProcess.\n")); 

		HANDLE hThread = CreateThread(&sa, 0, ProcessWaitingThread, &pwi, 0, NULL );

		if( 0 != hThread )
		{
			dbgtprintf(TEXT("OK to CreateThread.\n")); 

			SYSTEMTIME st;
			GetSystemTime(&st);

			TCHAR logFileName[MAX_PATH] = {0};

			_stprintf_s(logFileName, MAX_PATH - 1, _T("RC_%04d%02d%02d-%d-%02d%02d%02d-%03d.log"),
				st.wYear,st.wMonth,st.wDay,st.wDayOfWeek,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
			
			dbgtprintf(logFileName); 
			dbgtprintf(L"\n"); 

			HANDLE hLogFile = CreateFile(logFileName,
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ,
				&sa,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if(INVALID_HANDLE_VALUE == hLogFile)
			{
				_tprintf(_T("Failed to CreateFile, st = 0x%x\n"), GetLastError());
				goto ErrorExit;
			}
			else
			{
				dbgtprintf(TEXT("OK to CreateFile.\n")); 
			}

			BOOL bReadable = TRUE;
			BOOL bWrite = FALSE;

			while (bReadable)
			{
				CHAR strCmdFeedbackBuffer[1024] = {0};
				DWORD dwRead = 0;
				DWORD dwToWrite = 0;
				DWORD dwWritten = 0;

				bReadable = ReadFile( hReadPipe, 
									  strCmdFeedbackBuffer, 
									  1024-1, 
									  &dwRead, 
									  NULL);

				printf(strCmdFeedbackBuffer);
				
				dwToWrite = dwRead;

				bWrite = WriteFile( hLogFile, 
									strCmdFeedbackBuffer, 
									dwToWrite, 
									&dwWritten, 
									NULL);

				if(!bWrite)
				{
					_tprintf(_T("Write to log file error, st=0x%x\n"), GetLastError());
					assert(false);
				}

				assert(dwToWrite == dwWritten);
			}


			CloseHandle(hLogFile);
		}
		else
		{
			_tprintf(_T("Failed to CreateThread, st = 0x%x\n"), GetLastError());
			goto ErrorExit;
		}
	}
	else
	{
		_tprintf(_T("Failed to CreateProcess, st = 0x%x\n"), GetLastError());
		goto ErrorExit;
	}

	// waitThread

	//

	goto OkExit;

ErrorExit:
	if (!CloseHandle(hWritePipe))
	{
		_tprintf(_T("Close hWritePipe False, st = 0x%x\n"), GetLastError());
	}

	if (!CloseHandle(hReadPipe))
	{
		_tprintf(_T("Close hWritePipe False, st = 0x%x\n"), GetLastError());
	}
	return -1;

OkExit:
	return 0;
}


DWORD WINAPI ProcessWaitingThread( LPVOID context)
{

	assert(context);

	LPPROCESS_WAITING_THREAD_CONTEXT ppwtc = static_cast<LPPROCESS_WAITING_THREAD_CONTEXT>(context);

	assert(ppwtc->ppi);

	WaitForSingleObject ( ppwtc->ppi->hProcess , INFINITE ); 

	if (!CloseHandle ( ppwtc->ppi->hProcess ))
	{
		_tprintf(_T("Close ppwtc->ppi->hProcess False\n"));

	}  

	if (!CloseHandle ( ppwtc->ppi->hThread ))
	{
		_tprintf(_T("Close ppwtc->ppi->hThread False\n"));
	}  

	if (!CloseHandle(ppwtc->hWritePipe))
	{
		_tprintf(_T("Close hWritePipe False, st = 0x%x\n"), GetLastError());
	}

	if (!CloseHandle(ppwtc->hReadPipe))
	{
		_tprintf(_T("Close hWritePipe False, st = 0x%x\n"), GetLastError());
	}

	return 0;
}