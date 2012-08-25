#include <iostream>
using namespace std;
int main()
{
	//Java Óë C++²»Í¬
	int a = 5;
	int b = 5;
	int c = 5;
	int d = 5;
	int e = 5;
	int f = 5;

	a+=(a++)+(a++)+a;	//5+(5+5+5)+1+1
	cout<<"a="<<a<<endl;

	b+=(b++)+(b--)+b;	//5+(5+5+5)+1-1
	cout<<"b="<<b<<endl;

	c+=(c--)+(c--)+c;	//5+(5+5+5)-1-1
	cout<<"c="<<c<<endl;

	d+=(++d)+(d++)+d;	//6+(6+6+6)+1
	cout<<"d="<<d<<endl;

	e+=(e--)+(--e)+e;	//4+(4+4+4)-1
	cout<<"e="<<e<<endl;

	f+=(--f)+(++f)+f;	//a=5+1=1;//5+(5+5+5)
	cout<<"f="<<f<<endl;

	return 0;
}