// loop.cpp : Defines the entry point for the console application.
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

BOOL ParameterCheck(int argc, 
	_TCHAR* argv[], 
	UINT & loop_times, 
	DWORD & ms_between_loop,
	BOOL & enable_logs,
	TCHAR * p_command_buf, 
	ULONG command_buf_size);

int ExecuteCommand(
	TCHAR * p_command_buf, 
	ULONG command_buf_size,
	BOOL enable_logs,
	HANDLE hLogFile);

VOID PrintUsage(TCHAR *);
BOOL IsPureDigital(TCHAR * str, INT maxLength);
BOOL IsPureSpace(TCHAR * str, INT maxLength);


TCHAR * BASE_CMD = TEXT("CMD.EXE /C");
TCHAR * OPTION_CMD = TEXT("-cmd");
TCHAR * OPTION_C = TEXT("-c");
TCHAR * OPTION_TIMES = TEXT("-times");
TCHAR * OPTION_T = TEXT("-t");
TCHAR * OPTION_SLEEP = TEXT("-sleep");
TCHAR * OPTION_S = TEXT("-s");
TCHAR * OPTION_LOG = TEXT("-log");
TCHAR * OPTION_L = TEXT("-l");

const ULONG CMD_OPTION_LENGTH = 1024;

const ULONG CMD_BUF_SIZE = 1024;
const ULONG CMD_FEEDBACK_BUF_SIZE = 1024;
const ULONG CMD_PARAMETER_LENGTH_LIMIT = 1024;

const DWORD SLEEP_MIN = 1;
const DWORD SLEEP_MAX = 180;

const DWORD TIMES_MIN = 1;
const DWORD TIMES_MAX = 1000;


int _tmain(int argc, _TCHAR* argv[])
{
	UINT loop_times = 0;
	BOOL loop_endless = FALSE;
	DWORD ms_between_loop = 0;
	BOOL enable_logs = FALSE;
	TCHAR command[CMD_BUF_SIZE] = {0}; 

	if ( ParameterCheck(argc, argv, loop_times, ms_between_loop, enable_logs, command, CMD_BUF_SIZE) )
	{
		loop_endless = ( 0 == loop_times ); 
	}
	else
	{
		PrintUsage(argv[0]);
		return 1;
	}

	HANDLE hLogFile = INVALID_HANDLE_VALUE;

	if( TRUE == enable_logs)
	{
		SYSTEMTIME st;
		GetSystemTime(&st);

		TCHAR logFileName[MAX_PATH] = {0};

		_stprintf_s(logFileName, MAX_PATH - 1, _T("loop_trace_%04d%02d%02d-%d-%02d%02d%02d-%03d.log"),
			st.wYear,st.wMonth,st.wDay,st.wDayOfWeek,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);

		dbgtprintf(logFileName); 
		dbgtprintf(L"\n"); 


		hLogFile = CreateFile(logFileName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if(INVALID_HANDLE_VALUE == hLogFile)
		{
			_tprintf(_T("Failed to Create Log File, st = 0x%x\n"), GetLastError());
			return 1;
		}
		else
		{
			dbgtprintf(TEXT("OK to CreateFile.\n")); 
		}
	}

	while( loop_endless || loop_times > 0 )
	{
		if(!loop_endless)
		{
			--loop_times;
		}

		CHAR printedCommand[CMD_BUF_SIZE] = {0}; 
		
		sprintf_s(printedCommand, CMD_BUF_SIZE - 1, "\n-> %S\n\n",
			command + _tcsnlen(BASE_CMD, CMD_BUF_SIZE) + 1);

		printf(printedCommand);

		if (INVALID_HANDLE_VALUE != hLogFile)
		{
			DWORD dwToWrite = sizeof(CHAR) * strnlen(printedCommand, CMD_BUF_SIZE);
			DWORD dwWritten = 0;
			BOOL bWrite = WriteFile( hLogFile, 
						printedCommand, 
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

		ExecuteCommand(command, CMD_BUF_SIZE, enable_logs, hLogFile);

		
		Sleep(ms_between_loop);
	}

	if (INVALID_HANDLE_VALUE != hLogFile)
	{
		CloseHandle(hLogFile);
	}

	return 0;
}

BOOL ParameterCheck(int argc, 
	_TCHAR* argv[], 
	UINT & loop_times, 
	DWORD & ms_between_loop,
	BOOL & enable_logs,
	TCHAR * p_command_buf, 
	ULONG command_buf_size)
	// loop -cmd "dir *.exe" -times 5 -sleep 2 -log
	// loop -c "dir *.exe" -t 5 -s 2 -l
	// loop dir *.exe
{
	assert(argv);
	assert(p_command_buf);

	BOOL result = FALSE;

	loop_times = 0;
	ms_between_loop = 1000;
	enable_logs = FALSE;

	dbgtprintf(_T("%d parameter(s)\n"), argc - 1 );

	for(int i = 0; i< argc; i++)
	{
		dbgtprintf(_T("%s\n"),argv[i]);
	}

	if ( 1 == argc )
	{
		// Do nothing here, To Invoke PrintUsage(argv[0]);
	}
	else
	{
		int len = _tcsnlen(argv[1],CMD_OPTION_LENGTH);

		dbgtprintf(_T("len of argv[1] = %d\n"), len);

		if ( 0 < len )
		{
			_tcslwr_s(argv[1], len+1); //_tcsdup
		}
		else
		{
			// do nothing
		}


		if ( 0 == _tcsncmp(OPTION_CMD,argv[1], _tcslen(OPTION_CMD)) || 0 == _tcsncmp(OPTION_C,argv[1],_tcslen(OPTION_C)) || 
			0 == _tcsncmp(OPTION_TIMES,argv[1], _tcslen(OPTION_TIMES)) || 0 == _tcsncmp(OPTION_T,argv[1],_tcslen(OPTION_T)) || 
			0 == _tcsncmp(OPTION_SLEEP,argv[1], _tcslen(OPTION_SLEEP)) || 0 == _tcsncmp(OPTION_S,argv[1],_tcslen(OPTION_S)) ||
			0 == _tcsncmp(OPTION_LOG,argv[1], _tcslen(OPTION_LOG)) || 0 == _tcsncmp(OPTION_L,argv[1],_tcslen(OPTION_L)) )
		{
			dbgtprintf(_T("Parameters Mode.\n"));

			int iExpectedParameters = 0;
			BOOL bGetCmdFromOpt = FALSE;
			BOOL bGetTimesFromOpt = FALSE;
			BOOL bGetSleepFromOpt = FALSE;
			BOOL bGetLogFromOpt = FALSE;

			result = TRUE;

			for( int i = 1; i < argc; i++ )
			{
				int len = _tcsnlen(argv[i],CMD_OPTION_LENGTH);

				dbgtprintf(_T("len of argv[%d] = %d"), i, len);

				if ( 0 < len )
				{
					_tcslwr_s(argv[i], len+1); //_tcsdup
				}
				else
				{
					// do nothing
				}

				if ( !bGetCmdFromOpt && 
					( 0 == _tcsncmp(OPTION_CMD,argv[i], _tcslen(OPTION_CMD)) || 
					0 == _tcsncmp(OPTION_C, argv[i], _tcslen(OPTION_C)) )
					)
				{
					dbgtprintf(_T("Get Command From Option\n"));

					if ( i + 1 < argc && !IsPureSpace(argv[i+1], CMD_PARAMETER_LENGTH_LIMIT) && _tcsnlen(argv[i+1], CMD_PARAMETER_LENGTH_LIMIT) > 0 )
					{
						dbgtprintf(_T("Command Length, %d\n"), _tcsnlen(argv[i+1], CMD_PARAMETER_LENGTH_LIMIT));

						bGetCmdFromOpt = TRUE;

						iExpectedParameters += 2;

						ULONG position = 0;

						_stprintf_s(p_command_buf, command_buf_size-position-1, _T("%s "), BASE_CMD);

						position += _tcsnlen(BASE_CMD, CMD_PARAMETER_LENGTH_LIMIT) + 1;

						_stprintf_s(p_command_buf + position, command_buf_size-position-1, _T("%s"), argv[i+1]);

						dbgtprintf(_T("Command: %s\n"),p_command_buf);
					}
					else
					{
						bGetCmdFromOpt = FALSE;

						result = FALSE;

						_tprintf(_T("Invalid \"command\" Parameter!\n") );

					}

				}
				else if ( !bGetTimesFromOpt &&
					( 0 == _tcsncmp(OPTION_TIMES,argv[i], _tcslen(OPTION_TIMES)) ||
					0 == _tcsncmp(OPTION_T, argv[i], _tcslen(OPTION_T)) )
					)
				{
					dbgtprintf(_T("Get Times From Option\n"));

					if ( i + 1 < argc && IsPureDigital(argv[i+1], CMD_PARAMETER_LENGTH_LIMIT) )
					{
						bGetTimesFromOpt = TRUE;

						iExpectedParameters += 2;

						// to-do
						loop_times = _tcstoul(argv[i+1], NULL, 10 );

						if ( loop_times >= TIMES_MIN && loop_times <= TIMES_MAX )
						{

							dbgtprintf(_T("Times: %d\n"), loop_times);
						}
						else
						{
							result = FALSE;

							_tprintf(_T("Invalid Range of \"times\" Parameter!\n"));
						}

					}
					else
					{
						bGetTimesFromOpt = FALSE;

						result = FALSE;

						_tprintf(_T("Invalid \"times\" Parameter!\n"));
					}


				}
				else if ( !bGetSleepFromOpt &&
					( 0 == _tcsncmp(OPTION_SLEEP, argv[i], _tcslen(OPTION_SLEEP)) ||
					0 == _tcsncmp(OPTION_S, argv[i], _tcslen(OPTION_S)) )
					)
				{
					dbgtprintf(_T("Get Sleep From Option\n"));

					if ( i + 1 < argc && IsPureDigital(argv[i+1], CMD_PARAMETER_LENGTH_LIMIT) )
					{
						bGetSleepFromOpt = TRUE;

						iExpectedParameters += 2;

						// to-do
						DWORD s_between_loop = _tcstoul(argv[i+1], NULL, 10 );

						if (s_between_loop >= SLEEP_MIN && s_between_loop <= SLEEP_MAX)
						{
							ms_between_loop = s_between_loop * 1000;
							dbgtprintf(_T("Sleep: %d\n"),ms_between_loop);
						}
						else
						{
							result = FALSE;

							_tprintf(_T("Invalid Range of \"sleep\" Parameter!\n"));

						}

					}
					else
					{
						bGetSleepFromOpt = FALSE;

						result = FALSE;

						_tprintf(_T("Invalid \"sleep\" Parameter!\n"));
					}
				}
				else if ( !bGetLogFromOpt && 
					( 0 == _tcsncmp(OPTION_LOG, argv[i], _tcslen(OPTION_LOG)) ||
					0 == _tcsncmp(OPTION_L, argv[i], _tcslen(OPTION_L)) )
					)
				{
					dbgtprintf(_T("Get Log From Option\n"));

					bGetLogFromOpt = TRUE;

					enable_logs = TRUE;

					iExpectedParameters++;
				}
				else
				{
					dbgtprintf(_T("Other parameters from command line: %s\n"), argv[i]);
				}

				if ( FALSE == result )
				{
					break;
				}

			}

			if ( result == TRUE && FALSE == bGetCmdFromOpt )
			{
				result = FALSE;

				_tprintf(_T("The \"cmd\" Parameter is Mandantory!\n") );

			}

			if ( result == TRUE && iExpectedParameters != argc - 1)
			{
				result = FALSE;

				dbgtprintf(_T("iExpectedParameters = %d, argc - 1 = %d\n"), iExpectedParameters, argc - 1);
				_tprintf(_T("Too many parameters!\n"), iExpectedParameters, argc - 1);

			}



		}
		else
		{
			result = TRUE;

			dbgtprintf(_T("Always Loop Mode.\n"));
			dbgtprintf(_T("%p, %p\n"), p_command_buf, p_command_buf+1);

			ULONG position = 0;

			for(int i = 0; i < argc; i++)
			{
				if( 0 == i )
				{
					dbgtprintf(_T("s,%d\n"),i);
					_stprintf_s(p_command_buf + position, command_buf_size-position-1, _T("%s "), BASE_CMD);
					position += _tcsnlen(BASE_CMD, CMD_PARAMETER_LENGTH_LIMIT) + 1;
					dbgtprintf(_T("e,%d\n"),i);
				}
				else
				{
					dbgtprintf(_T("s,%d\n"),i);
					dbgtprintf(_T("%s\n"),argv[i]);
					_stprintf_s(p_command_buf + position, command_buf_size-position-1, _T("%s "), argv[i]);
					position += _tcsnlen(argv[i], CMD_PARAMETER_LENGTH_LIMIT) + 1;
					dbgtprintf(_T("e,%d\n"),i);
				}
			}

			dbgtprintf(_T("Command: %s\n"),p_command_buf);

		}
	}

	return result;
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

VOID PrintUsage(TCHAR * loop)
{
	_tprintf(_T("Usage:\n"));
	_tprintf(_T("\t%s -cmd \"[command]\" <-times [number of loop times]> <-sleep [seconds]> <-log>\n"), loop);
	_tprintf(_T("\t%s -c \"[command]\" <-t [number of loop times]> <-s [seconds]> <-l>\n"), loop);
	_tprintf(_T("\t%s [command]\n"), loop);

	_tprintf(_T("\n\tThe valid number of loop times is between %u and %u\n"),  TIMES_MIN, TIMES_MAX);
	_tprintf(_T("\tThe valid value of sleep is between %u second and %u seconds\n"),SLEEP_MIN, SLEEP_MAX);
	_tprintf(_T("\tSwitch \"-l\" or \"-log\" is used for enabling the log\n"),SLEEP_MIN, SLEEP_MAX);
}

BOOL IsPureDigital(TCHAR * str, INT maxLength)
{
	BOOL Result= TRUE;

	int strLength = _tcsnlen(str, maxLength);

	for (int i = 0; i < strLength; ++i )
	{
		if( str[i] < L'0' ||  str[i] > L'9')
		{
			Result = FALSE;

			break;
		}
	}

	return Result;
}

BOOL IsPureSpace(TCHAR * str, INT maxLength)
{
	BOOL Result= TRUE;

	int strLength = _tcsnlen(str, maxLength);

	for (int i = 0; i < strLength; ++i )
	{
		if( str[i] != L' ')
		{
			Result = FALSE;

			break;
		}
	}

	return Result;
}

int ExecuteCommand(
	TCHAR * p_command_buf, 
	ULONG command_buf_size,
	BOOL enable_logs,
	HANDLE hLogFile)
{
	assert(p_command_buf);

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

	if ( CreateProcess ( NULL, p_command_buf, &sa, &sa, TRUE, NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi) )  
	{  
		pwi.ppi = &pi;
		dbgtprintf(TEXT("OK to CreateProcess.\n")); 

		HANDLE hThread = CreateThread(&sa, 0, ProcessWaitingThread, &pwi, 0, NULL );

		if( 0 != hThread )
		{
			dbgtprintf(TEXT("OK to CreateThread.\n")); 

			BOOL bReadable = TRUE;
			BOOL bWrite = FALSE;

			while (bReadable)
			{
				CHAR strCmdFeedbackBuffer[CMD_FEEDBACK_BUF_SIZE] = {0};
				DWORD dwRead = 0;

				bReadable = ReadFile( hReadPipe, 
					strCmdFeedbackBuffer, 
					CMD_FEEDBACK_BUF_SIZE-1, 
					&dwRead, 
					NULL);

				printf(strCmdFeedbackBuffer);


				if (INVALID_HANDLE_VALUE != hLogFile)
				{
					DWORD dwToWrite = 0;
					DWORD dwWritten = 0;
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
			}
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

