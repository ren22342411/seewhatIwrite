#include "stdafx.h"
#include "wcstombs_and_strncat.h"

void wcstombs_and_strncat_main()
{
	// Test 1
	wchar_t * wstr = L"hello ";
	char * str = "wcstombs_and_strncat";
	char str_new[256] = {0};

	wprintf( L"%s\t%u\n", wstr, wcslen(wstr));

	unsigned long r1 = wcstombs(str_new, wstr, wcslen(wstr));

	cout << str_new << "\t" << strlen(str_new) << endl;
	cout << r1 << endl;

	char * str1 = strncat(str_new, str, 256 - wcslen(wstr));

	cout << str1 << endl;

	wprintf( L"%s\t%u\n", wstr, wcslen(wstr));

	cout << str << "\t" << strlen(str) << endl;

	cout << str_new << "\t" << strlen(str_new) << endl;

	// Test 2
	wchar_t * wstr1 = L"hello, I am a String!";
	char astr1[256] = {1};

	unsigned long r2 = wcstombs(astr1, wstr1, 256);

	cout << astr1 << "\t" << strlen(astr1) << "\t" << wcslen(wstr1) << endl;
}