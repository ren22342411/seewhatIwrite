#include "stdafx.h"
#include "class_virtual_func.h"

void classVirtualMechanismMain()
{
	SubClass sc;
	sc.FunctionA();
}

void BaseClass::FunctionA()
{
	cout << "BaseClass::FunctionA()" << endl;
	FunctionB();
}

void BaseClass::FunctionB()
{
	cout << "BaseClass::FunctionB()" << endl;
}

void SubClass::FunctionA()
{
	cout << "SubClass::FunctionA()" << endl;
	BaseClass::FunctionA();
}

void SubClass::FunctionB()
{
	BaseClass::FunctionB();
	cout << "SubClass::FunctionB()" << endl;
}


void SubClass::FunctionC()
{
	cout << "SubClass::FunctionC()" << endl;
}
