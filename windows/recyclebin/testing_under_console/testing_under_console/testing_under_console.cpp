// testing_under_console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "delete_void_pointer.h"

#include "wcstombs_and_strncat.h"

#include "class_virtual_func.h"

#include "unsigned_char_char.h"

#include "test_snprintf.h"

class GohanConsoleHelper  
{  
    HANDLE _hIn;  
    HANDLE _hOut;  
    INPUT_RECORD _InRec;  
    DWORD _NumRead;  
public:  
    WORD VKey;  
    GohanConsoleHelper(void){  
        _hIn = GetStdHandle(STD_INPUT_HANDLE);  
        _hOut = GetStdHandle(STD_OUTPUT_HANDLE);  
        VKey=0;  
    }  
    bool ReadOneInput()  
    {  
        return 0!=ReadConsoleInput(_hIn,&_InRec,1,&_NumRead);  
    }  
    bool ReadOneInput(INPUT_RECORD& InRec)  
    {  
        return 0!=ReadConsoleInput(_hIn,&InRec,1,&_NumRead);  
    }  
    DWORD ReadKeyDown()  
    {  
        if(!ReadConsoleInput(_hIn,&_InRec,1,&_NumRead))  
            return 0;  
        if(_InRec.EventType!=KEY_EVENT)  
            return 0;  
        if(_InRec.Event.KeyEvent.bKeyDown > 0)  
            return 0;  
        VKey = _InRec.Event.KeyEvent.wVirtualKeyCode;  
        return VKey;  
    }  
    DWORD ReadKeyPush()  
    {  
        if(!ReadConsoleInput(_hIn,&_InRec,1,&_NumRead))  
            return 0;  
        if(_InRec.EventType!=KEY_EVENT)  
            return 0;  
        if(_InRec.Event.KeyEvent.bKeyDown == 0)  
            return 0;  
        VKey = _InRec.Event.KeyEvent.wVirtualKeyCode;  
        return VKey;  
    }  
public:  
    ~GohanConsoleHelper(void){}  
};  

typedef struct _TEMP_STRUCT
{
	int a;
	int b;
}TEMP_STRUCT, *PTEMP_STRUCT;

int _tmain(int argc, _TCHAR* argv[])
{
	//dvp_main_fun();

	//wcstombs_and_strncat_main();

	//classVirtualMechanismMain();

	//unsigned_char_char_main();

	//test_snprintf_main();

	//BOOL bR = AllocConsole();
/*
	    GohanConsoleHelper gch;  
    while (true)  
    {  
        if(gch.ReadKeyPush()!=0) //使用ReadKeyDown()捕获按键弹起的VK_CODE  
        {  
            if(gch.VKey != VK_ESCAPE)  
                cout<<"VK_CODE == "<<gch.VKey<<endl;  
            else {  
                cout<<"Bye~~"<<endl;  
                break;  
            }  
        }  
    }  

	*/
	/*static ULONGLONG i = 0;
	i++;
	printf("%d\n",i);
	return _tmain(argc, argv);
	*/
	/*auto x = 2*(float)3.3;
	cout << sizeof(x);*/
	PTEMP_STRUCT pts = (PTEMP_STRUCT)malloc(sizeof(TEMP_STRUCT));

	pts->a = 55;
	pts->b = 66;

	free(pts);

	cout << pts->a << endl;
	cout << pts->b << endl;



	return 0;

}

