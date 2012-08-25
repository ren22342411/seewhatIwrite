// Test-07.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;

union u{
	int a[4];
	char b;
	double c;
};

struct s{
	char a;
	u b;
};

struct s_ch{
	char a;
};

union u_ch{
	char a;
};

struct su{
	int f;
	s_ch a;
	u_ch b;
	s_ch c;
	u_ch d;
	u_ch e;
};

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "char:\t" << sizeof(char) << endl;
	cout << "int:\t" << sizeof(int) << endl;
	cout << "short:\t" << sizeof(short) << endl;
	cout << "long:\t" << sizeof(long) << endl;
	cout << "float:\t" << sizeof(float) << endl;
	cout << "double:\t" << sizeof(double) << endl;
	cout << "wchar_t:\t" << sizeof(wchar_t) << endl;
	cout << "long long:\t" << sizeof(long long) << endl;
	cout << "__int64:\t" << sizeof(__int64) << endl;
	cout << "u:\t" << sizeof(u) << endl;
	cout << "s:\t" << sizeof(s) << endl;//以8位对齐
	cout << "s_ch:\t" << sizeof(s_ch) << endl;
	cout << "u_ch:\t" << sizeof(u_ch) << endl;
	cout << "su:\t" << sizeof(su) << endl;

	return 0;
}

