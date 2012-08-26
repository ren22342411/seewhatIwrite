#include <windows.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
#include "resource.h"
#include "block.h"

#define SB_H 30
#define SB_W 30
#define nBB_H 13
#define nBB_W 6
#define nBB_LH 10
#define InitialPos_X 2
#define InitialPos_Y 10
#define Initial_X 60
#define Initial_Y 300
#define NextMB_X 240
#define NextMB_Y 210
#define xB2C(x) ((x)+25)
#define yB2C(y) (295-(y))

int MyRandom()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	return ((st.wDay+st.wDayOfWeek+st.wHour+st.wMinute+st.wMonth+st.wSecond+st.wYear+st.wMilliseconds)%rand());
}


BOOL
	DrawSBState(
	HDC hdc,
	HBRUSH hbrFore,
	DRAWSTATEPROC qfnCallBack,
	LPARAM lData,
	WPARAM wData,
	int x,
	int y,
	int cx,
	int cy,
	UINT uFlags)
{

	WCHAR trace[32] = {};

	swprintf(trace,32, L"x=%d, y=%d, cx=%d, cy=%d \n",      x,
		y,
		cx,
		cy);

	OutputDebugString(trace);

	if(y < 25)
	{
		HDC hDCBits = CreateCompatibleDC(hdc);

		SelectObject(hDCBits, (HBITMAP)lData);

		BOOL bResult = BitBlt(hdc, x, 25, cx, y + 5, hDCBits, 0, cy - ( y + 5 ), SRCCOPY);

		DeleteDC(hDCBits);

		return bResult;
	}
	else{

		return DrawState(hdc, hbrFore, qfnCallBack, lData, wData, x, y, cx, cy, uFlags);
	}
}


SingleBlock::SingleBlock(SingleBlock &rhs)
{
	state=rhs.state;
	position.x=rhs.position.x;
	position.y=rhs.position.y;
	order=rhs.order;

}

SingleBlock::SingleBlock(int x,int y)
{
	state=SB_BLANK;
	position.x=x;
	position.y=y;
	order=0;
}

SingleBlock::SingleBlock(SBState sbstate,int x,int y)
{
	state=sbstate;
	position.x=x;
	position.y=y;
	order=0;
}

SBState SingleBlock::GetState()
{
	return state;
}

SBPosition SingleBlock::GetPosition()
{
	return position;
}

int SingleBlock::GetOrder()
{
	return order;
}

void SingleBlock::PutState( SBState st )
{
	state=st;
}

void SingleBlock::PutPosition(SBPosition &pos)
{
	position.x=pos.x;
	position.y=pos.y;
}

void SingleBlock::PutPosition(int x,int y)
{
	position.x=x;
	position.y=y;
}

void SingleBlock::SetOrder(int i)
{
	order=i;
}

bool SingleBlock::isSBEmpty()
{
	if(!state) return true;
	else return false;
}

void SingleBlock::makeSBEmpty()
{
	state=SB_BLANK;
}

SingleBlock SingleBlock::operator=(SingleBlock &rhs)
{
	if(this != &rhs)
	{
		state=rhs.state;
		position.x=rhs.position.x;
		position.y=rhs.position.y;
		order=rhs.order;
	}
	return *this;
}

MultiBlock::MultiBlock()
{}

void MultiBlock::MBInitial( MBKind mbk )
{
	kind=mbk;
	switch(kind)
	{
	case MB_2H:
		numSB=2;
		left=-SB_W;
		right=SB_W;
		high=SB_H;
		break;
	case MB_2V:
		numSB=2;
		left=0;
		right=SB_W;
		high=2*SB_H;
		break;
	case MB_3L:
		numSB=3;
		left=-SB_W;
		right=SB_W;
		high=2*SB_H;
		break;
	case MB_3R:
		numSB=3;
		left=0;
		right=2*SB_W;
		high=2*SB_H;
		break;
	case MB_3H:
		numSB=3;
		left=-SB_W;
		right=2*SB_W;
		high=SB_H;
		break;
	case MB_3V:
		numSB=3;
		left=0;
		right=SB_W;
		high=3*SB_H;
		break;
	case MB_1:
		numSB=1;
		left=0;
		right=SB_W;
		high=SB_H;
		break;
	default:
		break;
	}
	for(int i=0;i<numSB;i++)
	{
		SingleBlock *temp=new SingleBlock;
		MBstate[i]=*temp;
		MBstate[i].SetOrder(i+1);
		delete temp;
	}
	switch(kind)
	{
	case MB_2H:
		MBstate[0].PutPosition(InitialPos_X,InitialPos_Y);
		MBstate[1].PutPosition(InitialPos_X-1,InitialPos_Y);
		break;
	case MB_2V:
		MBstate[0].PutPosition(InitialPos_X,InitialPos_Y);
		MBstate[1].PutPosition(InitialPos_X,InitialPos_Y+1);
		break;
	case MB_3L:
		MBstate[0].PutPosition(InitialPos_X,InitialPos_Y);
		MBstate[1].PutPosition(InitialPos_X,InitialPos_Y+1);
		MBstate[2].PutPosition(InitialPos_X-1,InitialPos_Y);
		break;
	case MB_3R:
		MBstate[0].PutPosition(InitialPos_X,InitialPos_Y);
		MBstate[1].PutPosition(InitialPos_X,InitialPos_Y+1);
		MBstate[2].PutPosition(InitialPos_X+1,InitialPos_Y);
		break;
	case MB_3H:
		MBstate[0].PutPosition(InitialPos_X,InitialPos_Y);
		MBstate[1].PutPosition(InitialPos_X-1,InitialPos_Y);
		MBstate[2].PutPosition(InitialPos_X+1,InitialPos_Y);
		break;
	case MB_3V:
		MBstate[0].PutPosition(InitialPos_X,InitialPos_Y);
		MBstate[1].PutPosition(InitialPos_X,InitialPos_Y+1);
		MBstate[2].PutPosition(InitialPos_X,InitialPos_Y+2);
		break;
	case MB_1:
		MBstate[0].PutPosition(InitialPos_X,InitialPos_Y);
		break;
	default:
		break;
	}
	Active_Pt.x=Initial_X;
	Active_Pt.y=Initial_Y;	
}

void MultiBlock::Transposition()
{
	switch(numSB)
	{
	case 2:	
		SBState temp2_SBState;
		//			SBPosition temp2_SBPosition;
		temp2_SBState=MBstate[0].GetState();
		//temp2_SBPosition.x=MBstate[0].GetPosition().x;
		//temp2_SBPosition.y=MBstate[0].GetPosition().y;
		MBstate[0].PutState(MBstate[1].GetState());
		//MBstate[0].PutPosition(MBstate[1].GetPosition().x,MBstate[1].GetPosition().y);
		MBstate[1].PutState(temp2_SBState);
		//MBstate[1].PutPosition(temp2_SBPosition.x,temp2_SBPosition.y);

		break;
	case 3:
		SBState temp3_SBState;
		//			SBPosition temp3_SBPosition;
		temp3_SBState=MBstate[0].GetState();
		//temp3_SBPosition.x=MBstate[0].GetPosition().x;
		//temp3_SBPosition.y=MBstate[0].GetPosition().y;
		MBstate[0].PutState(MBstate[1].GetState());
		//MBstate[0].PutPosition(MBstate[1].GetPosition().x,MBstate[1].GetPosition().y);
		MBstate[1].PutState(MBstate[2].GetState());
		//MBstate[1].PutPosition(MBstate[2].GetPosition().x,MBstate[2].GetPosition().y);			
		MBstate[2].PutState(temp3_SBState);
		//MBstate[2].PutPosition(temp3_SBPosition.x,temp3_SBPosition.y);
		break;
	default:
		break;
	}
}

MBKind MultiBlock::GetKind()
{
	return kind;
}

int MultiBlock::GetNumSB()
{
	return numSB;
}

int MultiBlock::GetLeft()
{
	return left;
}

int MultiBlock::GetRight()
{
	return right;
}

int MultiBlock::GetHigh()
{
	return high;
}

MultiBlock MultiBlock::operator=(MultiBlock &rhs)
{
	if(this != &rhs)
	{
		kind=rhs.kind;
		numSB=rhs.numSB;
		left=rhs.left;
		right=rhs.right;
		high=rhs.high;
		Active_Pt=rhs.Active_Pt;
		for(int i=0;i<max_numSB;i++)
		{
			MBstate[i]=rhs.MBstate[i];
		}
	}
	return *this;
}


BlockBoard::BlockBoard()
{
	for( int i=0;i<n_width;i++)
		for(int j=0;j<n_height;j++)
		{
			SingleBlock *temp=new SingleBlock(i,j);
			column[i][j]=*temp;
			delete temp;
		}
		nMB=-2;
		Score=0;
		Level=1;
		MaxErasingTimes=0;
		dead_flag=true;
}

SBState BlockBoard::GetSBState( SBPosition pos )
{
	return column[pos.x][pos.y].GetState();
}

SBState BlockBoard::GetSBState( int x,int y)
{
	return column[x][y].GetState();
}

void BlockBoard::GenerateMB(MultiBlock &MB)
{
	MBKind mbk;
	long random,module=3000;
	random=(rand()+MyRandom())%module;
	if( random >= 0 && random <300 ) mbk=MB_1;
	if( random >= 300 && random <750) mbk=MB_2H;
	if( random >= 750 && random < 1200 ) mbk=MB_2V;
	if( random >= 1200 && random < 1650 ) mbk=MB_3L;
	if( random >= 1650 && random < 2100 ) mbk=MB_3R;
	if( random >= 2100 && random < 2550 ) mbk=MB_3H;
	if( random >= 2550 && random < 3000 ) mbk=MB_3V;
	MB.MBInitial(mbk);
	if( random >= 0 && random <100 ) MB.MBstate[0].PutState(SB_BOMB);
	if( random >= 100 && random <200 ) MB.MBstate[0].PutState(SB_LEVIN);
	if( random >= 200 && random <300 ) MB.MBstate[0].PutState(SB_PENTACLE);
	if( random >= 300 && random <700)
		for(int i=0;i<2;i++)
			MB.MBstate[i].PutState((SBState)(1+(rand()+MyRandom())%5));
	if( random >= 700 && random <750)
		for(int i=0;i<2;i++)
			MB.MBstate[i].PutState(SB_STONE);
	if( random >= 750 && random <1150)
		for(int i=0;i<2;i++)
			MB.MBstate[i].PutState((SBState)(1+(rand()+MyRandom())%5));
	if( random >= 1150 && random <1200)
		for(int i=0;i<2;i++)
			MB.MBstate[i].PutState(SB_STONE);
	if( random >= 1200 && random <1600)
		for(int i=0;i<3;i++)
			MB.MBstate[i].PutState((SBState)(1+(rand()+MyRandom())%5));
	if( random >= 1600 && random <1650)
		for(int i=0;i<3;i++)
			MB.MBstate[i].PutState(SB_STONE);
	if( random >= 1650 && random <2050)
		for(int i=0;i<3;i++)
			MB.MBstate[i].PutState((SBState)(1+(rand()+MyRandom())%5));
	if( random >= 2050 && random <2100)
		for(int i=0;i<3;i++)
			MB.MBstate[i].PutState(SB_STONE);		
	if( random >= 2100 && random <2500)
		for(int i=0;i<3;i++)
			MB.MBstate[i].PutState((SBState)(1+(rand()+MyRandom())%5));
	if( random >= 2500 && random <2550)
		for(int i=0;i<3;i++)
			MB.MBstate[i].PutState(SB_STONE);		
	if( random >= 2550 && random <2950)
		for(int i=0;i<3;i++)
			MB.MBstate[i].PutState((SBState)(1+(rand()+MyRandom())%5));
	if( random >= 2950 && random <3000)
		for(int i=0;i<3;i++)
			MB.MBstate[i].PutState(SB_STONE);
	dead_flag=false;
	if(ErasingTimes>MaxErasingTimes) MaxErasingTimes=ErasingTimes;
	ErasingTimes=0;
	Active_Pt=MB.Active_Pt;	
	nMB=nMB+1;
	next_step=-1;
}

void BlockBoard::MBMoveLeft()
{
	if(CurrentMB.MBstate[0].GetPosition().x+CurrentMB.GetLeft()/SB_W > 0 && column[CurrentMB.MBstate[0].GetPosition().x+CurrentMB.GetLeft()/SB_W-1][CurrentMB.MBstate[0].GetPosition().y].isSBEmpty()) 
	{
		for(int i=0;i<CurrentMB.GetNumSB();i++)
			CurrentMB.MBstate[i].PutPosition(CurrentMB.MBstate[i].GetPosition().x-1,CurrentMB.MBstate[i].GetPosition().y);
		CurrentMB.Active_Pt.x-=30;
		Active_Pt=CurrentMB.Active_Pt;
	}
}

void BlockBoard::MBMoveRight()//注意右值至少就有30
{
	if(CurrentMB.MBstate[0].GetPosition().x+CurrentMB.GetRight()/SB_W - 1 < 5 && column[CurrentMB.MBstate[0].GetPosition().x+CurrentMB.GetRight()/SB_W][CurrentMB.MBstate[0].GetPosition().y].isSBEmpty()) 
	{
		for(int i=0;i<CurrentMB.GetNumSB();i++)
			CurrentMB.MBstate[i].PutPosition(CurrentMB.MBstate[i].GetPosition().x+1,CurrentMB.MBstate[i].GetPosition().y);
		CurrentMB.Active_Pt.x+=30;
		Active_Pt=CurrentMB.Active_Pt;
	}
}

void BlockBoard::MBTransposition()
{
	CurrentMB.Transposition();
}

void BlockBoard::MBFallDown()
{
	for(int i=0;i<CurrentMB.GetNumSB();i++)
	{	
		int j;
		for(j=0;j<=CurrentMB.MBstate[i].GetPosition().y;j++)
			if(column[CurrentMB.MBstate[i].GetPosition().x][j].isSBEmpty()) break;
		column[CurrentMB.MBstate[i].GetPosition().x][j].PutState(CurrentMB.MBstate[i].GetState());	
		CurrentMB.MBstate[i].PutState(SB_BLANK);
		//CurrentMB.MBstate[i].PutPosition(CurrentMB.MBstate[i].GetPosition().x,j);////////////////////////////////////////////////////////////
		column[CurrentMB.MBstate[i].GetPosition().x][j].PutPosition(CurrentMB.MBstate[i].GetPosition().x,j);
		if( i == 0 ) 
		{
			DeadPosition.x=CurrentMB.MBstate[i].GetPosition().x;
			DeadPosition.y=j;
			DeadState=column[CurrentMB.MBstate[i].GetPosition().x][j].GetState();
		}
	}	
	dead_flag=true;
}

void BlockBoard::MBFallStep()
{
	if( next_step < SB_H && next_step > -1 ) 
	{
		next_step--;
		CurrentMB.Active_Pt.y=CurrentMB.Active_Pt.y-1;
		Active_Pt=CurrentMB.Active_Pt;
	}
	else if( next_step == -1 ) 
	{
		next_step=SB_H-1;
		CurrentMB.Active_Pt.y=CurrentMB.Active_Pt.y-1;
		Active_Pt=CurrentMB.Active_Pt;
		for(int i=0;i<CurrentMB.GetNumSB();i++)
			CurrentMB.MBstate[i].PutPosition(CurrentMB.MBstate[i].GetPosition().x,CurrentMB.MBstate[i].GetPosition().y-1);
	}
}

bool BlockBoard::WillMBDead()
{
	switch(CurrentMB.GetKind())
	{	
	case MB_1:
		if( WillSBDead(0) ) return true;
		else return false;
		break;
	case MB_2H:
		if( WillSBDead(0) || WillSBDead(1) ) return true;
		else return false;
		break;
	case MB_2V:
		if( WillSBDead(0) ) return true;
		else return false;
		break;
	case MB_3L:
		if( WillSBDead(0) || WillSBDead(2)) return true;
		else return false;
		break;
	case MB_3R:
		if( WillSBDead(0) || WillSBDead(2)) return true;
		else return false;
		break;
	case MB_3H:
		if( WillSBDead(0) || WillSBDead(1) || WillSBDead(2)) return true;
		else return false;
		break;
	case MB_3V:
		if( WillSBDead(0) ) return true;
		else return false;
		break;
	default:
		return false;
		break;
	}
}

SBState BlockBoard::ReceiveWhat()
{
	return column[DeadPosition.x][DeadPosition.y].GetState();
}

SBState BlockBoard::ReceiveWhat( int i)
{
	return DeadState;
}

void BlockBoard::Find4NormalErasing()
{
	for( int i=1;i<n_width-1;i++)
		for(int j=1;j<n_height-1;j++)
		{ 
			if(!column[i][j].isSBEmpty() && column[i][j].GetState()!=SB_STONE)
			{
				if( column[i-1][j].GetState()==column[i][j].GetState() && column[i+1][j].GetState()==column[i][j].GetState() ) 
				{
					SBPosition tem=column[i][j].GetPosition();
					{
						bool flag = true;
						for(int n=0;n < (int)ErasingVec.size();n++)
							if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
							{
								flag=false;
								break;
							}
							if(flag) ErasingVec.push_back(tem);
					}
					tem=column[i-1][j].GetPosition();
					{
						bool flag = true;
						for(int n=0;n < (int)ErasingVec.size();n++)
							if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
							{
								flag=false;
								break;
							}
							if(flag) ErasingVec.push_back(tem);
					}
					tem=column[i+1][j].GetPosition();
					{
						bool flag = true;
						for(int n=0;n < (int)ErasingVec.size();n++)
							if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
							{
								flag=false;
								break;
							}
							if(flag) ErasingVec.push_back(tem);
					}
				}
				if( column[i][j-1].GetState()==column[i][j].GetState() && column[i][j+1].GetState()==column[i][j].GetState() ) 
				{
					SBPosition tem=column[i][j].GetPosition();
					{
						bool flag = true;
						for(int n=0;n < (int)ErasingVec.size();n++)
							if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
							{
								flag=false;
								break;
							}
							if(flag) ErasingVec.push_back(tem);
					}
					tem=column[i][j-1].GetPosition();
					{
						bool flag = true;
						for(int n=0;n < (int)ErasingVec.size();n++)
							if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
							{
								flag=false;
								break;
							}
							if(flag) ErasingVec.push_back(tem);
					}
					tem=column[i][j+1].GetPosition();
					{
						bool flag = true;
						for(int n=0;n < (int)ErasingVec.size();n++)
							if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
							{
								flag=false;
								break;
							}
							if(flag) ErasingVec.push_back(tem);
					}
				}
				if( column[i-1][j-1].GetState()==column[i][j].GetState() && column[i+1][j+1].GetState()==column[i][j].GetState() ) 
				{
					SBPosition tem=column[i][j].GetPosition();
					{
						bool flag = true;
						for(int n=0;n < (int)ErasingVec.size();n++)
							if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
							{
								flag=false;
								break;
							}
							if(flag) ErasingVec.push_back(tem);
					}
					tem=column[i-1][j-1].GetPosition();
					{
						bool flag = true;
						for(int n=0;n < (int)ErasingVec.size();n++)
							if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
							{
								flag=false;
								break;
							}
							if(flag) ErasingVec.push_back(tem);
					}
					tem=column[i+1][j+1].GetPosition();
					{
						bool flag = true;
						for(int n=0;n < (int)ErasingVec.size();n++)
							if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
							{
								flag=false;
								break;
							}
							if(flag) ErasingVec.push_back(tem);
					}
				}
				if( column[i-1][j+1].GetState()==column[i][j].GetState() && column[i+1][j-1].GetState()==column[i][j].GetState() ) 
				{
					SBPosition tem=column[i][j].GetPosition();
					{
						bool flag = true;
						for(int n=0;n < (int)ErasingVec.size();n++)
							if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
							{
								flag=false;
								break;
							}
							if(flag) ErasingVec.push_back(tem);
					}
					tem=column[i-1][j+1].GetPosition();
					{
						bool flag = true;
						for(int n=0;n < (int)ErasingVec.size();n++)
							if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
							{
								flag=false;
								break;
							}
							if(flag) ErasingVec.push_back(tem);
					}
					tem=column[i+1][j-1].GetPosition();
					{
						bool flag = true;
						for(int n=0;n < (int)ErasingVec.size();n++)
							if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
							{
								flag=false;
								break;
							}
							if(flag) ErasingVec.push_back(tem);
					}
				}
			}
		}
		for(int k=0;k<2;k++)
		{	int i;
		if(k==0) i=0;
		if(k==1) i=n_width-1;
		for(int j=1;j<n_height-1;j++)
		{
			if(!column[i][j].isSBEmpty() && column[i][j].GetState()!=SB_STONE)
			{
				if( column[i][j-1].GetState()==column[i][j].GetState() && column[i][j+1].GetState()==column[i][j].GetState() ) 
				{
					SBPosition tem=column[i][j].GetPosition();
					{
						bool flag = true;
						for(int n=0;n < (int)ErasingVec.size();n++)
							if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
							{
								flag=false;
								break;
							}
							if(flag) ErasingVec.push_back(tem);
					}
					tem=column[i][j-1].GetPosition();
					{
						bool flag = true;
						for(int n=0;n < (int)ErasingVec.size();n++)
							if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
							{
								flag=false;
								break;
							}
							if(flag) ErasingVec.push_back(tem);
					}
					tem=column[i][j+1].GetPosition();
					{
						bool flag = true;
						for(int n=0;n < (int)ErasingVec.size();n++)
							if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
							{
								flag=false;
								break;
							}
							if(flag) ErasingVec.push_back(tem);
					}
				}
			}
		}
		}
		for(int i=1;i<n_width-1;i++)
		{	
			for(int k=0;k<2;k++)
			{
				int j;
				if(k==0) j=0;
				if(k==1) j=n_height-1;
				if(!column[i][j].isSBEmpty() && column[i][j].GetState()!=SB_STONE)
				{
					if( column[i-1][j].GetState()==column[i][j].GetState() && column[i+1][j].GetState()==column[i][j].GetState() ) 
					{
						SBPosition tem=column[i][j].GetPosition();
						{
							bool flag = true;
							for(int n=0;n < (int)ErasingVec.size();n++)
								if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
								{
									flag=false;
									break;
								}
								if(flag) ErasingVec.push_back(tem);
						}
						tem=column[i-1][j].GetPosition();
						{
							bool flag = true;
							for(int n=0;n < (int)ErasingVec.size();n++)
								if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
								{
									flag=false;
									break;
								}
								if(flag) ErasingVec.push_back(tem);
						}
						tem=column[i+1][j].GetPosition();
						{
							bool flag = true;
							for(int n=0;n < (int)ErasingVec.size();n++)
								if(ErasingVec[n].x==tem.x && ErasingVec[n].y==tem.y) 
								{
									flag=false;
									break;
								}
								if(flag) ErasingVec.push_back(tem);
						}
					}
				}
			}
		}
}

void BlockBoard::Find4BOMBErasing(SBPosition &pos)
{
	if(pos.x>0&&pos.x<nBB_W-1&&pos.y>0)
		for(int i=pos.x-1;i<=pos.x+1;i++)
			for(int j=pos.y-1;j<=pos.y+1;j++)
			{
				SBPosition tem;
				tem.x=i;
				tem.y=j;
				if(GetSBState(tem)) ErasingVec.push_back(tem);
			}
			if(pos.x==0 && pos.y==0 )
				for(int i=pos.x;i<=pos.x+1;i++)
					for(int j=pos.y;j<=pos.y+1;j++)
					{
						SBPosition tem;
						tem.x=i;
						tem.y=j;
						if(GetSBState(tem)) ErasingVec.push_back(tem);
					}
					if(pos.x==nBB_W-1 && pos.y==0 )
						for(int i=pos.x-1;i<=pos.x;i++)
							for(int j=pos.y;j<=pos.y+1;j++)
							{
								SBPosition tem;
								tem.x=i;
								tem.y=j;
								if(GetSBState(tem)) ErasingVec.push_back(tem);
							}
							if(pos.x==0 && pos.y>0)
								for(int i=pos.x;i<=pos.x+1;i++)
									for(int j=pos.y-1;j<=pos.y+1;j++)
									{
										SBPosition tem;
										tem.x=i;
										tem.y=j;
										if(GetSBState(tem)) ErasingVec.push_back(tem);
									}
									if(pos.x==nBB_W-1 && pos.y>0)
										for(int i=pos.x-1;i<=pos.x;i++)
											for(int j=pos.y-1;j<=pos.y+1;j++)
											{
												SBPosition tem;
												tem.x=i;
												tem.y=j;
												if(GetSBState(tem)) ErasingVec.push_back(tem);
											}
											if(pos.x>0 && pos.x < nBB_W && pos.y==0)
												for(int i=pos.x-1;i<=pos.x+1;i++)
													for(int j=pos.y;j<=pos.y+1;j++)
													{
														SBPosition tem;
														tem.x=i;
														tem.y=j;
														if(GetSBState(tem)) ErasingVec.push_back(tem);
													}

}

void BlockBoard::Find4LEVINErasing(SBPosition &pos)
{
	for(int n=0;n<=pos.y;n++)
	{
		SBPosition tem;
		tem.x=pos.x;
		tem.y=n;
		if(GetSBState(tem)) ErasingVec.push_back(tem);
	}
}

void BlockBoard::Find4PENTACLEErasing(SBPosition &pos)
{
	ErasingVec.push_back(pos);
	SBState sbstate;
	if(pos.y>0) {sbstate=GetSBState(pos.x,pos.y-1);
	for( int i=0;i<n_width;i++)
		for(int j=0;j<n_height;j++)
		{
			if(column[i][j].GetState()==sbstate) 
			{
				SBPosition tem;
				tem.x=i;
				tem.y=j;
				ErasingVec.push_back(tem);
			}
		}}
}

void BlockBoard::Erasing()
{
	if( ErasingVec.size()>0 ) 
	{
		Score=Score+(int)ErasingVec.size()+ErasingTimes*2;
		IsLevelUp();
		ErasingTimes++;
	}
	for( int i=0;i<n_width;i++)
		for(int j=n_height-1;j>-1;j--)
			for(int n=0;n<(int)ErasingVec.size();n++)
				if( ErasingVec[n].x==i && ErasingVec[n].y==j )
				{
					column[i][j].makeSBEmpty();
					SBPosition del;
					del.x=i;
					del.y=j;
					while(!column[del.x][del.y+1].isSBEmpty())
					{
						column[del.x][del.y].PutState(column[del.x][del.y+1].GetState());
						column[del.x][del.y+1].makeSBEmpty();
						del.y++;
					}
				}
				ErasingVec.clear();
}

void BlockBoard::DisplayErasing(HDC &hdc)
{
	if(ErasingTimes>0)
	{
		RECT rectErasingTimes;
		rectErasingTimes.bottom=64;
		rectErasingTimes.left=45;
		rectErasingTimes.right=64;
		rectErasingTimes.top=45;
		wchar_t outErasingTimes[2];
		_itow_s(ErasingTimes,outErasingTimes,4); 
		DrawState(hdc,NULL,NULL,(LPARAM)NP_ERASINGTIMES,0,30,30,50,50,DST_BITMAP);
		DrawState(hdc,NULL,NULL,(LPARAM)NP_GOOD,0,229,152,45,25,DST_BITMAP);
		SetTextColor(hdc,0x00ff0000);
		SetBkColor(hdc,0x00000000) ;
		//设置字体大小
		DrawText(hdc,outErasingTimes,-1,&rectErasingTimes,DT_CENTER);
	}
	Sleep(150);
	for(int i=0;i<(int)ErasingVec.size();i++)
		DrawState(hdc,NULL,NULL,(LPARAM)NP_ERASING_1,0,xB2C(ErasingVec[i].x*30),yB2C(ErasingVec[i].y*30),30,30,DST_BITMAP);
	Sleep(150);
	for(int i=0;i<(int)ErasingVec.size();i++)
		DrawState(hdc,NULL,NULL,(LPARAM)NP_ERASING_2,0,xB2C(ErasingVec[i].x*30),yB2C(ErasingVec[i].y*30),30,30,DST_BITMAP);
	Sleep(150);
	for(int i=0;i<(int)ErasingVec.size();i++)
		DrawState(hdc,NULL,NULL,(LPARAM)NP_ERASING_3,0,xB2C(ErasingVec[i].x*30),yB2C(ErasingVec[i].y*30),30,30,DST_BITMAP);
	Sleep(150);
	for(int i=0;i<(int)ErasingVec.size();i++)
		DrawState(hdc,NULL,NULL,(LPARAM)NP_ERASING_4,0,xB2C(ErasingVec[i].x*30),yB2C(ErasingVec[i].y*30),30,30,DST_BITMAP);
	Sleep(150);
}

void BlockBoard::DisplayCurrentMB(HDC &hdc)
{
	for(int i=0;i<CurrentMB.GetNumSB();i++)
	{
		switch(CurrentMB.MBstate[i].GetState())
		{
		case SB_BOMB:
			DrawSBState(hdc,NULL,NULL,(LPARAM)NP_BOMB,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_LEVIN:
			DrawSBState(hdc,NULL,NULL,(LPARAM)NP_LEVIN,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_PENTACLE:
			DrawSBState(hdc,NULL,NULL,(LPARAM)NP_PENTACLE,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_RED:
			DrawSBState(hdc,NULL,NULL,(LPARAM)NP_RED,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_YELLOW:
			DrawSBState(hdc,NULL,NULL,(LPARAM)NP_YELLOW,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_BLUE:
			DrawSBState(hdc,NULL,NULL,(LPARAM)NP_BLUE,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_GREEN:
			DrawSBState(hdc,NULL,NULL,(LPARAM)NP_GREEN,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_PURPLE:
			DrawSBState(hdc,NULL,NULL,(LPARAM)NP_PURPLE,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_STONE:
			DrawSBState(hdc,NULL,NULL,(LPARAM)NP_STONE,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		default:
			break;
		}
	}
}

void BlockBoard::DisplayNextMB(HDC &hdc)
{
	DrawState(hdc,NULL,NULL,(LPARAM)NP_SMBB,0,235,24,90,90,DST_BITMAP);

	int _n_Y;
	if(NextMB.GetKind()==MB_3V) _n_Y=NextMB_Y;
	else _n_Y=NextMB_Y+30;
	for(int i=0;i<NextMB.GetNumSB();i++)
	{
		switch(NextMB.MBstate[i].GetState())
		{
		case SB_BOMB:
			DrawState(hdc,NULL,NULL,(LPARAM)NP_BOMB,0,xB2C(NextMB_X+(NextMB.MBstate[i].GetPosition().x-NextMB.MBstate[0].GetPosition().x)*30),yB2C(_n_Y+(NextMB.MBstate[i].GetPosition().y-NextMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_LEVIN:
			DrawState(hdc,NULL,NULL,(LPARAM)NP_LEVIN,0,xB2C(NextMB_X+(NextMB.MBstate[i].GetPosition().x-NextMB.MBstate[0].GetPosition().x)*30),yB2C(_n_Y+(NextMB.MBstate[i].GetPosition().y-NextMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_PENTACLE:
			DrawState(hdc,NULL,NULL,(LPARAM)NP_PENTACLE,0,xB2C(NextMB_X+(NextMB.MBstate[i].GetPosition().x-NextMB.MBstate[0].GetPosition().x)*30),yB2C(_n_Y+(NextMB.MBstate[i].GetPosition().y-NextMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_RED:
			DrawState(hdc,NULL,NULL,(LPARAM)NP_RED,0,xB2C(NextMB_X+(NextMB.MBstate[i].GetPosition().x-NextMB.MBstate[0].GetPosition().x)*30),yB2C(_n_Y+(NextMB.MBstate[i].GetPosition().y-NextMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_YELLOW:
			DrawState(hdc,NULL,NULL,(LPARAM)NP_YELLOW,0,xB2C(NextMB_X+(NextMB.MBstate[i].GetPosition().x-NextMB.MBstate[0].GetPosition().x)*30),yB2C(_n_Y+(NextMB.MBstate[i].GetPosition().y-NextMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_BLUE:
			DrawState(hdc,NULL,NULL,(LPARAM)NP_BLUE,0,xB2C(NextMB_X+(NextMB.MBstate[i].GetPosition().x-NextMB.MBstate[0].GetPosition().x)*30),yB2C(_n_Y+(NextMB.MBstate[i].GetPosition().y-NextMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_GREEN:
			DrawState(hdc,NULL,NULL,(LPARAM)NP_GREEN,0,xB2C(NextMB_X+(NextMB.MBstate[i].GetPosition().x-NextMB.MBstate[0].GetPosition().x)*30),yB2C(_n_Y+(NextMB.MBstate[i].GetPosition().y-NextMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_PURPLE:
			DrawState(hdc,NULL,NULL,(LPARAM)NP_PURPLE,0,xB2C(NextMB_X+(NextMB.MBstate[i].GetPosition().x-NextMB.MBstate[0].GetPosition().x)*30),yB2C(_n_Y+(NextMB.MBstate[i].GetPosition().y-NextMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		case SB_STONE:
			DrawState(hdc,NULL,NULL,(LPARAM)NP_STONE,0,xB2C(NextMB_X+(NextMB.MBstate[i].GetPosition().x-NextMB.MBstate[0].GetPosition().x)*30),yB2C(_n_Y+(NextMB.MBstate[i].GetPosition().y-NextMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
			break;
		default:
			break;
		}
	}

}

void BlockBoard::DisplayBB(HDC &hdc)
{
	DrawState(hdc,NULL,NULL,(LPARAM)NP_DEADLINE,0,25,0,180,25,DST_BITMAP);
	RECT rectLevel,rectScore;
	rectScore.bottom=270;
	rectScore.left=270;
	rectScore.right=320;
	rectScore.top=250;
	rectLevel.bottom=320;
	rectLevel.left=270;
	rectLevel.right=320;
	rectLevel.top=300;
	wchar_t outScore[10],outLevel[10];
	_itow_s(Level,outLevel,10);//数字转字符串
	_itow_s(Score,outScore,10);
	SetTextColor(hdc,0x00ff0000);
	SetBkColor(hdc,0x0080ffff) ;
	DrawText(hdc,outLevel,-1,&rectLevel,DT_CENTER);
	DrawText(hdc,outScore,-1,&rectScore,DT_CENTER);

	for(int i=0;i<n_width;i++)
		for(int j=0;j<HighLimit;j++)
			switch(column[i][j].GetState())
		{
			case SB_BOMB:
				DrawState(hdc,NULL,NULL,(LPARAM)NP_BOMB,0,xB2C(i*30),yB2C(j*30),30,30,DST_BITMAP);
				break;
			case SB_LEVIN:
				DrawState(hdc,NULL,NULL,(LPARAM)NP_LEVIN,0,xB2C(i*30),yB2C(j*30),30,30,DST_BITMAP);
				break;
			case SB_PENTACLE:
				DrawState(hdc,NULL,NULL,(LPARAM)NP_PENTACLE,0,xB2C(i*30),yB2C(j*30),30,30,DST_BITMAP);
				break;
			case SB_RED:
				DrawState(hdc,NULL,NULL,(LPARAM)NP_RED,0,xB2C(i*30),yB2C(j*30),30,30,DST_BITMAP);
				break;
			case SB_YELLOW:
				DrawState(hdc,NULL,NULL,(LPARAM)NP_YELLOW,0,xB2C(i*30),yB2C(j*30),30,30,DST_BITMAP);
				break;
			case SB_BLUE:
				DrawState(hdc,NULL,NULL,(LPARAM)NP_BLUE,0,xB2C(i*30),yB2C(j*30),30,30,DST_BITMAP);
				break;
			case SB_GREEN:
				DrawState(hdc,NULL,NULL,(LPARAM)NP_GREEN,0,xB2C(i*30),yB2C(j*30),30,30,DST_BITMAP);
				break;
			case SB_PURPLE:
				DrawState(hdc,NULL,NULL,(LPARAM)NP_PURPLE,0,xB2C(i*30),yB2C(j*30),30,30,DST_BITMAP);
				break;
			case SB_STONE:
				DrawState(hdc,NULL,NULL,(LPARAM)NP_STONE,0,xB2C(i*30),yB2C(j*30),30,30,DST_BITMAP);
				break;
			case SB_BLANK:
				DrawState(hdc,NULL,NULL,(LPARAM)NP_BLANK,0,xB2C(i*30),yB2C(j*30),30,30,DST_BITMAP);
				break;
			default:
				break;
		}
}

int BlockBoard::GetSpeed()
{
	if( Level < 11 && Level > 0 ) return (151-15*Level);
	else return 1;
}

int BlockBoard::GetLevel()
{
	return Level;
}

int BlockBoard::GetMaxErasingTimes()
{
	return MaxErasingTimes;
}

int BlockBoard::GetnMB()
{
	return nMB;
}

int BlockBoard::GetScore()
{
	return Score;
}

void BlockBoard::BBInitial()
{	
	for( int i=0;i<n_width;i++)
		for(int j=0;j<n_height;j++)
			column[i][j].PutState(SB_BLANK);
	nMB=-2;
	Score=0;
	Level=1;
	MaxErasingTimes=0;
	dead_flag=true;

}

SBPosition BlockBoard::GetDeadPosition()
{
	return DeadPosition;
}

bool BlockBoard::NeedErasing()
{
	if(ErasingVec.size()>0) return true;
	else return false;
}

bool BlockBoard::IsLevelUp()
{
	if( Score >= (30*Level*Level) )
	{
		Level++;
		return true;
	}
	else return false;
}

bool BlockBoard::IsGameOver()
{
	bool game_over=false;
	for(int i=0;i<n_width;i++)
	{
		if(!column[i][HighLimit].isSBEmpty())
		{
			game_over=true;
			break;
		}
	}
	return game_over;
}

void BlockBoard::MB_Next2Current()
{
	CurrentMB=NextMB;
}

void BlockBoard::DisplayGameOver(HDC &hdc)
{
	DrawState(hdc,NULL,NULL,(LPARAM)NP_GAMEOVER,0,60,65,110,70,DST_BITMAP);
}

void BlockBoard::SAVE(HANDLE &hFile)
{
	unsigned long lWrite;
	WriteFile(hFile,&Active_Pt.x,sizeof(LONG),&lWrite,NULL);
	WriteFile(hFile,&Active_Pt.y,sizeof(LONG),&lWrite,NULL);	
	WriteFile(hFile,&dead_flag,sizeof(bool),&lWrite,NULL);
	WriteFile(hFile,&next_step,sizeof(int),&lWrite,NULL);
	WriteFile(hFile,&DeadPosition.x,sizeof(int),&lWrite,NULL);
	WriteFile(hFile,&DeadPosition.y,sizeof(int),&lWrite,NULL);
	WriteFile(hFile,&DeadState,sizeof(SBState),&lWrite,NULL);
	WriteFile(hFile,&Score,sizeof(int),&lWrite,NULL);
	WriteFile(hFile,&ErasingTimes,sizeof(int),&lWrite,NULL);
	WriteFile(hFile,&Level,sizeof(int),&lWrite,NULL);
	WriteFile(hFile,&nMB,sizeof(int),&lWrite,NULL);
	WriteFile(hFile,&MaxErasingTimes,sizeof(int),&lWrite,NULL);

	WriteFile(hFile,&CurrentMB.Active_Pt.x,sizeof(LONG),&lWrite,NULL);
	WriteFile(hFile,&CurrentMB.Active_Pt.y,sizeof(LONG),&lWrite,NULL);
	WriteFile(hFile,&CurrentMB.high,sizeof(int),&lWrite,NULL);
	WriteFile(hFile,&CurrentMB.kind,sizeof(int),&lWrite,NULL);
	WriteFile(hFile,&CurrentMB.left,sizeof(int),&lWrite,NULL);
	WriteFile(hFile,&CurrentMB.right,sizeof(int),&lWrite,NULL);
	WriteFile(hFile,&CurrentMB.numSB,sizeof(int),&lWrite,NULL);
	for(int i=0;i<CurrentMB.max_numSB;i++)
	{
		WriteFile(hFile,&CurrentMB.MBstate[i].order,sizeof(int),&lWrite,NULL);
		WriteFile(hFile,&CurrentMB.MBstate[i].position.x,sizeof(int),&lWrite,NULL);
		WriteFile(hFile,&CurrentMB.MBstate[i].position.y,sizeof(int),&lWrite,NULL);
		WriteFile(hFile,&CurrentMB.MBstate[i].state,sizeof(int),&lWrite,NULL);
	}

	WriteFile(hFile,&NextMB.Active_Pt.x,sizeof(LONG),&lWrite,NULL);
	WriteFile(hFile,&NextMB.Active_Pt.y,sizeof(LONG),&lWrite,NULL);
	WriteFile(hFile,&NextMB.high,sizeof(int),&lWrite,NULL);
	WriteFile(hFile,&NextMB.kind,sizeof(int),&lWrite,NULL);
	WriteFile(hFile,&NextMB.left,sizeof(int),&lWrite,NULL);
	WriteFile(hFile,&NextMB.right,sizeof(int),&lWrite,NULL);
	WriteFile(hFile,&NextMB.numSB,sizeof(int),&lWrite,NULL);
	for(int i=0;i<NextMB.max_numSB;i++)
	{
		WriteFile(hFile,&NextMB.MBstate[i].order,sizeof(int),&lWrite,NULL);
		WriteFile(hFile,&NextMB.MBstate[i].position.x,sizeof(int),&lWrite,NULL);
		WriteFile(hFile,&NextMB.MBstate[i].position.y,sizeof(int),&lWrite,NULL);
		WriteFile(hFile,&NextMB.MBstate[i].state,sizeof(int),&lWrite,NULL);
	}

	for( int i=0;i<n_width;i++)
		for(int j=0;j<n_height;j++)
		{
			WriteFile(hFile,&column[i][j].order,sizeof(int),&lWrite,NULL);
			WriteFile(hFile,&column[i][j].position.x,sizeof(int),&lWrite,NULL);
			WriteFile(hFile,&column[i][j].position.y,sizeof(int),&lWrite,NULL);
			WriteFile(hFile,&column[i][j].state,sizeof(int),&lWrite,NULL);
		}
		//WriteFile(hFile,this,sizeof(BlockBoard),&lWrite,NULL);
}

void BlockBoard::LOAD(HANDLE &hFile)
{
	unsigned long lRead;
	ReadFile(hFile,&Active_Pt.x,sizeof(LONG),&lRead,NULL);
	ReadFile(hFile,&Active_Pt.y,sizeof(LONG),&lRead,NULL);	
	ReadFile(hFile,&dead_flag,sizeof(bool),&lRead,NULL);
	ReadFile(hFile,&next_step,sizeof(int),&lRead,NULL);
	ReadFile(hFile,&DeadPosition.x,sizeof(int),&lRead,NULL);
	ReadFile(hFile,&DeadPosition.y,sizeof(int),&lRead,NULL);
	ReadFile(hFile,&DeadState,sizeof(SBState),&lRead,NULL);
	ReadFile(hFile,&Score,sizeof(int),&lRead,NULL);
	ReadFile(hFile,&ErasingTimes,sizeof(int),&lRead,NULL);
	ReadFile(hFile,&Level,sizeof(int),&lRead,NULL);
	ReadFile(hFile,&nMB,sizeof(int),&lRead,NULL);
	ReadFile(hFile,&MaxErasingTimes,sizeof(int),&lRead,NULL);

	ReadFile(hFile,&CurrentMB.Active_Pt.x,sizeof(LONG),&lRead,NULL);
	ReadFile(hFile,&CurrentMB.Active_Pt.y,sizeof(LONG),&lRead,NULL);
	ReadFile(hFile,&CurrentMB.high,sizeof(int),&lRead,NULL);
	ReadFile(hFile,&CurrentMB.kind,sizeof(int),&lRead,NULL);
	ReadFile(hFile,&CurrentMB.left,sizeof(int),&lRead,NULL);
	ReadFile(hFile,&CurrentMB.right,sizeof(int),&lRead,NULL);
	ReadFile(hFile,&CurrentMB.numSB,sizeof(int),&lRead,NULL);
	for(int i=0;i<CurrentMB.max_numSB;i++)
	{
		ReadFile(hFile,&CurrentMB.MBstate[i].order,sizeof(int),&lRead,NULL);
		ReadFile(hFile,&CurrentMB.MBstate[i].position.x,sizeof(int),&lRead,NULL);
		ReadFile(hFile,&CurrentMB.MBstate[i].position.y,sizeof(int),&lRead,NULL);
		ReadFile(hFile,&CurrentMB.MBstate[i].state,sizeof(int),&lRead,NULL);
	}

	ReadFile(hFile,&NextMB.Active_Pt.x,sizeof(LONG),&lRead,NULL);
	ReadFile(hFile,&NextMB.Active_Pt.y,sizeof(LONG),&lRead,NULL);
	ReadFile(hFile,&NextMB.high,sizeof(int),&lRead,NULL);
	ReadFile(hFile,&NextMB.kind,sizeof(int),&lRead,NULL);
	ReadFile(hFile,&NextMB.left,sizeof(int),&lRead,NULL);
	ReadFile(hFile,&NextMB.right,sizeof(int),&lRead,NULL);
	ReadFile(hFile,&NextMB.numSB,sizeof(int),&lRead,NULL);
	for(int i=0;i<NextMB.max_numSB;i++)
	{
		ReadFile(hFile,&NextMB.MBstate[i].order,sizeof(int),&lRead,NULL);
		ReadFile(hFile,&NextMB.MBstate[i].position.x,sizeof(int),&lRead,NULL);
		ReadFile(hFile,&NextMB.MBstate[i].position.y,sizeof(int),&lRead,NULL);
		ReadFile(hFile,&NextMB.MBstate[i].state,sizeof(int),&lRead,NULL);
	}

	for( int i=0;i<n_width;i++)
		for(int j=0;j<n_height;j++)
		{
			ReadFile(hFile,&column[i][j].order,sizeof(int),&lRead,NULL);
			ReadFile(hFile,&column[i][j].position.x,sizeof(int),&lRead,NULL);
			ReadFile(hFile,&column[i][j].position.y,sizeof(int),&lRead,NULL);
			ReadFile(hFile,&column[i][j].state,sizeof(int),&lRead,NULL);
		}

		//ReadFile(hFile,this,sizeof(BlockBoard),&lRead,NULL);
}


void Read_Dispaly_Record(HDC &);
void DelRecord();
void DisplayHelp(HDC &);
void DisplayIntro(HDC &);

bool BlockBoard::WillSBDead(int suffix)
{
	if( (!column[CurrentMB.MBstate[suffix].GetPosition().x][CurrentMB.MBstate[suffix].GetPosition().y-1].isSBEmpty()) && (CurrentMB.MBstate[suffix].GetPosition().y*SB_H==(CurrentMB.Active_Pt.y - 0)) ) return true;
	else if(CurrentMB.Active_Pt.y==0) return true;
	else return false;
}


Record::Record()
{
	Score=0;
	MaxErasingTimes=0;
	Level=0;
	nMB=0; 
}

Record::Record(BlockBoard &rhs)
{
	Score=rhs.GetScore();
	MaxErasingTimes=rhs.GetMaxErasingTimes();
	Level=rhs.GetLevel();
	nMB=rhs.GetnMB();
	SYSTEMTIME SysTime;
	GetSystemTime(&SysTime);
	wsprintf(Time,L"%d-%d-%d %d:%d:%d",SysTime.wYear,SysTime.wMonth,SysTime.wDay,SysTime.wHour+8,SysTime.wMinute,SysTime.wSecond);//数字转字符串
	//	MessageBox(NULL,Time,L"GameOver",MB_OK);
}

void Record::Record2File()
{
	HANDLE REC=CreateFile(L"RECORD.rec",GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
	if(REC!=(HANDLE)-1)
	{
		SetFilePointer(REC,0,0,FILE_END);
		unsigned long lWrite;
		WriteFile(REC,&Score,sizeof(int),&lWrite,NULL);
		WriteFile(REC,&nMB,sizeof(int),&lWrite,NULL);
		WriteFile(REC,&MaxErasingTimes,sizeof(int),&lWrite,NULL);
		WriteFile(REC,&Level,sizeof(int),&lWrite,NULL);
		WriteFile(REC,Time,sizeof(wchar_t)*20,&lWrite,NULL);
	}
	CloseHandle(REC);
}

int Record::GetScore()
{
	return Score;
}

int Record::GetMaxErasingTimes()
{
	return MaxErasingTimes;
}

int Record::GetLevel()
{
	return Level;
}

int Record::GetnMB()
{
	return nMB;
}

bool Record::operator<(Record &rhs)
{
	if(Score<rhs.Score) return true;
	else if(Score==rhs.Score && nMB<rhs.nMB) return true;
	else if(Score==rhs.Score && nMB==rhs.nMB && MaxErasingTimes<rhs.MaxErasingTimes ) return true;
	else if(Score==rhs.Score && nMB==rhs.nMB && MaxErasingTimes==rhs.MaxErasingTimes && Level<rhs.Level) return true;
	else return false;
}

wchar_t * Record::GetTime()
{
	return Time;
}


void BlockBoard::Read_Dispaly_Record(HDC &hdc)
{//Read
	vector<Record> rec_vec;
	HANDLE REC=CreateFile(L"RECORD.rec",GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(REC!=(HANDLE)-1)
	{
		unsigned long lRead=-1;
		do
		{
			Record tem4rec;

			ReadFile(REC,&tem4rec.Score,sizeof(int),&lRead,NULL);
			ReadFile(REC,&tem4rec.nMB,sizeof(int),&lRead,NULL);
			ReadFile(REC,&tem4rec.MaxErasingTimes,sizeof(int),&lRead,NULL);
			ReadFile(REC,&tem4rec.Level,sizeof(int),&lRead,NULL);
			ReadFile(REC,tem4rec.Time,sizeof(wchar_t)*20,&lRead,NULL);

			rec_vec.push_back(tem4rec);
		}while(lRead!=0);
	}
	CloseHandle(REC);
	sort(rec_vec.begin(),rec_vec.end());
	//Display
	SetTextColor(hdc,0x000000ff);
	SetBkColor(hdc,0x00000000) ;
	TextOut(hdc,6,10,L"名次  分数   块数  连消数  级别       日期 时间",32);
	for(int i=0;i<10;i++)
		if( rec_vec.size() > 0 )
		{	
			wchar_t outREC_num[4];
			wchar_t outREC_Score[8];
			wchar_t outREC_nMB[8];
			wchar_t outREC_MaxErasingTimes[4];
			wchar_t outREC_Level[8];
			wchar_t outREC_Time[24];
			wsprintf(outREC_num,L"%2d.\0",i);
			wsprintf(outREC_Score,L"%6d\0",rec_vec[rec_vec.size()-1].GetScore());
			wsprintf(outREC_nMB,L"%6d\0",rec_vec[rec_vec.size()-1].GetnMB());
			wsprintf(outREC_MaxErasingTimes,L"%2d\0",rec_vec[rec_vec.size()-1].GetMaxErasingTimes());
			wsprintf(outREC_Level,L"%4d\0",rec_vec[rec_vec.size()-1].GetLevel());
			wsprintf(outREC_Time,L"%20s\0",rec_vec[rec_vec.size()-1].GetTime());

			TextOut(hdc,10,i*30+40,outREC_num,(int)wcslen(outREC_num));//wcslen 宽字符串长度
			TextOut(hdc,34,i*30+40,outREC_Score,(int)wcslen(outREC_Score));//wcslen 宽字符串长度
			TextOut(hdc,77,i*30+40,outREC_nMB,(int)wcslen(outREC_nMB));//wcslen 宽字符串长度
			TextOut(hdc,132,i*30+40,outREC_MaxErasingTimes,(int)wcslen(outREC_MaxErasingTimes));//wcslen 宽字符串长度
			TextOut(hdc,170,i*30+40,outREC_Level,(int)wcslen(outREC_Level));//wcslen 宽字符串长度
			TextOut(hdc,205,i*30+40,outREC_Time,(int)wcslen(outREC_Time));//wcslen 宽字符串长度
			rec_vec.pop_back();
		}
		else
		{
			wchar_t outREC[64];
			wchar_t time[20]=L"0000-00-00 00:00:00";
			wsprintf(outREC,L"%2d.%8d%10d%8d%10d%24s\0",i,0,0,0,0,time);
			//MessageBox(NULL,outREC,L"ok",MB_OK);
			TextOut(hdc,10,i*30+40,outREC,(int)wcslen(outREC));
		}
}

void BlockBoard::DelRecord()
{
	DeleteFile(L"RECORD.rec");
}


void BlockBoard::DisplayHelp(HDC &hdc)
{
	SetTextColor(hdc,0x0000ffff);
	SetBkColor(hdc,0x00000000) ;
	TextOut(hdc,40,40,L"帮助：",3);
	TextOut(hdc,40,70,L"游戏开始：Alt+N+N",12);
	TextOut(hdc,40,100,L"左移：      LEFT",13);
	TextOut(hdc,40,130,L"右移：      RIGHT",14);
	TextOut(hdc,40,160,L"下落：      DOWN",13);
	TextOut(hdc,40,190,L"换色：      UP",11);
	TextOut(hdc,40,220,L"保存游戏：Alt+N+D",12);
	TextOut(hdc,40,250,L"载入游戏：Alt+N+S",12);
	TextOut(hdc,40,280,L"退出游戏：Esc/Alt+N+X",16);

}

void BlockBoard::DisplayIntro(HDC &hdc)
{
	SetTextColor(hdc,0x00ff0000);
	SetBkColor(hdc,0x00000000) ;
	TextOut(hdc,40,40,L"NaturePark",10);
	TextOut(hdc,40,100,L"各个方向连续三个相同的色块可以消去。",18);
	TextOut(hdc,40,160,L"连消数即最大连消数，落下一块后连续消",18);
	TextOut(hdc,40,190,L"块的最大次数。",7);
}