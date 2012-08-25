// TJOJ-10001-Summing a Series.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

/*
Description
Sum a given integer series.

Input
A series of positive integers, one per line. Proceed to the end of input.

Output
One integer, which is the sum of the input series. Our input guarantees the correct answer will never exceed 231-1.

Sample Input
3
9
7

Sample Output
19
*/

#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int sum = 0;
	int input = 0;
	while (cin>>input)
	{
		sum+=input;
	}
	cout<<sum;
	return 0;
}

