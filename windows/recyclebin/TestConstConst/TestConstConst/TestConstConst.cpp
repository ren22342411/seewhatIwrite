// TestConstConst.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	const char cch = 'a';
	const char * const * keywords1;
	const char const * keywords2;
	const char * const keywords3 = &cch;
	const char const keywords4 = 'c'; 
	return 0;
}

