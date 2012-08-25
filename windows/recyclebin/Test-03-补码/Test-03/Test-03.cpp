// Test-03.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int _x = -54;
	int _x2 = -27;
	int _2x = -108;
	int _xR = _x >> 1;
	int _xL = _x << 1;
	printf("0x%x\n",_x);
	printf("0x%x\n",_x2);
	printf("0x%x\n",_2x);
	printf("0x%x\n",_xR);
	printf("0x%x\n",_xL);
	return 0;
}

