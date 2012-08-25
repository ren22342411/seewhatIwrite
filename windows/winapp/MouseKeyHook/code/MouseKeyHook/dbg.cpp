#include "dbg.h"

#ifdef _DEBUG
void ShowUp(const char * lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	char szBuffer[1024]="";
	wvsprintf(szBuffer,lpszFormat, args);
	va_end(args);
	OutputDebugString(szBuffer);
}
#endif