#ifndef _DBG_H
#define _DBG_H

#include "StdAfx.h"

#ifdef _DEBUG
	void ShowUp(const char* lpszFormat, ...);
#else
	#define ShowUp(X)
#endif

#endif