// Test-06.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

using namespace std;

void calc(char * str,unsigned int length);
int _tmain(int argc, _TCHAR* argv[])
{

	char testStr1[32] = "AshWD12ji#NqqI#84h#$hHuic2HE";
	char testStr[32] = "AshWD12ji#NqqI#84h#$hHuic2HE";
	char * testStr2 = testStr;
	char * testStr3 = "AshWD12ji#NqqI#84h#$hHuic2HE";

	cout << testStr1 << endl;
	calc(testStr1,(unsigned int)strlen(testStr1));
	cout << testStr1 << endl;

	cout << endl;

	cout << testStr2 << endl;
	calc(testStr2,(unsigned int)strlen(testStr2));
	cout << testStr2 << endl;

	cout << endl;

	cout << testStr3 << endl;
	calc(testStr3,(unsigned int)strlen(testStr3));
	cout << testStr3 << endl;

	return 0;
}

void calc(char * str,unsigned int length)
{
	if (str)
	{
		unsigned int pos = 0;
		for (unsigned int i = 0; i < length; i++)
		{
			if ( *(str+i) <= 'z' && *(str+i) >= 'a' )
			{
				++pos;
			}
		}

		char * p1 = str;
		char * p2 = str + pos;
		char tmp = 0;

		for (unsigned int i = 0; i < length; i++)
		{
			if ( *p1 <= 'z' && *p1 >= 'a' ) ++p1;
			else
			{
				if ( *p2 > 'z' || *p2 < 'a' )
				{
					++p2;
				}
				else
				{
					tmp = *p1;
					*p1 = *p2;
					*p2 = tmp;

					++p2;
				}
			}
		}
	}
}