#include "stdafx.h"
#include <vector>

enum MonsterGrade{easiest=1,easier,easy,normal,hard,harder,hardest};//怪物等级：灰色，绿色，浅绿，白色，浅红，小红，大红
/*
typedef struct _MPPixel
{
	int _x;
	int _y;
	long _color;
}MPPixel;

class MonsterPattern
{
public:
	MonsterPattern(LPCWSTR fileName,MonsterGrade mg)
	{
		pMonsterName=fileName;
		MG=mg;
		wchar_t fullFileName[50];
		LPWSTR pFullFileName=fullFileName;
		switch(mg)
		{
		case easiest:
			lstrcpy(fullFileName,L"easiestMPPic\\");
			break;
		case easier:
			lstrcpy(fullFileName,L"easierMPPic\\");
			break;
		case easy:
			lstrcpy(fullFileName,L"easyMPPic\\");
			break;
		case normal:
			lstrcpy(fullFileName,L"normalMPPic\\");
			break;
		case hard:
			lstrcpy(fullFileName,L"hardMPPic\\");
			break;
		case harder:
			lstrcpy(fullFileName,L"harderMPPic\\");
			break;
		case hardest:
			lstrcpy(fullFileName,L"hardestMPPic\\");
			break;
		}
		lstrcat(pFullFileName,fileName);
		lstrcat(pFullFileName,L".bmp");
		OutputDebugString(pFullFileName);
		OutputDebugString(L"\n");
		HBITMAP hMP=(HBITMAP)LoadImage(NULL,pFullFileName,IMAGE_BITMAP,length,width,LR_LOADFROMFILE);
		BYTE pBits[length][width][depth];
		LONG l;
		l=GetBitmapBits(hMP,length*width*depth,pBits);
		for(int y=0;y<width;y++)
		{
			for(int x=0;x<length;x++)
			{
				if(pBits[x][y][0]==12&&pBits[x][y][1]==14&&pBits[x][y][2]==16);
				else 
				{
					MPPixel tempMPP;
					tempMPP._x=x;
					tempMPP._y=y;
					long left=pBits[x][y][0]<<16;
					long center=pBits[x][y][1]<<8;
					long right=pBits[x][y][2];
					tempMPP._color=left+center+right;
					vMPPs.push_back(tempMPP);
				}
			}
		}
	}

	long getNumOfMPPS()
	{
		return (long)vMPPs.size();
	}

	MonsterGrade getMonsterGrade()
	{
		return MG;
	}
private:
	static const int length=118;
	static const int width=14;
	static const int depth=4;
	LPCWSTR pMonsterName;
	MonsterGrade MG;
	std::vector<MPPixel> vMPPs;
};
*/
class MonsterNamePicture
{
public:
	MonsterNamePicture(LPCWSTR fileName,MonsterGrade mg)
	{		
		pMonsterName=fileName;
		MG=mg;
		wchar_t fullFileName[50];
		LPWSTR pFullFileName=fullFileName;
		switch(mg)
		{
		case easiest:
			lstrcpy(fullFileName,L"easiestMPPic\\");
			break;
		case easier:
			lstrcpy(fullFileName,L"easierMPPic\\");
			break;
		case easy:
			lstrcpy(fullFileName,L"easyMPPic\\");
			break;
		case normal:
			lstrcpy(fullFileName,L"normalMPPic\\");
			break;
		case hard:
			lstrcpy(fullFileName,L"hardMPPic\\");
			break;
		case harder:
			lstrcpy(fullFileName,L"harderMPPic\\");
			break;
		case hardest:
			lstrcpy(fullFileName,L"hardestMPPic\\");
			break;
		}
		lstrcat(pFullFileName,fileName);
		lstrcat(pFullFileName,L".bmp");
		OutputDebugString(pFullFileName);
		OutputDebugString(L"\n");
		HBITMAP hMP=(HBITMAP)LoadImage(NULL,pFullFileName,IMAGE_BITMAP,length,width,LR_LOADFROMFILE);
		BYTE pBits[width][length][depth];
		LONG l;
		l=GetBitmapBits(hMP,length*width*depth,pBits);
		NumOfDMNPPS=0;
		for(int y=0;y<length;y++)
		{
			for(int x=0;x<width;x++)
			{
				if(pBits[x][y][0]!=12||pBits[x][y][1]!=14||pBits[x][y][2]!=16)
					++NumOfDMNPPS;
				long left=pBits[x][y][0]<<16;
				long center=pBits[x][y][1]<<8;
				long right=pBits[x][y][2];
				aMNPPs[y][x]=left+center+right;
			}
		}
	}
	
	int getNumOfDMNPPS() const
	{
		return NumOfDMNPPS;
	}

	MonsterGrade getMonsterGrade()
	{
		return MG;
	}

	COLORREF getMNPPixel(int x,int y) const
	{
		if((x>=0&&x<length)&&(y>=0&&y<width))
			return aMNPPs[x][y];
		else
			return 0x0;
	}
private:
	static const int length=118;
	static const int width=14;
	static const int depth=4;
	LPCWSTR pMonsterName;
	MonsterGrade MG;
	COLORREF aMNPPs[length][width];
	int NumOfDMNPPS;
};