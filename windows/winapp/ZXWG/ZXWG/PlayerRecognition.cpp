#include "stdafx.h"

static const long _HpUpB = 0; 
static const long _HpUpG = 0; 
static const long _HpUpR = 248; 
static const long _MpUpB = 213; 
static const long _MpUpG = 109; 
static const long _MpUpR = 34;

static const int HpMpLength = 118;
static const int HpMpWidth = 11;

static const int HpUpLeftX = 123;
static const int HpUpLeftY = 41;
static const int HpUpRightX = HpUpLeftX + HpMpLength - 1;
static const int HpUpRightY = HpUpLeftY;
static const int HpDownLeftX = HpUpLeftX;
static const int HpDownLeftY = HpUpLeftY + HpMpWidth - 1;
static const int HpDownRightX = HpDownLeftX + HpMpLength - 1;
static const int HpDownRightY = HpDownLeftY;

static const int MpUpLeftX = 123;
static const int MpUpLeftY = 59;
static const int MpUpRightX = MpUpLeftX + HpMpLength - 1;
static const int MpUpRightY = MpUpLeftY;
static const int MpDownLeftX = MpUpLeftX;
static const int MpDownLeftY = MpUpLeftY + HpMpWidth - 1;
static const int MpDownRightX = MpDownLeftX + HpMpLength - 1;
static const int MpDownRightY = MpDownLeftY;

enum PlayerRecognitionAccuracy{L=1,M,H};

typedef struct _ColorRange
{
	long min;
	long max;
	long dmin;
	long dmax;
}ColorRange;

class PlayerRecognition
{
public:
	PlayerRecognition()
	{/**/
		HpUpA.min=1279;
		HpUpA.max=639487;
		HpUpA.dmin=-216320;
		HpUpA.dmax=214016;

		HpUpB.min=0;
		HpUpB.max=9;
		HpUpB.dmin=-3;
		HpUpB.dmax=3;

		HpUpG.min=4;
		HpUpG.max=212;
		HpUpG.dmin=-79;
		HpUpG.dmax=67;

		HpUpR.min=255;
		HpUpR.max=255;
		HpUpR.dmin=0;
		HpUpR.dmax=0;

		HpDownA.min=329871;
		HpDownA.max=613631;
		HpDownA.dmin=-133394;
		HpDownA.dmax=133393;

		HpDownB.min=5;
		HpDownB.max=9;
		HpDownB.dmin=-2;
		HpDownB.dmax=2;

		HpDownG.min=5;
		HpDownG.max=92;
		HpDownG.dmin=-11;
		HpDownG.dmax=11;

		HpDownR.min=129;
		HpDownR.max=255;
		HpDownR.dmin=-22;
		HpDownR.dmax=20;

		MpUpA.min=13987106;
		MpUpA.max=14058070;
		MpUpA.dmin=-66052;
		MpUpA.dmax=65796;

		MpUpB.min=213;
		MpUpB.max=214;
		MpUpB.dmin=-1;
		MpUpB.dmax=1;

		MpUpG.min=109;
		MpUpG.max=130;
		MpUpG.dmin=-3;
		MpUpG.dmax=3;

		MpUpR.min=34;
		MpUpR.max=86;
		MpUpR.dmin=-6;
		MpUpR.dmax=5;

		MpDownA.min=6895123;
		MpDownA.max=7485972;
		MpDownA.dmin=-525313;
		MpDownA.dmax=525313;

		MpDownB.min=105;
		MpDownB.max=114;
		MpDownB.dmin=-8;
		MpDownB.dmax=8;

		MpDownG.min=54;
		MpDownG.max=59;
		MpDownG.dmin=-4;
		MpDownG.dmax=4;

		MpDownR.min=19;
		MpDownR.max=22;
		MpDownR.dmin=-1;
		MpDownR.dmax=1;
	}

	bool isFightingNow(HWND hZX,PlayerRecognitionAccuracy pra)//Must Be Alive
	{
		bool flag = false;
		HDC hdcZX=::GetDC(hZX);

		bool conditions;
		::COLORREF CurrentColour = GetPixel(hdcZX,HpUpLeftX,HpUpLeftY);
		switch(pra)
		{
		case L:
			conditions = _HpUpR == R_Colour(CurrentColour);
			break;
		case M:
			conditions = _HpUpR == R_Colour(CurrentColour) && _HpUpB == B_Colour(CurrentColour);
			break;
		case H:
			conditions = _HpUpR == R_Colour(CurrentColour) && _HpUpG == G_Colour(CurrentColour) && _HpUpB == B_Colour(CurrentColour);
			break;
		}

		if(conditions) 
			flag = false;
		else flag = true;
		ReleaseDC(hZX,hdcZX);
		return flag;
	}

	int getPlayerHpNumNotFighting(HWND hZX,PlayerRecognitionAccuracy pra)
	{
		int hpNum = -1;
		HDC hdcZX=::GetDC(hZX);
		COLORREF HpUp[118];
		getColorHpMpArray(hdcZX,HpUp,HpUpLeftY);
		switch(pra)
		{
		case L:
			{
				hpNum = 0;
				while(_HpUpR == R_Colour(HpUp[hpNum]))
				{
					++hpNum;
				}
			}
			break;
		case M:
			{
				hpNum = 0;
				while(_HpUpR == R_Colour(HpUp[hpNum]) && _HpUpB == B_Colour(HpUp[hpNum]))
				{
					++hpNum;
				}
			}
			break;
		case H:			
			{
				hpNum = 0;
				while(_HpUpR == R_Colour(HpUp[hpNum]) && _HpUpG == G_Colour(HpUp[hpNum]) && _HpUpB == B_Colour(HpUp[hpNum]))
				{
					++hpNum;
				}
			}
			break;
		}
		ReleaseDC(hZX,hdcZX);
		return hpNum;
	}

	int getPlayerHpNumWhenFighting(HWND hZX,PlayerRecognitionAccuracy pra)
	{
		int hpNum = -1;
		HDC hdcZX=::GetDC(hZX);
		COLORREF HpUp[118];
		COLORREF HpDown[118];
		getColorHpMpArray(hdcZX,HpUp,HpUpLeftY);
		getColorHpMpArray(hdcZX,HpDown,HpDownLeftY);

		switch(pra)
		{
		case L:
			{
				int hpNumA[3];
				hpNumA[0]=getHpMpNumber(HpUp,HpUpR,3);
				hpNumA[1]=getHpMpNumber(HpDown,HpDownB,1);
				hpNumA[2]=getHpMpNumber(HpUp,HpUpB,1);
				hpNum = findRightHpMpNumber(hpNumA,3);
			}
			break;
		case M:
			{
				int hpNumA[4];
				hpNumA[0]=getHpMpNumber(HpUp,HpUpR,3);
				hpNumA[1]=getHpMpNumber(HpDown,HpDownB,1);
				hpNumA[2]=getHpMpNumber(HpUp,HpUpB,1);
				hpNumA[3]=getHpMpNumber(HpDown,HpDownG,2);
				hpNum = findRightHpMpNumber(hpNumA,4);
			}
			break;
		case H:
			{
				int hpNumA[5];
				hpNumA[0]=getHpMpNumber(HpUp,HpUpR,3);
				hpNumA[1]=getHpMpNumber(HpDown,HpDownB,1);
				hpNumA[2]=getHpMpNumber(HpUp,HpUpB,1);
				hpNumA[3]=getHpMpNumber(HpDown,HpDownG,2);
				hpNumA[4]=getHpMpNumber(HpDown,HpDownR,3);
				hpNum = findRightHpMpNumber(hpNumA,5);
			}
			break;
		}
		ReleaseDC(hZX,hdcZX);
		return hpNum;
	}

	int getPlayerMpNumNotFighting(HWND hZX,PlayerRecognitionAccuracy pra)
	{
		int mpNum = -1;
		HDC hdcZX=::GetDC(hZX);
		COLORREF MpUp[118];
		getColorHpMpArray(hdcZX,MpUp,MpUpLeftY);
		switch(pra)
		{
		case L:
			{
				mpNum = 0;
				while(_MpUpR == R_Colour(MpUp[mpNum]))
				{
					++mpNum;
				}
			}
			break;
		case M:
			{
				mpNum = 0;
				while(_MpUpR == R_Colour(MpUp[mpNum]) && _MpUpB == B_Colour(MpUp[mpNum]))
				{
					++mpNum;
				}
			}
			break;
		case H:			
			{
				mpNum = 0;
				while(_MpUpR == R_Colour(MpUp[mpNum]) && _MpUpG == G_Colour(MpUp[mpNum]) && _MpUpB == B_Colour(MpUp[mpNum]))
				{
					++mpNum;
				}
			}
			break;
		}
		ReleaseDC(hZX,hdcZX);
		return mpNum;
	}

	int getPlayerMpNumWhenFighting(HWND hZX,PlayerRecognitionAccuracy pra)
	{
		int mpNum = -1;
		HDC hdcZX=::GetDC(hZX);
		COLORREF MpUp[118];
		COLORREF MpDown[118];
		getColorHpMpArray(hdcZX,MpUp,MpUpLeftY);
		getColorHpMpArray(hdcZX,MpDown,MpDownLeftY);

		switch(pra)
		{
		case L:
			{
				int mpNumA[3];
				mpNumA[0]=getHpMpNumber(MpUp,MpUpB,1);
				mpNumA[1]=getHpMpNumber(MpDown,MpDownR,3);
				mpNumA[2]=getHpMpNumber(MpUp,MpUpG,2);
				mpNum = findRightHpMpNumber(mpNumA,3);
			}
			break;
		case M:
			{
				int mpNumA[4];
				mpNumA[0]=getHpMpNumber(MpUp,MpUpB,1);
				mpNumA[1]=getHpMpNumber(MpDown,MpDownR,3);
				mpNumA[2]=getHpMpNumber(MpUp,MpUpG,2);
				mpNumA[3]=getHpMpNumber(MpDown,MpDownG,2);
				mpNum = findRightHpMpNumber(mpNumA,4);
			}
			break;
		case H:
			{
				int mpNumA[5];
				mpNumA[0]=getHpMpNumber(MpUp,MpUpB,1);
				mpNumA[1]=getHpMpNumber(MpDown,MpDownR,3);
				mpNumA[2]=getHpMpNumber(MpUp,MpUpG,2);
				mpNumA[3]=getHpMpNumber(MpDown,MpDownG,2);
				mpNumA[3]=getHpMpNumber(MpDown,MpDownB,1);
				mpNum = findRightHpMpNumber(mpNumA,5);
			}
			break;
		}
		ReleaseDC(hZX,hdcZX);
		return mpNum;
	}


	double getPlayerHpPercentage(HWND hZX,PlayerRecognitionAccuracy pra)
	{
		double hpPercentage=0;
		if(isFightingNow(hZX,pra))
			hpPercentage=((double)getPlayerHpNumWhenFighting(hZX,pra))/((double)HpMpLength);
		else
			hpPercentage=((double)getPlayerHpNumNotFighting(hZX,pra))/((double)HpMpLength);
		return hpPercentage;
	}

	double getPlayerMpPercentage(HWND hZX,PlayerRecognitionAccuracy pra)
	{
		double mpPercentage=0;
		if(isFightingNow(hZX,pra))
			mpPercentage=((double)getPlayerMpNumWhenFighting(hZX,pra))/((double)HpMpLength);
		else
			mpPercentage=((double)getPlayerMpNumNotFighting(hZX,pra))/((double)HpMpLength);
		return mpPercentage;
	}
private:
	ColorRange HpUpA;
	ColorRange HpUpB;
	ColorRange HpUpG;
	ColorRange HpUpR;
	ColorRange HpDownA;
	ColorRange HpDownB;
	ColorRange HpDownG;
	ColorRange HpDownR;
	ColorRange MpUpA;
	ColorRange MpUpB;
	ColorRange MpUpG;
	ColorRange MpUpR;
	ColorRange MpDownA;
	ColorRange MpDownB;
	ColorRange MpDownG;
	ColorRange MpDownR;

	long A_Colour(COLORREF x)
	{
	 return (long)(x);
	}

	long R_Colour(COLORREF x)
	{
	 return (long)(( x << 24) >> 24);
	}

	long G_Colour(COLORREF x)
	{
	 return (long)(( x << 16) >> 24);
	}

	long B_Colour(COLORREF x)
	{
	 return (long)(( x << 8) >> 24);
	}

	void getColorHpMpArray(HDC hdcZX,COLORREF *pHpMpArray,int HpMpLeftY)
	{
		for(int i=0;i<HpMpLength;i++)
		{
			*(pHpMpArray+i) = GetPixel(hdcZX,123+i,HpMpLeftY);
		}
	}

	int getHpMpNumber(COLORREF *pHpMpArray,ColorRange &cr,int abgr)
	{
		if( abgr < 4 && abgr >= 0)
		{
		int num=0;
		switch(abgr)
		{
		case 0:
			{
				num = 0;
				while( A_Colour(*(pHpMpArray+num)) <= cr.max && A_Colour(*(pHpMpArray+num)) >= cr.min 
					&& ( A_Colour(*(pHpMpArray+num+1)) - A_Colour(*(pHpMpArray+num)) ) <= cr.dmax && ( A_Colour(*(pHpMpArray+num+1)) - A_Colour(*(pHpMpArray+num)) ) >= cr.dmin )
				{
					++num;
					if(num == HpMpLength-1) break;
				}
				if( num == HpMpLength-1 && (A_Colour(*(pHpMpArray+num)) <= cr.max && A_Colour(*(pHpMpArray+num)) >= cr.min))
					num = HpMpLength;
			}
			break;
		case 1:
			{
				num = 0;
				while( B_Colour(*(pHpMpArray+num)) <= cr.max && B_Colour(*(pHpMpArray+num)) >= cr.min 
					&& ( B_Colour(*(pHpMpArray+num+1)) - B_Colour(*(pHpMpArray+num)) ) <= cr.dmax && ( B_Colour(*(pHpMpArray+num+1)) - B_Colour(*(pHpMpArray+num)) ) >= cr.dmin )
				{
					++num;
					if(num == HpMpLength-1) break;
				}
				if( num == HpMpLength-1 && (B_Colour(*(pHpMpArray+num)) <= cr.max && B_Colour(*(pHpMpArray+num)) >= cr.min))
					num = HpMpLength;
			}
			break;
		case 2:
			{
				num = 0;
				while( G_Colour(*(pHpMpArray+num)) <= cr.max && G_Colour(*(pHpMpArray+num)) >= cr.min 
					&& ( G_Colour(*(pHpMpArray+num+1)) - G_Colour(*(pHpMpArray+num)) ) <= cr.dmax && ( G_Colour(*(pHpMpArray+num+1)) - G_Colour(*(pHpMpArray+num)) ) >= cr.dmin )
				{
					++num;
					if(num == HpMpLength-1) break;
				}
				if( num == HpMpLength-1 && (G_Colour(*(pHpMpArray+num)) <= cr.max && G_Colour(*(pHpMpArray+num)) >= cr.min))
					num = HpMpLength;
			}
			break;
		case 3:
			{
				num = 0;
				while( R_Colour(*(pHpMpArray+num)) <= cr.max && R_Colour(*(pHpMpArray+num)) >= cr.min 
					&& ( R_Colour(*(pHpMpArray+num+1)) - R_Colour(*(pHpMpArray+num)) ) <= cr.dmax && ( R_Colour(*(pHpMpArray+num+1)) - R_Colour(*(pHpMpArray+num)) ) >= cr.dmin )
				{
					++num;
					if(num == HpMpLength-1) break;
				}
				if( num == HpMpLength-1 && (R_Colour(*(pHpMpArray+num)) <= cr.max && R_Colour(*(pHpMpArray+num)) >= cr.min))
					num = HpMpLength;
			}/**/
			break;
		}
		return num;
		}
		else return 0;
	}
	int findRightHpMpNumber(int *pHpMpNum,const int size)
	{
		int num=0;
		for(int i=0;i<size;i++)
		{
			for(int j=i;j<size;j++)
			{
				if( *(pHpMpNum+j) < *(pHpMpNum+i) )
				{
					int temp = *(pHpMpNum+j);
					*(pHpMpNum+j) = *(pHpMpNum+i);
					*(pHpMpNum+i) = temp;
				}
			}
		}

		int min = 0;
		int min_v =  *(pHpMpNum+1) - *pHpMpNum;
		for(int i=0;i<size-1;i++)
		{
			if(*(pHpMpNum+i+1) - *(pHpMpNum+i)<min_v)
			{
				min_v = *(pHpMpNum+i+1) - *(pHpMpNum+i);
				min = i;
			}
		}
		num = *(pHpMpNum+min);
		return num;
	}
};