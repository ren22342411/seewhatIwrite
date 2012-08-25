// Test-03.cpp : 定义控制台应用程序的入口点。
//
/*
(1)给一个字符串，要你统计里面的ACII码的频数，其中大写的字母算作小写字母来统计，输出的时候这样输出：假如字符串是bbCca*  ，则输出b：2 C：2 a：1 *：1，按字母首
次出现的顺序输出。给出一个c函数的形式为void calc（char* input，int len），你也可以用你喜欢的语言去写。

(2)给出测试数据来证明程序运行的各种可能性。
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
			char_sequence[sequence++] =temCh;/* *( input + i ) -- 第一个出现的是什么就什么*//*temCh -- 不管大小写，一律小写*///第一个出现的字母，1、不管大小写，一律小写，2、
		}
		
		++char_times[temCh];

	}

	for ( int i = 0 ; i < sequence ; i++ )
	{
		printf("%c:%d\n",char_sequence[i],char_times[char_sequence[i]]);
	}
}
