// Test-02.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	bool bFind = false;
	long x = 0;
	long y = 0;
	int num = 0;
	
	for( int i = 1 ; i < 200 ; i++ )
	{
		for( x = -10000 ; x < 10000 ; x++ )
		{
			for( y = -10000 ; y < 10000 ; y++ )
			{
				if ( i == 24 * x + 15 * y  )
				{
					++num;
					bFind = true;
					break;
				}
			}
			if (bFind)
			{
				break;
			}
		}

		if (bFind)
		{
			cout<<num<<" find: "<<i<<" = 24 * "<< x <<" + 15 * "<< y << endl;
		}

		bFind = false;

	}
	return 0;
}

