#include <iostream>
#include "TestDll.h"
#pragma comment(lib, "TestDll.lib")
int main()
{
	SayHello();
    std::cout << Add(32, 58) << "\n";
	SayHello();
	while(1){}
	return 0;
}