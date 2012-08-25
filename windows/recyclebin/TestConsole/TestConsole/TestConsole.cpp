// TestConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <vector>
using namespace std;
typedef struct _MYSTRUCT
{
	LPTSTR name;
	LPTSTR val;
}MY_STRUCT;
vector<MY_STRUCT> iva[4];
int _tmain(int argc, _TCHAR* argv[])
{
	if(false)//自增自减表达式
	{
		//Java 与 C++不同
		int a = 5;
		int b = 5;
		int c = 5;
		int d = 5;
		int e = 5;
		int f = 5;

		a+=(a++)+(a++)+a;	//5+(5+5+5)+1+1
		cout<<"a="<<a<<endl;

		b+=(b++)+(b--)+b;	//5+(5+5+5)+1-1
		cout<<"b="<<b<<endl;

		c+=(c--)+(c--)+c;	//5+(5+5+5)-1-1
		cout<<"c="<<c<<endl;

		d+=(++d)+(d++)+d;	//6+(6+6+6)+1
		cout<<"d="<<d<<endl;

		e+=(e--)+(--e)+e;	//4+(4+4+4)-1
		cout<<"e="<<e<<endl;

		f+=(--f)+(++f)+f;	//a=5+1=1;//5+(5+5+5)
		cout<<"f="<<f<<endl;
	}
	else if(true)
	{



		for(int i =0;i<100;i++)
		{
			MY_STRUCT mi;
			mi.name = new TCHAR [128];
			mi.val = new TCHAR [128];
			iva[0].push_back(mi);
		}

	}

	return 0;
}

