// Test-03.cpp : �������̨Ӧ�ó������ڵ㡣
//
/*
(1)��һ���ַ�����Ҫ��ͳ�������ACII���Ƶ�������д�д����ĸ����Сд��ĸ��ͳ�ƣ������ʱ����������������ַ�����bbCca*  �������b��2 C��2 a��1 *��1������ĸ��
�γ��ֵ�˳�����������һ��c��������ʽΪvoid calc��char* input��int len������Ҳ��������ϲ��������ȥд��

(2)��������������֤���������еĸ��ֿ����ԡ�
*/
#include "stdafx.h"
#include <iostream>
using namespace std;

void calc(char * input, int len);
int _tmain(int argc, _TCHAR* argv[])
{
	char * ch = "aaQ@@**3432sd$sq###qqBBAAb&bc";
	calc(ch,(int)strlen(ch));
	return 0;
}

void calc(char * input, int len)
{
	char char_sequence[256]={0};
	int char_times[256]={0};
	int sequence = 0;

	for (int i = 0; i < len; i++)
	{
		char temCh = *( input + i ) <= 'Z' &&  *( input + i ) >= 'A' ? *( input + i ) + 'a' - 'A' :  *( input + i );

		if ( char_times[temCh] == 0 )
		{
			char_sequence[sequence++] =temCh;/* *( input + i ) -- ��һ�����ֵ���ʲô��ʲô*//*temCh -- ���ܴ�Сд��һ��Сд*///��һ�����ֵ���ĸ��1�����ܴ�Сд��һ��Сд��2��
		}
		
		++char_times[temCh];

	}

	for ( int i = 0 ; i < sequence ; i++ )
	{
		printf("%c:%d\n",char_sequence[i],char_times[char_sequence[i]]);
	}
}
