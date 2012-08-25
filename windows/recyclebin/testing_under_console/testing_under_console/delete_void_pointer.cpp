#include "stdafx.h"
#include "delete_void_pointer.h"

const char * dvp_name = "delete void pointer";
int dvp_main_fun();

class ClassDVP;

typedef ClassDVP * PClassDVP;

class ClassDVP
{
public:
	ClassDVP()
	{
		cout << "ClassDVP constructor" << endl;
	};
	
	~ClassDVP()
	{
		cout << "ClassDVP destructor" << endl;
	};
};


int dvp_main_fun()
{
	PClassDVP pcdvp = new ClassDVP();

	delete pcdvp;

	PClassDVP pcdvp1 = new ClassDVP();

	void * pcdvp2 = (void *)pcdvp1;

	delete pcdvp2; // destructor will not be called.

	return 0;
}