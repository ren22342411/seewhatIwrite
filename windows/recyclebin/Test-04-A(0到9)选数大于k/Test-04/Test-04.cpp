// Test-04.cpp : 定义控制台应用程序的入口点。
//
/*
2.给定一个数组A，里面只出现0-9这10个数字，但不一定全部出现，然后给定一个K的值，求A中大于K的整数当中最小的一个，并输出。例如A={0，1}， k =12,则结果为100.    

请编程实现。
*/
#include "stdafx.h"
#include <iostream>
using namespace std;

void calc( unsigned int * A , unsigned int size , unsigned int number );
void calc( unsigned int * A , unsigned int size , char * number ,  unsigned int length );

int _tmain(int argc, _TCHAR* argv[])
{
	const unsigned int size = 4;
	unsigned int A[size] = {5,6,7,8};
	unsigned int k = 5678;

	unsigned int AA[size] = {5,6,7,8};
	char * kk = "5678";

	calc(A,size,k);
	calc(AA,size,kk,strlen(kk));

	return 0;
}

//O(n2)
void calc( unsigned int * A , unsigned int size , unsigned int number )
{
	bool flagNum[10] = {false};
	unsigned int larger = number;
	unsigned int largerC;

	for (unsigned int i = 0 ; i < size ; i++ )
	{
		flagNum[*(A+i)] = true;
	}

	while (true)
	{
		++larger;
		largerC = larger;
		while (largerC)
		{
			if(!flagNum[largerC%10]) break;
			else largerC /= 10; 
		}
		if(!largerC) break;
	}

	printf("%d\n",larger);
}

//大一点
bool larger_little(bool flagNum[10], unsigned int jV, unsigned int * jL)
{
	while( jV < 9 )
	{
		if(flagNum[++jV])
		{
			*jL = jV;
			return true;
		}
	}
	return false;
}

void calc( unsigned int * A , unsigned int size , char * number ,  unsigned int length )
{
/*

----------------------> 先扫描一遍，看有没有等于的，直到扫描完 或 没等与的（记下当前位置）
<-----------------..... 然后在往回扫，看看有没有大一点的，直到 有 或者 扫描完

如果有 大一点的 则（从当前位置） ---------> 填最小

如果 扫描完 也没 大一点的 返回 多一位中的最小值

*/

	bool flagNum[10] = {false};

	for (unsigned int n = 0 ; n < size ; n++ )
	{
		flagNum[*(A+n)] = true;
	}

	unsigned int SortA[10] = {0};
	unsigned int m = 0;

	for (unsigned int n = 0 ; n < 10 ; n++ )
	{
		if (flagNum[n])
		{
			SortA[m++] = n; 
		}
	}

	//----------------------> 先扫描一遍，看有没有等于的，直到扫描完 或 没等与的（记下当前位置）

	unsigned int i = 0;

	for( i = 0; i < length; i++ )
	{
		if (!flagNum[ *(number+i) - '0' ])
		{
			break;
		}
	}

	//<-----------------..... 然后在往回扫，看看有没有大一点的，直到 有 或者 扫描完
	unsigned int j = 0;
	unsigned int jL = 0;

	for( j = i; j > 0; j-- )
	{
		if (larger_little( flagNum, *(number+j) - '0', &jL))
		{
			break;
		}
	}

	//如果有 大一点的 则（从当前位置） ---------> 填最小
	if(j)
	{
		for (unsigned int n = 0 ; n < j ; n++)
		{
			printf("%c",number[n]);
		}
		printf("%d",jL);
		for (unsigned int n = j+1 ; n < length ; n++)
		{
			printf("%d",SortA[0]);
		}
		printf("\n");
	}
	else//如果 扫描完 也没 大一点的 返回 多一位中的最小值
	{
		if (SortA[0])
		{
			for (unsigned int n = 0 ; n < length + 1 ; n++)
			{
				printf("%d",SortA[0]);
			}
			printf("\n");
		}
		else
		{
			printf("%d",SortA[1]);
			for (unsigned int n = 1 ; n < length + 1; n++)
			{
				printf("%d",SortA[0]);
			}
			printf("\n");
		}
	}
}