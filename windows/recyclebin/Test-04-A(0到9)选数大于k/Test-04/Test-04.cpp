// Test-04.cpp : �������̨Ӧ�ó������ڵ㡣
//
/*
2.����һ������A������ֻ����0-9��10�����֣�����һ��ȫ�����֣�Ȼ�����һ��K��ֵ����A�д���K������������С��һ���������������A={0��1}�� k =12,����Ϊ100.    

����ʵ�֡�
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

//��һ��
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

----------------------> ��ɨ��һ�飬����û�е��ڵģ�ֱ��ɨ���� �� û����ģ����µ�ǰλ�ã�
<-----------------..... Ȼ��������ɨ��������û�д�һ��ģ�ֱ�� �� ���� ɨ����

����� ��һ��� �򣨴ӵ�ǰλ�ã� ---------> ����С

��� ɨ���� Ҳû ��һ��� ���� ��һλ�е���Сֵ

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

	//----------------------> ��ɨ��һ�飬����û�е��ڵģ�ֱ��ɨ���� �� û����ģ����µ�ǰλ�ã�

	unsigned int i = 0;

	for( i = 0; i < length; i++ )
	{
		if (!flagNum[ *(number+i) - '0' ])
		{
			break;
		}
	}

	//<-----------------..... Ȼ��������ɨ��������û�д�һ��ģ�ֱ�� �� ���� ɨ����
	unsigned int j = 0;
	unsigned int jL = 0;

	for( j = i; j > 0; j-- )
	{
		if (larger_little( flagNum, *(number+j) - '0', &jL))
		{
			break;
		}
	}

	//����� ��һ��� �򣨴ӵ�ǰλ�ã� ---------> ����С
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
	else//��� ɨ���� Ҳû ��һ��� ���� ��һλ�е���Сֵ
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