void classVirtualMechanismMain();

class BaseClass
{
public:
	void FunctionA();
	virtual void FunctionC() = 0;
	virtual void FunctionB();
	
	// Could pass the compiling if the FunctionD will bot be invoked
	void FunctionD(); 
	
	// Could not pass the compiling although the FunctionD will bot be invoked.
	// A defination of the impure virtual function is required by the compliler.
	// virtual void FunctionB(); 

};

class SubClass : public BaseClass
{
public:
	void FunctionA();
	virtual void FunctionB();
	virtual void FunctionC();

};
