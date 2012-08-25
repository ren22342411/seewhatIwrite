#include "stdafx.h"
#include "MonsterPattern.cpp"

static const COLORREF BackColorForMN = 0xC0E10;//怪物名字背景色
static const COLORREF MonsterHPColor[8]={0x606f9,0x4141fd,0x6262fe,0x2727e9,0xca,0xb5,0x9d,0x8e};//怪物HP颜色
static const COLORREF ColorOfMWD[11][2]={{0xffffff,0xffffff},{0xffffff,0x0},{0xffffff,0x0},{0xffffff,0x0},{0xffffff,0x0},{0xffffff,0x0},{0xf9f9f9,0x0},{0xececed,0x0},{0xfbfbfc,0x0},{0xffffff,0x0},{0xffffff,0xffffff}};//怪物空HP颜色特征
static const COLORREF BlackZX = 0x0;
static const COLORREF WhiteZX = 0xffffff;
enum MonsterRecognitionAccuracy{LOWEST=1,LOW,MIDDLE,HIGH,HIGHEST};

class MonsterRecognition
{
public:
	static bool isSeeingMonster(HWND hZX,MonsterRecognitionAccuracy mra)
	{
		bool flag = false;
		HDC hdcZX=::GetDC(hZX);
		switch(mra)
		{
		case LOWEST:
			{
				flag = BackColorForMN==GetPixel(hdcZX,MonsterNameLeft,MonsterNameUp)
					&& BackColorForMN==GetPixel(hdcZX,MonsterNameRight,MonsterNameDown)
					&& MonsterHPColor[0]==GetPixel(hdcZX,MonsterHPLeft-1,MonsterHPUp)
					&& MonsterHPColor[7]==GetPixel(hdcZX,MonsterHPLeft-1,MonsterHPDown);
			}
			break;
		case LOW:
			{
				flag = BackColorForMN==GetPixel(hdcZX,MonsterNameLeft,MonsterNameUp)
					&& BackColorForMN==GetPixel(hdcZX,MonsterNameLeft,MonsterNameDown)
					&& BackColorForMN==GetPixel(hdcZX,MonsterNameRight,MonsterNameUp)
					&& BackColorForMN==GetPixel(hdcZX,MonsterNameRight,MonsterNameDown)
					&& MonsterHPColor[0]==GetPixel(hdcZX,MonsterHPLeft-1,MonsterHPUp)
					&& MonsterHPColor[2]==GetPixel(hdcZX,MonsterHPLeft-1,MonsterHPUp+2)
					&& MonsterHPColor[4]==GetPixel(hdcZX,MonsterHPLeft-1,MonsterHPUp+4)
					&& MonsterHPColor[6]==GetPixel(hdcZX,MonsterHPLeft-1,MonsterHPUp+6);
			}
			break;
		case MIDDLE:
			{
				bool tempFlag1 = BackColorForMN==GetPixel(hdcZX,MonsterNameLeft,MonsterNameUp)
								&&BackColorForMN==GetPixel(hdcZX,MonsterNameLeft,MonsterNameDown)
								&&BackColorForMN==GetPixel(hdcZX,MonsterNameRight,MonsterNameUp)
								&&BackColorForMN==GetPixel(hdcZX,MonsterNameRight,MonsterNameDown);
				bool tempFlag2=true;
				for(int n=0;n<8;++n)
				{
					if(MonsterHPColor[n]!=GetPixel(hdcZX,MonsterHPLeft-1,MonsterHPUp+n))
					{
						tempFlag2=false;
						break;
					}
				}
				flag=tempFlag1&&tempFlag2;
			}
			break;
		case HIGH:
			{
				bool tempFlag1 = BackColorForMN==GetPixel(hdcZX,MonsterNameLeft,MonsterNameUp)
								&&BackColorForMN==GetPixel(hdcZX,MonsterNameLeft,MonsterNameDown)
								&&BackColorForMN==GetPixel(hdcZX,MonsterNameRight,MonsterNameUp)
								&&BackColorForMN==GetPixel(hdcZX,MonsterNameRight,MonsterNameDown)
								&&BackColorForMN==GetPixel(hdcZX,MonsterNameLeft+1,MonsterNameUp)
								&&BackColorForMN==GetPixel(hdcZX,MonsterNameLeft+1,MonsterNameDown)
								&&BackColorForMN==GetPixel(hdcZX,MonsterNameRight-1,MonsterNameUp)
								&&BackColorForMN==GetPixel(hdcZX,MonsterNameRight-1,MonsterNameDown)
								&&BackColorForMN==GetPixel(hdcZX,MonsterNameLeft,MonsterNameUp+1)
								&&BackColorForMN==GetPixel(hdcZX,MonsterNameLeft,MonsterNameDown-1)
								&&BackColorForMN==GetPixel(hdcZX,MonsterNameRight,MonsterNameUp+1)
								&&BackColorForMN==GetPixel(hdcZX,MonsterNameRight,MonsterNameDown-1);

				bool tempFlag2 =  BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp-1)
								&&WhiteZX==GetPixel(hdcZX,MonsterHPLeft-1,MonsterHPUp-2)
								&&WhiteZX==GetPixel(hdcZX,MonsterHPLeft-1,MonsterHPUp-1)
								&&WhiteZX==GetPixel(hdcZX,MonsterHPLeft-1,MonsterHPDown+1);

				bool tempFlag3 = true;
				for(int n=0;n<8;++n)
				{
					if(MonsterHPColor[n]!=GetPixel(hdcZX,MonsterHPLeft-1,MonsterHPUp+n))
					{
						tempFlag3=false;
						break;
					}
				}

				flag=tempFlag1&&tempFlag2&&tempFlag3;
			}
			break;
		case HIGHEST:
			{
				bool tempFlag1 = true;
				for(int n=0;n<14;++n)
				{
					if(BackColorForMN!=GetPixel(hdcZX,MonsterNameLeft,MonsterNameUp+n)
						||BackColorForMN!=GetPixel(hdcZX,MonsterNameLeft+1,MonsterNameUp+n)
						||BackColorForMN!=GetPixel(hdcZX,MonsterNameRight-1,MonsterNameUp+n)
						||BackColorForMN!=GetPixel(hdcZX,MonsterNameRight,MonsterNameUp+n)
						)
					{
						tempFlag1=false;
						break;
					}
				}

				bool tempFlag2 = true;
				for(int n=0;n<120;++n)
				{
					if(WhiteZX!=GetPixel(hdcZX,MonsterHPLeft-1+n,MonsterHPUp-2)||WhiteZX!=GetPixel(hdcZX,MonsterHPLeft-1+n,MonsterHPDown+1))
					{
						tempFlag1=false;
						break;
					}
				}

				bool tempFlag3 = true;
				for(int n=0;n<8;++n)
				{
					if(MonsterHPColor[n]!=GetPixel(hdcZX,MonsterHPLeft-1,MonsterHPUp+n))
					{
						tempFlag3=false;
						break;
					}
				}
					
				bool tempFlag4 = true;
				for(int n=0;n<118;++n)
				{
					if(BlackZX!=GetPixel(hdcZX,MonsterHPLeft+n,MonsterHPUp-1))
					{
						tempFlag4=false;
						break;
					}
				}

				flag = tempFlag1&&tempFlag2&&tempFlag3&&tempFlag4;

			}
			break;
		default:
			flag=false;
		}
		ReleaseDC(hZX,hdcZX);
		return flag;
	}

	static bool isMonsterFullHP(HWND hZX,MonsterRecognitionAccuracy mra)
	{	
		bool flag = false;
		HDC hdcZX=::GetDC(hZX);
		switch(mra)
		{
		case LOWEST:
			{
				flag = GetPixel(hdcZX,MonsterHPRight,MonsterHPUp)==MonsterHPColor[0]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPDown)==MonsterHPColor[7];

			}
			break;
		case LOW:
			{
				flag = GetPixel(hdcZX,MonsterHPRight,MonsterHPUp)==MonsterHPColor[0]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+2)==MonsterHPColor[2]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+5)==MonsterHPColor[5]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+7)==MonsterHPColor[7];
			}
			break;
		case MIDDLE:
			{
				flag = GetPixel(hdcZX,MonsterHPRight,MonsterHPUp)==MonsterHPColor[0]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+1)==MonsterHPColor[1]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+3)==MonsterHPColor[3]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+4)==MonsterHPColor[4]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+6)==MonsterHPColor[6]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+7)==MonsterHPColor[7];
			}
			break;
		case HIGH:
			{
				flag = GetPixel(hdcZX,MonsterHPRight,MonsterHPUp)==MonsterHPColor[0]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+1)==MonsterHPColor[1]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+2)==MonsterHPColor[2]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+3)==MonsterHPColor[3]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+4)==MonsterHPColor[4]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+5)==MonsterHPColor[5]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+6)==MonsterHPColor[6]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+7)==MonsterHPColor[7];
			}
			break;
		case HIGHEST:
			{
				flag = GetPixel(hdcZX,MonsterHPRight,MonsterHPUp)==MonsterHPColor[0]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+1)==MonsterHPColor[1]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+2)==MonsterHPColor[2]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+3)==MonsterHPColor[3]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+4)==MonsterHPColor[4]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+5)==MonsterHPColor[5]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+6)==MonsterHPColor[6]
					&& GetPixel(hdcZX,MonsterHPRight,MonsterHPUp+7)==MonsterHPColor[7]
					&& GetPixel(hdcZX,MonsterHPRight-1,MonsterHPUp)==MonsterHPColor[0]
					&& GetPixel(hdcZX,MonsterHPRight-2,MonsterHPUp+1)==MonsterHPColor[1]
					&& GetPixel(hdcZX,MonsterHPRight-3,MonsterHPUp+2)==MonsterHPColor[2]
					&& GetPixel(hdcZX,MonsterHPRight-4,MonsterHPUp+3)==MonsterHPColor[3]
					&& GetPixel(hdcZX,MonsterHPRight-5,MonsterHPUp+4)==MonsterHPColor[4]
					&& GetPixel(hdcZX,MonsterHPRight-6,MonsterHPUp+5)==MonsterHPColor[5]
					&& GetPixel(hdcZX,MonsterHPRight-7,MonsterHPUp+6)==MonsterHPColor[6]
					&& GetPixel(hdcZX,MonsterHPRight-8,MonsterHPUp+7)==MonsterHPColor[7];
			}
			break;
		default:
			flag=false;
		}
		ReleaseDC(hZX,hdcZX);
		return flag;
	}

	static bool willMonsterDie(HWND hZX,MonsterRecognitionAccuracy mra)
	{	
		bool flag = false;
		HDC hdcZX=::GetDC(hZX);
		switch(mra)
		{
		case LOWEST:
			{
				flag = BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPDown);
			}
			break;
		case LOW:
			{
				flag = BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+2)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+4)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+6);
			}
			break;
		case MIDDLE:
			{
				flag = BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+1)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+2)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+3)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+4)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+5)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+6);
			}
			break;
		case HIGH:
			{
				flag = BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+1)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+2)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+3)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+4)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+5)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+6)
					&& BlackZX==GetPixel(hdcZX,MonsterHPLeft,MonsterHPUp+7);
			}
			break;
		case HIGHEST:
			{
				bool tempFlag=true;
				for(int y=0;y<11;y++)
				{
					for(int x=0;x<2;x++)
					{
						if(GetPixel(hdcZX,MonsterHPLeft-1+x,MonsterHPUp-2+y) != ColorOfMWD[y][x])
						{
							tempFlag = false;
							break;
						}
					}
					if(!tempFlag) break;
				}
				flag = tempFlag;
			}
			break;
		default:
			flag=false;
		}
		ReleaseDC(hZX,hdcZX);
		return flag;
	}
	static bool isMonsterDead(HWND hZX,MonsterRecognitionAccuracy mra)
	{
		if(willMonsterDie(hZX,mra))	
			return false;
		else 
			return !isSeeingMonster(hZX,mra);
	}

	static bool isMonster(const MonsterNamePicture &mp,HWND hZX,MonsterRecognitionAccuracy mra)
	{
		bool flag = false;
		HDC hdcZX=::GetDC(hZX);
		switch(mra)
		{
		case LOWEST:
			{
				int NowNumOfDMNPPS=0;
				for(int y=0;y<MonsterNameWidth;++y)
					for(int x=0;x<MonsterNameLength;++x)
						if(GetPixel(hdcZX,MonsterNameLeft+x,MonsterNameUp+y)!=BackColorForMN)
							++NowNumOfDMNPPS;
				flag=NowNumOfDMNPPS==mp.getNumOfDMNPPS();
			}
			break;
		case LOW:
			{
				bool tempFlag=true;
				for(int y=2;y<11;y=y+4)
				{
					int x;
					for(x=44;x<MonsterNameLength-44;++x)
					{
						if(GetPixel(hdcZX,MonsterNameLeft+x,MonsterNameUp+y)!=mp.getMNPPixel(x,y))
						{
							tempFlag=false;
							break;
						}
					}
					//if(x!=MonsterNameLength-44) break;
					if(!tempFlag) break;
				}
				flag=tempFlag;
			}
			break;
		case MIDDLE:
			{
				bool tempFlag=true;
				for(int y=1;y<MonsterNameWidth;y=y+2)
				{
					int x;
					for(x=29;x<MonsterNameLength-29;++x)
					{
						if(GetPixel(hdcZX,MonsterNameLeft+x,MonsterNameUp+y)!=mp.getMNPPixel(x,y))
						{
							tempFlag=false;
							break;
						}
					}
					//if(x!=MonsterNameLength-29) break;
					if(!tempFlag) break;
				}
				flag=tempFlag;
			}
			break;
		case HIGH:
			{
				bool tempFlag=true;
				for(int y=1;y<11;++y)
				{
					int x;
					for(x=14;x<MonsterNameLength-14;++x)
					{
						if(GetPixel(hdcZX,MonsterNameLeft+x,MonsterNameUp+y)!=mp.getMNPPixel(x,y))
						{
							tempFlag=false;
							break;
						}
					}
					//if(x!=MonsterNameLength-14) break;
					if(!tempFlag) break;
				}
				flag=tempFlag;
			}
			break;
		case HIGHEST:
			{
				bool tempFlag=true;
				for(int y=0;y<MonsterNameWidth;++y)
				{
					int x;
					for(x=0;x<MonsterNameLength;++x)
					{
						if(GetPixel(hdcZX,MonsterNameLeft+x,MonsterNameUp+y)!=mp.getMNPPixel(x,y))
						{
							tempFlag=false;
							break;
						}
					}
					//if(x!=MonsterNameLength1) break;
					if(!tempFlag) break;
				}
				flag=tempFlag;
			}
			break;
		default:
			flag=false;
		}
		ReleaseDC(hZX,hdcZX);
		return flag;
	}
private:
	static const int MonsterNameLeft=431;
	static const int MonsterNameUp=30;
	static const int MonsterNameLength=118;
	static const int MonsterNameWidth=14;
	static const int MonsterNameRight=MonsterNameLeft+MonsterNameLength-1;
	static const int MonsterNameDown=MonsterNameUp+MonsterNameWidth-1;
	static const int MonsterHPLeft=431;
	static const int MonsterHPUp=53;
	static const int MonsterHPLength=118;
	static const int MonsterHPWidth=8;
	static const int MonsterHPRight=MonsterHPLeft+MonsterHPLength-1;
	static const int MonsterHPDown=MonsterHPUp+MonsterHPWidth-1;
};