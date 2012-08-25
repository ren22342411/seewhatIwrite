// Test-05.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

template<typename T>
void cal1( T * a, T * b, size_t size, T * c );
template<typename T>
void cal2( T * a, T * b, size_t size, T * c );
int _tmain(int argc, _TCHAR* argv[])
{
	int a[16] = {1,2,2,3,2,1,1,3,3,4,1,2,3,3,2,4};
	int b[16] = {3,2,1,2,1,1,3,3,2,4,2,4,4,1,2,2};
	int c[16] = {0};
	int d[16] = {0};
	cal1(a,b,4,c);
	cal2(a,b,4,d);
	c;
	d;
	return 0;
}

template<typename T>
void cal1( T * a, T * b, size_t size, T * c )
{
	size_t i = 0;
	size_t j = 0;
	for( i = 0; i < size ; i++ )
	{
		for( j = 0; j < size ; j++ )
		{
			for(size_t k = 0; k < size ; k++ )
			{
				*( c + i * size + j ) += (*( a + i * size + k )) * (*( b + k * size + j ));
			}
		}
	}
}

template<typename T>
void cal2( T * a, T * b, size_t size, T * c )
{/*
 for(size_t x = 0; x < size; x++ ) 
 for(size_t y = 0; y < size; y++ )
 { 
 T temp = *(b + x*size +y); 
 *(b + x*size +y) = *(b + y*size +x); 
 *(b + y*size +x) = temp; 
 } 
 */
	size_t i = 0;
	size_t j = 0;
	for( i = 0; i < size * size; i++ )
	{
		for( j = 0; j < size; j++ )
		{
			*( c + i ) += ( * ( a + (i/size) * size + j ) ) * ( *( b + j * size + i % size ) );
		}
	}
}