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

enum NP_MSG{GAME_INITIAL,GAME_START,GAME_PAUSE,GAME_LEFT,GAME_RIGHT,GAME_UP,GAME_FALLDOWN,GAME_STEPDOWN,GAME_ERASING,GAME_OVER,LOOK_REC,DEl_REC,LOOK_INTRO,LOOK_HELP};
enum SBState{ SB_BLANK,SB_RED,SB_YELLOW,SB_BLUE,SB_GREEN,SB_PURPLE,SB_STONE,SB_BOMB,SB_LEVIN,SB_PENTACLE };
enum MBKind{MB_1=1,MB_2H,MB_2V,MB_3L,MB_3R,MB_3H,MB_3V};
typedef struct
{
	int x;
	int y;
}SBPosition;
HBITMAP NP_BB=(HBITMAP)LoadImage(NULL,L"NP_BB.bmp",IMAGE_BITMAP,345,350,LR_LOADFROMFILE);
HBITMAP NP_COVER=(HBITMAP)LoadImage(NULL,L"NP_COVER.bmp",IMAGE_BITMAP,345,350,LR_LOADFROMFILE);
HBITMAP NP_BLANK=(HBITMAP)LoadImage(NULL,L"NP_BLANK.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
HBITMAP NP_RED=(HBITMAP)LoadImage(NULL,L"NP_RED.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
HBITMAP NP_YELLOW=(HBITMAP)LoadImage(NULL,L"NP_YELLOW.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
HBITMAP NP_BLUE=(HBITMAP)LoadImage(NULL,L"NP_BLUE.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
HBITMAP NP_GREEN=(HBITMAP)LoadImage(NULL,L"NP_GREEN.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
HBITMAP NP_PURPLE=(HBITMAP)LoadImage(NULL,L"NP_PURPLE.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
HBITMAP NP_STONE=(HBITMAP)LoadImage(NULL,L"NP_STONE.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
HBITMAP NP_BOMB=(HBITMAP)LoadImage(NULL,L"NP_BOMB.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
HBITMAP NP_LEVIN=(HBITMAP)LoadImage(NULL,L"NP_LEVIN.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
HBITMAP NP_PENTACLE=(HBITMAP)LoadImage(NULL,L"NP_PENTACLE.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
HBITMAP NP_ERASING_1=(HBITMAP)LoadImage(NULL,L"NP_ERASING_1.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
HBITMAP NP_ERASING_2=(HBITMAP)LoadImage(NULL,L"NP_ERASING_2.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
HBITMAP NP_ERASING_3=(HBITMAP)LoadImage(NULL,L"NP_ERASING_3.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
HBITMAP NP_ERASING_4=(HBITMAP)LoadImage(NULL,L"NP_ERASING_4.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE);
HBITMAP NP_DEADLINE=(HBITMAP)LoadImage(NULL,L"NP_DEADLINE.bmp",IMAGE_BITMAP,180,25,LR_LOADFROMFILE);
HBITMAP NP_SMBB=(HBITMAP)LoadImage(NULL,L"NP_SMBB.bmp",IMAGE_BITMAP,90,90,LR_LOADFROMFILE);
HBITMAP NP_GAMEOVER=(HBITMAP)LoadImage(NULL,L"NP_GAMEOVER.bmp",IMAGE_BITMAP,110,70,LR_LOADFROMFILE);
HBITMAP NP_ERASINGTIMES=(HBITMAP)LoadImage(NULL,L"NP_ERASINGTIMES.bmp",IMAGE_BITMAP,50,50,LR_LOADFROMFILE);
HBITMAP NP_GOOD=(HBITMAP)LoadImage(NULL,L"NP_GOOD.bmp",IMAGE_BITMAP,45,25,LR_LOADFROMFILE);
HBITMAP NP_BKG=(HBITMAP)LoadImage(NULL,L"NP_BKG.bmp",IMAGE_BITMAP,345,350,LR_LOADFROMFILE);

int MyRandom()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	return ((st.wDay+st.wDayOfWeek+st.wHour+st.wMinute+st.wMonth+st.wSecond+st.wYear+st.wMilliseconds)%rand());
}

class SingleBlock
{
	friend class MultiBlock;//for SAVE and LOAD
	friend class BlockBoard;//for SAVE and LOAD
public:
	SingleBlock(SingleBlock &rhs)
	{
		state=rhs.state;
		position.x=rhs.position.x;
		position.y=rhs.position.y;
		order=rhs.order;
		
	}
	SingleBlock(int x=0,int y=0)
	{
		state=SB_BLANK;
		position.x=x;
		position.y=y;
		order=0;
	}
	SingleBlock(SBState sbstate,int x=0,int y=0)
	{
		state=sbstate;
		position.x=x;
		position.y=y;
		order=0;
	}
	SBState GetState()
	{
		return state;
	}
	SBPosition GetPosition()
	{
		return position;
	}
	int GetOrder()
	{
		return order;
	}
	void PutState( SBState st )
	{
		state=st;
	}
	void PutPosition(SBPosition &pos)
	{
		position.x=pos.x;
		position.y=pos.y;
	}
	void PutPosition(int x,int y)
	{
		position.x=x;
		position.y=y;
	}
	void SetOrder(int i)
	{
		order=i;
	}
	bool isSBEmpty()
	{
		if(!state) return true;
		else return false;
	}
	void makeSBEmpty()
	{
		state=SB_BLANK;
	}
	SingleBlock operator=(SingleBlock &rhs)
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

private:
	static const int height=SB_H;
	static const int width=SB_W;
	int order;
	SBState state;
	SBPosition position;
};
class MultiBlock
{
	friend class BlockBoard;//for SAVE and LOAD
public:
	MultiBlock()
	{}
	void MBInitial( MBKind mbk )
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
	void Transposition()
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
	MBKind GetKind()
	{
		return kind;
	}
	int GetNumSB()
	{
		return numSB;
	}
	int GetLeft()
	{
		return left;
	}
	int GetRight()
	{
		return right;
	}
	int GetHigh()
	{
		return high;
	}

	MultiBlock operator=(MultiBlock &rhs)
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


private:
	static const int max_numSB=3;
	MBKind kind;
	int numSB;
	int left;
	int right;
	int high;
public:
	SingleBlock MBstate[max_numSB];
	POINT Active_Pt;
};
class BlockBoard
{
public:
	BlockBoard()
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

	SBState GetSBState( SBPosition pos )
	{
		return column[pos.x][pos.y].GetState();
	}
	SBState GetSBState( int x,int y)
	{
		return column[x][y].GetState();
	}
	void GenerateMB(MultiBlock &MB)
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

	void MBMoveLeft()
	{
		if(CurrentMB.MBstate[0].GetPosition().x+CurrentMB.GetLeft()/SB_W > 0 && column[CurrentMB.MBstate[0].GetPosition().x+CurrentMB.GetLeft()/SB_W-1][CurrentMB.MBstate[0].GetPosition().y].isSBEmpty()) 
		{
			for(int i=0;i<CurrentMB.GetNumSB();i++)
				CurrentMB.MBstate[i].PutPosition(CurrentMB.MBstate[i].GetPosition().x-1,CurrentMB.MBstate[i].GetPosition().y);
			CurrentMB.Active_Pt.x-=30;
			Active_Pt=CurrentMB.Active_Pt;
		}
	}
	void MBMoveRight()//注意右值至少就有30
	{
		if(CurrentMB.MBstate[0].GetPosition().x+CurrentMB.GetRight()/SB_W - 1 < 5 && column[CurrentMB.MBstate[0].GetPosition().x+CurrentMB.GetRight()/SB_W][CurrentMB.MBstate[0].GetPosition().y].isSBEmpty()) 
		{
			for(int i=0;i<CurrentMB.GetNumSB();i++)
				CurrentMB.MBstate[i].PutPosition(CurrentMB.MBstate[i].GetPosition().x+1,CurrentMB.MBstate[i].GetPosition().y);
			CurrentMB.Active_Pt.x+=30;
			Active_Pt=CurrentMB.Active_Pt;
		}
	}
	void MBTransposition()
	{
		CurrentMB.Transposition();
	}
	void MBFallDown()
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
	void MBFallStep()
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
	bool WillMBDead()
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
	SBState ReceiveWhat()
	{
		return column[DeadPosition.x][DeadPosition.y].GetState();
	}
	SBState ReceiveWhat( int i)
	{
		return DeadState;
	}
	void Find4NormalErasing()
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
	void Find4BOMBErasing(SBPosition &pos)
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
	void Find4LEVINErasing(SBPosition &pos)
	{
		for(int n=0;n<=pos.y;n++)
		{
			SBPosition tem;
			tem.x=pos.x;
			tem.y=n;
			if(GetSBState(tem)) ErasingVec.push_back(tem);
		}
	}
	void Find4PENTACLEErasing(SBPosition &pos)
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
	void Erasing()
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
	void DisplayErasing(HDC &hdc)
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
	void DisplayCurrentMB(HDC &hdc)
	{
		for(int i=0;i<CurrentMB.GetNumSB();i++)
		{
			switch(CurrentMB.MBstate[i].GetState())
			{
				case SB_BOMB:
					DrawState(hdc,NULL,NULL,(LPARAM)NP_BOMB,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
					break;
				case SB_LEVIN:
					DrawState(hdc,NULL,NULL,(LPARAM)NP_LEVIN,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
					break;
				case SB_PENTACLE:
					DrawState(hdc,NULL,NULL,(LPARAM)NP_PENTACLE,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
					break;
				case SB_RED:
					DrawState(hdc,NULL,NULL,(LPARAM)NP_RED,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
					break;
				case SB_YELLOW:
					DrawState(hdc,NULL,NULL,(LPARAM)NP_YELLOW,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
					break;
				case SB_BLUE:
					DrawState(hdc,NULL,NULL,(LPARAM)NP_BLUE,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
					break;
				case SB_GREEN:
					DrawState(hdc,NULL,NULL,(LPARAM)NP_GREEN,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
					break;
				case SB_PURPLE:
					DrawState(hdc,NULL,NULL,(LPARAM)NP_PURPLE,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
					break;
				case SB_STONE:
					DrawState(hdc,NULL,NULL,(LPARAM)NP_STONE,0,xB2C(CurrentMB.Active_Pt.x+(CurrentMB.MBstate[i].GetPosition().x-CurrentMB.MBstate[0].GetPosition().x)*30),yB2C(CurrentMB.Active_Pt.y+(CurrentMB.MBstate[i].GetPosition().y-CurrentMB.MBstate[0].GetPosition().y)*30),30,30,DST_BITMAP);
					break;
				default:
					break;
			}
		}
	}
	void DisplayNextMB(HDC &hdc)
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
	void DisplayBB(HDC &hdc)
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

	int GetSpeed()
	{
		if( Level < 11 && Level > 0 ) return (151-15*Level);
		else return 1;
	}
	int GetLevel()
	{
		return Level;
	}
	int GetMaxErasingTimes()
	{
		return MaxErasingTimes;
	}
	int GetnMB()
	{
		return nMB;
	}
	int GetScore()
	{
		return Score;
	}
	void BBInitial()
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
	SBPosition GetDeadPosition()
	{
		return DeadPosition;
	}
	bool NeedErasing()
	{
		if(ErasingVec.size()>0) return true;
		else return false;
	}
	bool IsLevelUp()
	{
		if( Score >= (30*Level*Level) )
		{
			Level++;
			return true;
		}
		else return false;
	}
	bool IsGameOver()
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
	void MB_Next2Current()
	{
		CurrentMB=NextMB;
	}

	void DisplayGameOver(HDC &hdc)
	{
		DrawState(hdc,NULL,NULL,(LPARAM)NP_GAMEOVER,0,60,65,110,70,DST_BITMAP);
	}
	void SAVE(HANDLE &hFile)
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
	void LOAD(HANDLE &hFile)
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
	MultiBlock CurrentMB;
	MultiBlock NextMB;

private:
	static const int n_height=nBB_H;
	static const int n_width=nBB_W;
	static const int GeneratePositionX=InitialPos_X;
	static const int GeneratePositionY=InitialPos_Y;
	static const int GeneratePOINTX=Initial_X;
	static const int GeneratePOINTY=Initial_Y;
	static const int LeftLimit=0;
	static const int RightLimit=nBB_W-1;
	static const int HighLimit=nBB_LH;
	SingleBlock column[n_width][n_height];
	vector<SBPosition> ErasingVec;
	POINT Active_Pt;
	bool dead_flag;
	int next_step;
	SBPosition DeadPosition;
	SBState DeadState;
	int Score;
	int ErasingTimes;
	int MaxErasingTimes;
	int Level;
	int nMB; 
	bool WillSBDead(int suffix)
	{
		if( (!column[CurrentMB.MBstate[suffix].GetPosition().x][CurrentMB.MBstate[suffix].GetPosition().y-1].isSBEmpty()) && (CurrentMB.MBstate[suffix].GetPosition().y*SB_H==(CurrentMB.Active_Pt.y - 0)) ) return true;
		else if(CurrentMB.Active_Pt.y==0) return true;
		else return false;
	}
};
class Record
{
	friend void BlockBoard::Read_Dispaly_Record(HDC &);
public:
	Record()
	{
		Score=0;
		MaxErasingTimes=0;
		Level=0;
		nMB=0; 
	}
	Record(BlockBoard &rhs)
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
	void Record2File()
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

	int GetScore()
	{
		return Score;
	}
	int GetMaxErasingTimes()
	{
		return MaxErasingTimes;
	}
	int GetLevel()
	{
		return Level;
	}
	int GetnMB()
	{
		return nMB;
	}
	bool operator<(Record &rhs)
	{
		if(Score<rhs.Score) return true;
		else if(Score==rhs.Score && nMB<rhs.nMB) return true;
		else if(Score==rhs.Score && nMB==rhs.nMB && MaxErasingTimes<rhs.MaxErasingTimes ) return true;
		else if(Score==rhs.Score && nMB==rhs.nMB && MaxErasingTimes==rhs.MaxErasingTimes && Level<rhs.Level) return true;
		else return false;
	}
	wchar_t * GetTime()
	{
		return Time;
	}
private:
	int Score;
	int MaxErasingTimes;
	int Level;
	int nMB; 
	wchar_t Time[20];
};
inline void BlockBoard::Read_Dispaly_Record(HDC &hdc)
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

inline void BlockBoard::DelRecord()
	{
		DeleteFile(L"RECORD.rec");
	}


inline void BlockBoard::DisplayHelp(HDC &hdc)
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
inline void BlockBoard::DisplayIntro(HDC &hdc)
{
	SetTextColor(hdc,0x00ff0000);
	SetBkColor(hdc,0x00000000) ;
	TextOut(hdc,40,40,L"NaturePark",10);
	TextOut(hdc,40,100,L"各个方向连续三个相同的色块可以消去。",18);
	TextOut(hdc,40,160,L"连消数即最大连消数，落下一块后连续消",18);
	TextOut(hdc,40,190,L"块的最大次数。",7);
}