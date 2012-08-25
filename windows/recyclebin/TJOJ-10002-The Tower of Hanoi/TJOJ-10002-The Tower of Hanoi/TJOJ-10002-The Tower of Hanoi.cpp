// TJOJ-10002-The Tower of Hanoi.cpp : 定义控制台应用程序的入口点。
//
/*
Description
You are given a tower of n disks, initially stacked in decreasing size on one of three pegs, your objective is to transfer the entire tower to one of the other pegs, moving only one disk at a time and never moving a larger one onto a smaller. For example, n = 2:

We need 3 moves to perform the task.

Now the question arises: What's the best we can do? That is, how many moves are necessary and sufficient to perform the task? 

Please write a program to find out the minimum number of moves we need to do with n disks in the tower.

Input
One positive integer, n (1 <= n <= 64).

Output
One integer, the minimum number of moves needed.

Sample Input
2

Sample Output
3

Hint
The answer might be a huge integer which cannot be hold in a 32-bit int variable.
*/
#include "stdafx.h"

#include <iostream>

using namespace std;

__int64 f(int i);

int _tmain(int argc, _TCHAR* argv[])
{
	long i = 0;
	cin >> i;
	if (i==64)
	{
		cout << 1 ;
		cout << 8 ;
		cout << 4 ;
		cout << 4 ;
		cout << 6 ;
		cout << 7 ;
		cout << 4 ;
		cout << 4 ;
		cout << 0 ;
		cout << 7 ;
		cout << 3 ;
		cout << 7 ;
		cout << 0 ;
		cout << 9 ;
		cout << 5 ;
		cout << 5 ;
		cout << 1 ;
		cout << 6 ;
		cout << 1;
		cout << 5;
	}
	else
		cout << f(i);
	return 0;
}

__int64 f(int i)
{
	if (i==0)
	{
		return 0;
	}
	else if (i>0)
	{
		return f(i - 1) * 2 + 1;
	}
	else if (i<0)
	{
		return -1;
	}
}