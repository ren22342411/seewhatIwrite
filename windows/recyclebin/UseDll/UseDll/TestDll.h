#ifndef _DLL_TESTDLL_H_
#define _DLL_TESTDLL_H_
#include <iostream>
#if defined DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

extern "C"
{
        DECLDIR int Add( int a, int b );
        DECLDIR void SayHello( void );
}
#endif
