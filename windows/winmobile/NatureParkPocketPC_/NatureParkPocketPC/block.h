#define SB_H 24
#define SB_W 24
#define nBB_H 13
#define nBB_W 6
#define nBB_LH 10
#define InitialPos_X 2
#define InitialPos_Y 10
#define Initial_X 48
#define Initial_Y 240
#define NextMB_X 185
#define NextMB_Y 38
#define xB2C(x) ((x)+8)
#define yB2C(y) (230-(y))

enum NP_MSG{GAME_INITIAL,GAME_START,GAME_PAUSE,GAME_MBCHANGE,GAME_LEFT,GAME_RIGHT,GAME_UP,GAME_FALLDOWN,GAME_STEPDOWN,GAME_ERASING,GAME_OVER,LOOK_REC,DEl_REC,LOOK_INTRO,LOOK_HELP};
enum SBState{ SB_BLANK,SB_RED,SB_YELLOW,SB_BLUE,SB_GREEN,SB_PURPLE,SB_STONE,SB_BOMB,SB_LEVIN,SB_PENTACLE };
enum MBKind{MB_1=1,MB_2H,MB_2V,MB_3L,MB_3R,MB_3H,MB_3V};

int MyRandom()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	return ((st.wDay+st.wDayOfWeek+st.wHour+st.wMinute+st.wMonth+st.wSecond+st.wYear+st.wMilliseconds)%rand());
}

class SingleBlock
{
	//friend class MultiBlock;//for SAVE and LOAD
	//friend class BlockBoard;//for SAVE and LOAD
public:
	SingleBlock(SingleBlock &rhs)
	{
		sbstate=rhs.sbstate;
		sbposition=rhs.sbposition;
		erasingflag=rhs.erasingflag;
	}
	SingleBlock(POINT &pos)
	{
		sbstate=SB_BLANK;
		sbposition=pos;
		erasingflag=false;
	}
	SingleBlock(int x=0,int y=0)
	{
		sbstate=SB_BLANK;
		sbposition.x=x;
		sbposition.y=y;
		erasingflag=false;
	}
	SingleBlock(SBState st,POINT &pos)
	{
		sbstate=st;
		sbposition=pos;
		erasingflag=false;
	}
	SingleBlock(SBState st,int x=0,int y=0)
	{
		this->sbstate=st;
		sbposition.x=x;
		sbposition.y=y;
		erasingflag=false;
	}
	SBState getSbstate()
	{
		return sbstate;
	}
	POINT getSbposition()
	{
		return sbposition;
	}
	void setSbstate( SBState st )
	{
		sbstate=st;
	}
	void setSbposition(POINT &pos)
	{
		sbposition=pos;
	}
	void setSbposition(int x,int y)
	{
		sbposition.x=x;
		sbposition.y=y;
	}
	bool isSBEmpty()
	{
		if(!sbstate) return true;
		else return false;
	}
	void makeSBEmpty()
	{
		sbstate=SB_BLANK;
	}
	bool isErasingflag()
	{
		return erasingflag;
	}
	void setErasingflag(bool tf)
	{
		erasingflag=tf;
	}
	SingleBlock operator=(SingleBlock &rhs)
	{
		if(this != &rhs)
		{
			sbstate=rhs.sbstate;
			sbposition=rhs.sbposition;
			erasingflag=rhs.erasingflag;
		}
		return *this;
	}


private:
	static const int height=SB_H;
	static const int width=SB_W;
	SBState sbstate;
	POINT sbposition;
	bool erasingflag;
};
class MultiBlock
{
//	friend class BlockBoard;//for SAVE and LOAD
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
			delete temp;
		}
		switch(kind)
		{
		case MB_2H:
			MBstate[0].setSbposition(InitialPos_X,InitialPos_Y);
			MBstate[1].setSbposition(InitialPos_X-1,InitialPos_Y);
			break;
		case MB_2V:
			MBstate[0].setSbposition(InitialPos_X,InitialPos_Y);
			MBstate[1].setSbposition(InitialPos_X,InitialPos_Y+1);
			break;
		case MB_3L:
			MBstate[0].setSbposition(InitialPos_X,InitialPos_Y);
			MBstate[1].setSbposition(InitialPos_X,InitialPos_Y+1);
			MBstate[2].setSbposition(InitialPos_X-1,InitialPos_Y);
			break;
		case MB_3R:
			MBstate[0].setSbposition(InitialPos_X,InitialPos_Y);
			MBstate[1].setSbposition(InitialPos_X,InitialPos_Y+1);
			MBstate[2].setSbposition(InitialPos_X+1,InitialPos_Y);
			break;
		case MB_3H:
			MBstate[0].setSbposition(InitialPos_X,InitialPos_Y);
			MBstate[1].setSbposition(InitialPos_X-1,InitialPos_Y);
			MBstate[2].setSbposition(InitialPos_X+1,InitialPos_Y);
			break;
		case MB_3V:
			MBstate[0].setSbposition(InitialPos_X,InitialPos_Y);
			MBstate[1].setSbposition(InitialPos_X,InitialPos_Y+1);
			MBstate[2].setSbposition(InitialPos_X,InitialPos_Y+2);
			break;
		case MB_1:
			MBstate[0].setSbposition(InitialPos_X,InitialPos_Y);
			break;
		default:
			break;
		}
		active_Pt.x=Initial_X;
		active_Pt.y=Initial_Y;	
	}
	void Transposition()
	{
		switch(numSB)
		{
		case 2:	
			SBState temp2_SBState;
			temp2_SBState=MBstate[0].getSbstate();
			MBstate[0].setSbstate(MBstate[1].getSbstate());
			MBstate[1].setSbstate(temp2_SBState);
		break;
		case 3:
			SBState temp3_SBState;
			temp3_SBState=MBstate[0].getSbstate();
			MBstate[0].setSbstate(MBstate[1].getSbstate());
			MBstate[1].setSbstate(MBstate[2].getSbstate());
			MBstate[2].setSbstate(temp3_SBState);
			break;
		default:
			break;
		}
	}
	MBKind getKind()
	{
		return kind;
	}
	int getNumSB()
	{
		return numSB;
	}
	int getLeft()
	{
		return left;
	}
	int getRight()
	{
		return right;
	}
	int getHigh()
	{
		return high;
	}

	POINT getActive_Pt()
	{
		return active_Pt; 
	}
	void setActive_Pt(POINT &pt)
	{
		active_Pt=pt;
	}
	/*void setActive_Pt(int x,int y)
	{
		active_Pt.x=x;
		active_Pt.y=y;
	}
	*/
	MultiBlock operator=(MultiBlock &rhs)
	{
		if(this != &rhs)
		{
			kind=rhs.kind;
			numSB=rhs.numSB;
			left=rhs.left;
			right=rhs.right;
			high=rhs.high;
			active_Pt=rhs.active_Pt;
			for(int i=0;i<max_numSB;i++)
			{
				MBstate[i]=rhs.MBstate[i];
			}
		}
		return *this;
	}


	SBState getMBstate(int sufSB)
	{
		return MBstate[sufSB].getSbstate();
	}
	void setMBstate(SBState st,int sufSB)
	{
		MBstate[sufSB].setSbstate(st);
	}

	POINT getMBposition(int sufSB)
	{
		return MBstate[sufSB].getSbposition();
	}
	void setMBposition(POINT &pos,int sufSB)
	{
		MBstate[sufSB].setSbposition(pos);
	}
	void setMBposition(int x,int y,int sufSB)
	{
		MBstate[sufSB].setSbposition(x,y);
	}
	
private:
	static const int max_numSB=3;
	MBKind kind;
	int numSB;
	int left;
	int right;
	int high;
	POINT active_Pt;
	SingleBlock MBstate[max_numSB];
	
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
				erase4display[i][j]=false;
			}
		nMB=-2;
		Score=0;
		Level=1;
		MaxErasingTimes=0;
		dead_flag=true;
	}

	SBState GetSBState( POINT &pos )
	{
		return column[pos.x][pos.y].getSbstate();
	}
	SBState GetSBState( int x,int y)
	{
		return column[x][y].getSbstate();
	}
	void GenerateMB(MultiBlock &MB)
	{
		MBKind mbk;
		long random,module=3000;
		random=(rand()+MyRandom())%module;
		if( random >= 0 && random <400 ) mbk=MB_1;
		if( random >= 400 && random <850) mbk=MB_2H;
		if( random >= 850 && random < 1300 ) mbk=MB_2V;
		if( random >= 1300 && random < 1725 ) mbk=MB_3L;
		if( random >= 1725 && random < 2150 ) mbk=MB_3R;
		if( random >= 2150 && random < 2575 ) mbk=MB_3H;
		if( random >= 2575 && random < 3000 ) mbk=MB_3V;
		MB.MBInitial(mbk);
		if( random >= 0 && random <100 ) MB.setMBstate(SB_BOMB,0);
		if( random >= 100 && random <200 ) MB.setMBstate(SB_LEVIN,0);
		if( random >= 200 && random <300 ) MB.setMBstate(SB_PENTACLE,0);
		if( random >= 300 && random <400 ) MB.setMBstate(SB_STONE,0);

		if( random >= 400 && random <800)
			for(int i=0;i<2;i++)
				MB.setMBstate((SBState)(1+(rand()+MyRandom())%5),i);
		if( random >= 800 && random <850)
			for(int i=0;i<2;i++)
				MB.setMBstate(SB_STONE,i);
		if( random >= 850 && random <1250)
			for(int i=0;i<2;i++)
				MB.setMBstate((SBState)(1+(rand()+MyRandom())%5),i);
		if( random >= 1250 && random <1300)
			for(int i=0;i<2;i++)
				MB.setMBstate(SB_STONE,i);
		if( random >= 1300 && random <1700)
			for(int i=0;i<3;i++)
				MB.setMBstate((SBState)(1+(rand()+MyRandom())%5),i);
		if( random >= 1700 && random <1725)
			for(int i=0;i<3;i++)
				MB.setMBstate(SB_STONE,i);
		if( random >= 1725 && random <2125)
			for(int i=0;i<3;i++)
				MB.setMBstate((SBState)(1+(rand()+MyRandom())%5),i);
		if( random >= 2125 && random <2150)
			for(int i=0;i<3;i++)
				MB.setMBstate(SB_STONE,i);		
		if( random >= 2150 && random <2550)
			for(int i=0;i<3;i++)
				MB.setMBstate((SBState)(1+(rand()+MyRandom())%5),i);
		if( random >= 2550 && random <2575)
			for(int i=0;i<3;i++)
				MB.setMBstate(SB_STONE,i);		
		if( random >= 2575 && random <2975)
			for(int i=0;i<3;i++)
				MB.setMBstate((SBState)(1+(rand()+MyRandom())%5),i);
		if( random >= 2975 && random <3000)
			for(int i=0;i<3;i++)
				MB.setMBstate(SB_STONE,i);
		dead_flag=false;
		if(ErasingTimes>MaxErasingTimes) MaxErasingTimes=ErasingTimes;
		need_erase=false;
		ErasingTimes=0;
		Active_Pt=MB.getActive_Pt();	
		nMB=nMB+1;
		next_step=-step_length;
	}

	bool MBMoveLeft()
	{
		if(CurrentMB.getMBposition(0).x+CurrentMB.getLeft()/SB_W > 0 && column[CurrentMB.getMBposition(0).x+CurrentMB.getLeft()/SB_W-1][CurrentMB.getMBposition(0).y].isSBEmpty()) 
		{
			for(int i=0;i<CurrentMB.getNumSB();i++)
				CurrentMB.setMBposition(CurrentMB.getMBposition(i).x-1,CurrentMB.getMBposition(i).y,i);
			Active_Pt.x=Active_Pt.x-SB_W;
			CurrentMB.setActive_Pt(Active_Pt);
			return true;
		}
		else return false;
	}

	bool MBMoveRight()//注意右值至少就有SB_W
	{
		if(CurrentMB.getMBposition(0).x+(CurrentMB.getRight()-SB_W)/SB_W < nBB_W-1 && column[CurrentMB.getMBposition(0).x+(CurrentMB.getRight()-SB_W)/SB_W+1][CurrentMB.getMBposition(0).y].isSBEmpty()) 
		{
			for(int i=0;i<CurrentMB.getNumSB();i++)
				CurrentMB.setMBposition(CurrentMB.getMBposition(i).x+1,CurrentMB.getMBposition(i).y,i);
			Active_Pt.x=Active_Pt.x+SB_W;
			CurrentMB.setActive_Pt(Active_Pt);
			return true;
		}
		else return false;
	}
	void MBTransposition()
	{
		CurrentMB.Transposition();
	}
	void MBFallDown()
	{
		for(int i=0;i<CurrentMB.getNumSB();i++)
		{	
			int j;
			for(j=0;j<=CurrentMB.getMBposition(i).y;j++)
				if(column[CurrentMB.getMBposition(i).x][j].isSBEmpty()) break;
			column[CurrentMB.getMBposition(i).x][j].setSbstate(CurrentMB.getMBstate(i));	
			CurrentMB.setMBstate(SB_BLANK,i);
			//CurrentMB.MBstate[i].PutPosition(CurrentMB.MBstate[i].GetPosition().x,j);////////////////////////////////////////////////////////////
			column[CurrentMB.getMBposition(i).x][j].setSbposition(CurrentMB.getMBposition(i).x,j);
			if( i == 0 ) 
			{
				DeadPosition.x=CurrentMB.getMBposition(i).x;
				DeadPosition.y=j;
				DeadState=column[CurrentMB.getMBposition(i).x][j].getSbstate();
			}
		}	
		dead_flag=true;
	}
	void MBFallStep()
	{
		if( next_step < SB_H && next_step > -step_length ) 
		{
			next_step=next_step-step_length;
			Active_Pt.y=Active_Pt.y-step_length;
			CurrentMB.setActive_Pt(Active_Pt);
		}
		else if( next_step == -step_length ) 
		{
			next_step=SB_H-step_length*2;
			Active_Pt.y=Active_Pt.y-step_length;
			CurrentMB.setActive_Pt(Active_Pt);
			for(int i=0;i<CurrentMB.getNumSB();i++)
				CurrentMB.setMBposition(CurrentMB.getMBposition(i).x,CurrentMB.getMBposition(i).y-1,i);
		}
	}
	bool WillMBDead()
	{
		switch(CurrentMB.getKind())
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
	POINT getDeadPosition()
	{
		return DeadPosition;
	}
	SBState ReceiveWhat()
	{
		return column[DeadPosition.x][DeadPosition.y].getSbstate();
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
				if(!column[i][j].isSBEmpty() && column[i][j].getSbstate()!=SB_STONE)
				{
					if( column[i-1][j].getSbstate()==column[i][j].getSbstate() && column[i+1][j].getSbstate()==column[i][j].getSbstate() ) 
					{
						need_erase=true;
						column[i-1][j].setErasingflag(true);
						column[i][j].setErasingflag(true);
						column[i+1][j].setErasingflag(true);
					}
					if( column[i][j-1].getSbstate()==column[i][j].getSbstate() && column[i][j+1].getSbstate()==column[i][j].getSbstate() ) 
					{
						need_erase=true;
						column[i][j-1].setErasingflag(true);
						column[i][j].setErasingflag(true);
						column[i][j+1].setErasingflag(true);						
					}
					if( column[i-1][j-1].getSbstate()==column[i][j].getSbstate() && column[i+1][j+1].getSbstate()==column[i][j].getSbstate() ) 
					{
						need_erase=true;
						column[i-1][j-1].setErasingflag(true);
						column[i][j].setErasingflag(true);
						column[i+1][j+1].setErasingflag(true);
					}
					if( column[i-1][j+1].getSbstate()==column[i][j].getSbstate() && column[i+1][j-1].getSbstate()==column[i][j].getSbstate() ) 
					{
						need_erase=true;
						column[i-1][j+1].setErasingflag(true);
						column[i][j].setErasingflag(true);
						column[i+1][j-1].setErasingflag(true);
					}
				}
			}
		for(int k=0;k<2;k++)
		{	int i;
			if(k==0) i=0;
			if(k==1) i=n_width-1;
			for(int j=1;j<n_height-1;j++)
			{
				if(!column[i][j].isSBEmpty() && column[i][j].getSbstate()!=SB_STONE)
					{
						if( column[i][j-1].getSbstate()==column[i][j].getSbstate() && column[i][j+1].getSbstate()==column[i][j].getSbstate() ) 
						{
							need_erase=true;
							column[i][j-1].setErasingflag(true);
							column[i][j].setErasingflag(true);
							column[i][j+1].setErasingflag(true);							
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
				if(!column[i][j].isSBEmpty() && column[i][j].getSbstate()!=SB_STONE)
					{
						if( column[i-1][j].getSbstate()==column[i][j].getSbstate() && column[i+1][j].getSbstate()==column[i][j].getSbstate() ) 
						{
							need_erase=true;
							column[i-1][j].setErasingflag(true);
							column[i][j].setErasingflag(true);
							column[i+1][j].setErasingflag(true);
						}
					}
			}
		}
	}
	void Find4BOMBErasing(POINT &pos)
	{
		need_erase=true;
		if(pos.x>0&&pos.x<nBB_W-1&&pos.y>0)
		for(int i=pos.x-1;i<=pos.x+1;i++)
			for(int j=pos.y-1;j<=pos.y+1;j++)
			{
				if(SB_BLANK!=column[i][j].getSbstate()) column[i][j].setErasingflag(true);
			}
		if(pos.x==0 && pos.y==0 )
		for(int i=pos.x;i<=pos.x+1;i++)
			for(int j=pos.y;j<=pos.y+1;j++)
			{
				if(SB_BLANK!=column[i][j].getSbstate()) column[i][j].setErasingflag(true);
			}
		if(pos.x==nBB_W-1 && pos.y==0 )
		for(int i=pos.x-1;i<=pos.x;i++)
			for(int j=pos.y;j<=pos.y+1;j++)
			{
				if(SB_BLANK!=column[i][j].getSbstate()) column[i][j].setErasingflag(true);
			}
		if(pos.x==0 && pos.y>0)
		for(int i=pos.x;i<=pos.x+1;i++)
			for(int j=pos.y-1;j<=pos.y+1;j++)
			{
				if(SB_BLANK!=column[i][j].getSbstate()) column[i][j].setErasingflag(true);
			}
		if(pos.x==nBB_W-1 && pos.y>0)
		for(int i=pos.x-1;i<=pos.x;i++)
			for(int j=pos.y-1;j<=pos.y+1;j++)
			{
				if(SB_BLANK!=column[i][j].getSbstate()) column[i][j].setErasingflag(true);
			}
		if(pos.x>0 && pos.x < nBB_W && pos.y==0)
		for(int i=pos.x-1;i<=pos.x+1;i++)
			for(int j=pos.y;j<=pos.y+1;j++)
			{
				if(SB_BLANK!=column[i][j].getSbstate()) column[i][j].setErasingflag(true);
			}

	}
	void Find4LEVINErasing(POINT &pos)
	{
		need_erase=true;
		for(int n=0;n<=pos.y;n++)
		{
			need_erase=true;
			column[pos.x][n].setErasingflag(true);
		}
	}
	void Find4PENTACLEErasing(POINT &pos)
	{
		need_erase=true;
		column[pos.x][pos.y].setErasingflag(true);
		if(pos.y>0) 
		{
			SBState sbstate;
			sbstate=column[pos.x][pos.y-1].getSbstate();
		for( int i=0;i<n_width;i++)
			for(int j=0;j<n_height;j++)
			{
				if(column[i][j].getSbstate()==sbstate) 
				{
					column[i][j].setErasingflag(true);
				}
			}
		}
	}
	bool Erasing()
	{
		if( need_erase==true ) 
		{
			int add=0;
			for( int i=0;i<n_width;i++)
				for(int j=n_height-1;j>-1;j--)
					if(column[i][j].isErasingflag())
					{
						erase4display[i][j]=true;
						column[i][j].setErasingflag(false);
						add++;
						column[i][j].makeSBEmpty();
						POINT del;
						del.x=i;
						del.y=j;
						while(!column[del.x][del.y+1].isSBEmpty())
						{
							column[del.x][del.y].setSbstate(column[del.x][del.y+1].getSbstate());
							column[del.x][del.y+1].makeSBEmpty();
							del.y++;
						}
					}
			Score=Score+add+ErasingTimes*2;
			if(isLevelUp()) SpeedUp();
			ErasingTimes++;
			need_erase=false;
			return true;
		}
		else return false;
	}
	int getLevel()
	{
		return Level;
	}
	int getMaxErasingTimes()
	{
		return MaxErasingTimes;
	}
	int getnMB()
	{
		return nMB;
	}
	int getScore()
	{
		return Score;
	}
	int getTime()
	{
		return time;
	}
	void BBInitial()
	{	
		for( int i=0;i<n_width;i++)
			for(int j=0;j<n_height;j++)
				column[i][j].setSbstate(SB_BLANK);
		nMB=-2;
		Score=0;
		Level=1;
		MaxErasingTimes=0;
		dead_flag=true;
		need_erase=false;
		step_length=1;
		time=120;
	}

	bool isNeed_erase()
	{
		return need_erase;
	}
	bool isLevelUp()
	{
		if( Score >= (30*Level*Level) )
		{
			Level++;
			return true;
		}
		else return false;
	}
	bool isGameOver()
	{
		bool game_over=false;
		for(int i=0;i<n_width;i++)
		{
			if(!column[i][HighLimit+1].isSBEmpty())
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
	void DisplayCover(HDC &hdc)
	{
		HDC hdcMem = CreateCompatibleDC (hdc);
		SelectObject (hdcMem, NP_COVER_1);
		BitBlt (hdc, 0, 0, 120,	268, hdcMem, 0, 0, SRCCOPY);
		SelectObject (hdcMem, NP_COVER_2);
		BitBlt (hdc, 120, 0, 120,	268, hdcMem, 0, 0, SRCCOPY);
		DeleteDC(hdcMem);
	}
	void DisplayBlank(HDC &hdc)
	{
		for(int i=0;i<CurrentMB.getNumSB();i++)
		{
			HDC hdcMem = CreateCompatibleDC (hdc);
			SelectObject (hdcMem, NP_BLANK);
			BitBlt (hdc, xB2C(CurrentMB.getActive_Pt().x+(CurrentMB.getMBposition(i).x-CurrentMB.getMBposition(0).x)*SB_W),yB2C(CurrentMB.getActive_Pt().y+(CurrentMB.getMBposition(i).y-CurrentMB.getMBposition(0).y)*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
			DeleteDC(hdcMem);
		}
	}
	void DisplayCurrentMB(HDC &hdc)
	{
		HDC hdcMem = CreateCompatibleDC (hdc);
		for(int i=0;i<CurrentMB.getNumSB();i++)
		{
			if(CurrentMB.getMBposition(i).y<=9)
			switch(CurrentMB.getMBstate(i))
			{
				case SB_BOMB:
					{
						SelectObject (hdcMem, NP_BOMB);
						hBitBlt (hdc, xB2C(CurrentMB.getActive_Pt().x+(CurrentMB.getMBposition(i).x-CurrentMB.getMBposition(0).x)*SB_W),yB2C(CurrentMB.getActive_Pt().y+(CurrentMB.getMBposition(i).y-CurrentMB.getMBposition(0).y)*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_LEVIN:
					{
						SelectObject (hdcMem, NP_LEVIN);
						hBitBlt (hdc, xB2C(CurrentMB.getActive_Pt().x+(CurrentMB.getMBposition(i).x-CurrentMB.getMBposition(0).x)*SB_W),yB2C(CurrentMB.getActive_Pt().y+(CurrentMB.getMBposition(i).y-CurrentMB.getMBposition(0).y)*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_PENTACLE:
					{
						SelectObject (hdcMem, NP_PENTACLE);
						hBitBlt (hdc, xB2C(CurrentMB.getActive_Pt().x+(CurrentMB.getMBposition(i).x-CurrentMB.getMBposition(0).x)*SB_W),yB2C(CurrentMB.getActive_Pt().y+(CurrentMB.getMBposition(i).y-CurrentMB.getMBposition(0).y)*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_RED:
					{
						SelectObject (hdcMem, NP_RED);
						hBitBlt (hdc, xB2C(CurrentMB.getActive_Pt().x+(CurrentMB.getMBposition(i).x-CurrentMB.getMBposition(0).x)*SB_W),yB2C(CurrentMB.getActive_Pt().y+(CurrentMB.getMBposition(i).y-CurrentMB.getMBposition(0).y)*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_YELLOW:
					{
						SelectObject (hdcMem, NP_YELLOW);
						hBitBlt (hdc, xB2C(CurrentMB.getActive_Pt().x+(CurrentMB.getMBposition(i).x-CurrentMB.getMBposition(0).x)*SB_W),yB2C(CurrentMB.getActive_Pt().y+(CurrentMB.getMBposition(i).y-CurrentMB.getMBposition(0).y)*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_BLUE:
					{
						SelectObject (hdcMem, NP_BLUE);
						hBitBlt (hdc, xB2C(CurrentMB.getActive_Pt().x+(CurrentMB.getMBposition(i).x-CurrentMB.getMBposition(0).x)*SB_W),yB2C(CurrentMB.getActive_Pt().y+(CurrentMB.getMBposition(i).y-CurrentMB.getMBposition(0).y)*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_GREEN:
					{
						SelectObject (hdcMem, NP_GREEN);
						hBitBlt (hdc, xB2C(CurrentMB.getActive_Pt().x+(CurrentMB.getMBposition(i).x-CurrentMB.getMBposition(0).x)*SB_W),yB2C(CurrentMB.getActive_Pt().y+(CurrentMB.getMBposition(i).y-CurrentMB.getMBposition(0).y)*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_PURPLE:
					{
						SelectObject (hdcMem, NP_PURPLE);
						hBitBlt (hdc, xB2C(CurrentMB.getActive_Pt().x+(CurrentMB.getMBposition(i).x-CurrentMB.getMBposition(0).x)*SB_W),yB2C(CurrentMB.getActive_Pt().y+(CurrentMB.getMBposition(i).y-CurrentMB.getMBposition(0).y)*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_STONE:
					{
						SelectObject (hdcMem, NP_STONE);
						hBitBlt (hdc, xB2C(CurrentMB.getActive_Pt().x+(CurrentMB.getMBposition(i).x-CurrentMB.getMBposition(0).x)*SB_W),yB2C(CurrentMB.getActive_Pt().y+(CurrentMB.getMBposition(i).y-CurrentMB.getMBposition(0).y)*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_BLANK:
					{
						SelectObject (hdcMem, NP_BLANK);
						hBitBlt (hdc, xB2C(CurrentMB.getActive_Pt().x+(CurrentMB.getMBposition(i).x-CurrentMB.getMBposition(0).x)*SB_W),yB2C(CurrentMB.getActive_Pt().y+(CurrentMB.getMBposition(i).y-CurrentMB.getMBposition(0).y)*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				default:
					break;
			}
		}
	/*	switch(CurrentMB.getKind()){
			case MB_1:case MB_2H:case MB_3H:
				if(CurrentMB.getActive_Pt().y>191)
				{
					SelectObject (hdcMem, NP_HEAD);
					BitBlt (hdc,8,0,144,14, hdcMem, 0, 0, SRCCOPY);
				}
				break;
			case MB_2V:case MB_3L:case MB_3R:
				if(CurrentMB.getActive_Pt().y>167)
				{
					SelectObject (hdcMem, NP_HEAD);
					BitBlt (hdc,8,0,144,14, hdcMem, 0, 0, SRCCOPY);
				}
				break;
			case MB_3V:
				if(CurrentMB.getActive_Pt().y>143)
				{
					SelectObject (hdcMem, NP_HEAD);
					BitBlt (hdc,8,0,144,14, hdcMem, 0, 0, SRCCOPY);
				}
				break;
			default:
				break;
		}*/
		DeleteDC(hdcMem);
	}
	void DisplayNextMB(HDC &hdc)
	{
		HDC hdcMem = CreateCompatibleDC (hdc);
		//161，14
		SelectObject (hdcMem, NP_SMBB);
		BitBlt (hdc, 161,14,3*SB_W,3*SB_H, hdcMem, 0, 0, SRCCOPY);
		//
		switch(NextMB.getKind()){
			case MB_1:
				DisplaySB_r(hdc,hdcMem,NextMB_X,NextMB_Y,NextMB.getMBstate(0));
				break;
			case MB_2H:
				DisplaySB_r(hdc,hdcMem,NextMB_X,NextMB_Y,NextMB.getMBstate(0));
				DisplaySB_r(hdc,hdcMem,NextMB_X-SB_W,NextMB_Y,NextMB.getMBstate(1));
				break;
			case MB_2V:
				DisplaySB_r(hdc,hdcMem,NextMB_X,NextMB_Y,NextMB.getMBstate(0));
				DisplaySB_r(hdc,hdcMem,NextMB_X,NextMB_Y-SB_H,NextMB.getMBstate(1));
				break;
			case MB_3L:
				DisplaySB_r(hdc,hdcMem,NextMB_X,NextMB_Y,NextMB.getMBstate(0));
				DisplaySB_r(hdc,hdcMem,NextMB_X,NextMB_Y-SB_H,NextMB.getMBstate(1));
				DisplaySB_r(hdc,hdcMem,NextMB_X-SB_W,NextMB_Y,NextMB.getMBstate(2));
				break;
			case MB_3R:
				DisplaySB_r(hdc,hdcMem,NextMB_X,NextMB_Y,NextMB.getMBstate(0));
				DisplaySB_r(hdc,hdcMem,NextMB_X,NextMB_Y-SB_H,NextMB.getMBstate(1));
				DisplaySB_r(hdc,hdcMem,NextMB_X+SB_W,NextMB_Y,NextMB.getMBstate(2));
				break;
			case MB_3H:
				DisplaySB_r(hdc,hdcMem,NextMB_X,NextMB_Y,NextMB.getMBstate(0));
				DisplaySB_r(hdc,hdcMem,NextMB_X-SB_W,NextMB_Y,NextMB.getMBstate(1));
				DisplaySB_r(hdc,hdcMem,NextMB_X+SB_W,NextMB_Y,NextMB.getMBstate(2));
				break;
			case MB_3V:
				DisplaySB_r(hdc,hdcMem,NextMB_X,NextMB_Y+SB_H,NextMB.getMBstate(0));
				DisplaySB_r(hdc,hdcMem,NextMB_X,NextMB_Y,NextMB.getMBstate(1));
				DisplaySB_r(hdc,hdcMem,NextMB_X,NextMB_Y-SB_H,NextMB.getMBstate(2));
				break;
			default:
				break;
		}
		DeleteDC(hdcMem);
	}
	void DisplayMotherBoard(HDC &hdc)
	{
		HDC hdcMem = CreateCompatibleDC (hdc);
		SelectObject (hdcMem, NP_BB_1);
		BitBlt (hdc, 0, 0, 154,	268, hdcMem, 0, 0, SRCCOPY);
		SelectObject (hdcMem, NP_BB_2);
		BitBlt (hdc, 154, 0,86,268, hdcMem,0,0, SRCCOPY);
		DeleteDC(hdcMem);
	}
	void DisplayBB(HDC &hdc)
	{
		//DrawState(hdc,NULL,NULL,(LPARAM)NP_DEADLINE,0,25,0,180,25,DST_BITMAP);
		/*RECT rectLevel,rectScore;
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
		DrawText(hdc,outScore,-1,&rectScore,DT_CENTER);*/

		for(int i=0;i<n_width;i++)
			for(int j=0;j<HighLimit+1;j++)
				switch(column[i][j].getSbstate())
			{
				case SB_BOMB:
					{
						HDC hdcMem = CreateCompatibleDC (hdc);
						SelectObject (hdcMem, NP_BOMB);
						BitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
						DeleteDC(hdcMem);
					}
					break;
				case SB_LEVIN:
					{
						HDC hdcMem = CreateCompatibleDC (hdc);
						SelectObject (hdcMem, NP_LEVIN);
						BitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
						DeleteDC(hdcMem);
					}
					break;
				case SB_PENTACLE:
					{
						HDC hdcMem = CreateCompatibleDC (hdc);
						SelectObject (hdcMem, NP_PENTACLE);
						BitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
						DeleteDC(hdcMem);
					}
					break;
				case SB_RED:
					{
						HDC hdcMem = CreateCompatibleDC (hdc);
						SelectObject (hdcMem, NP_RED);
						BitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
						DeleteDC(hdcMem);
					}
					break;
				case SB_YELLOW:
					{
						HDC hdcMem = CreateCompatibleDC (hdc);
						SelectObject (hdcMem, NP_YELLOW);
						BitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
						DeleteDC(hdcMem);
					}
					break;
				case SB_BLUE:
					{
						HDC hdcMem = CreateCompatibleDC (hdc);
						SelectObject (hdcMem, NP_BLUE);
						BitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
						DeleteDC(hdcMem);
					}
					break;
				case SB_GREEN:
					{
						HDC hdcMem = CreateCompatibleDC (hdc);
						SelectObject (hdcMem, NP_GREEN);
						BitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
						DeleteDC(hdcMem);
					}
					break;
				case SB_PURPLE:
					{
						HDC hdcMem = CreateCompatibleDC (hdc);
						SelectObject (hdcMem, NP_PURPLE);
						BitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
						DeleteDC(hdcMem);
					}
					break;
				case SB_STONE:
					{
						HDC hdcMem = CreateCompatibleDC (hdc);
						SelectObject (hdcMem, NP_STONE);
						BitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
						DeleteDC(hdcMem);
					}
					break;
				case SB_BLANK:
					{
						HDC hdcMem = CreateCompatibleDC (hdc);
						SelectObject (hdcMem, NP_BLANK);
						BitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
						DeleteDC(hdcMem);
					}
					break;
				default:
					break;
			}
	}

	void DisplayBlank4FallStep(HDC &hdc)
	{
		HDC hdcMem = CreateCompatibleDC (hdc);
		switch(CurrentMB.getKind()){
			case MB_1:
				if(CurrentMB.getMBposition(0).y<9){
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0),SB_BLANK);
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0).x,CurrentMB.getMBposition(0).y+1,SB_BLANK);
				}
				break;
			case MB_2H:
				if(CurrentMB.getMBposition(0).y<9){
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0),SB_BLANK);
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1),SB_BLANK);
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0).x,CurrentMB.getMBposition(0).y+1,SB_BLANK);
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x,CurrentMB.getMBposition(1).y+1,SB_BLANK);
				}
				break;
			case MB_2V:
				if(CurrentMB.getMBposition(1).y<9){
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1),SB_BLANK);
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x,CurrentMB.getMBposition(1).y+1,SB_BLANK);
				}
				break;
			case MB_3L:
				if(CurrentMB.getMBposition(1).y<9){
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1),SB_BLANK);
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x,CurrentMB.getMBposition(1).y+1,SB_BLANK);
				}
				if(CurrentMB.getMBposition(2).y<9){
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2),SB_BLANK);
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2).x,CurrentMB.getMBposition(2).y+1,SB_BLANK);
				}
				break;
			case MB_3R:
				if(CurrentMB.getMBposition(1).y<9){
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1),SB_BLANK);
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x,CurrentMB.getMBposition(1).y+1,SB_BLANK);
				}
				if(CurrentMB.getMBposition(2).y<9){
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2),SB_BLANK);
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2).x,CurrentMB.getMBposition(2).y+1,SB_BLANK);
				}
				break;
			case MB_3H:
				if(CurrentMB.getMBposition(0).y<9){
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0),SB_BLANK);
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1),SB_BLANK);
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2),SB_BLANK);
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0).x,CurrentMB.getMBposition(0).y+1,SB_BLANK);
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x,CurrentMB.getMBposition(1).y+1,SB_BLANK);
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2).x,CurrentMB.getMBposition(2).y+1,SB_BLANK);
				}
				break;
			case MB_3V:
				if(CurrentMB.getMBposition(2).y<9){
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2),SB_BLANK);
					DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2).x,CurrentMB.getMBposition(2).y+1,SB_BLANK);
				}
				break;
			default:
				break;
		}
	/*	switch(CurrentMB.getKind()){
			case MB_1:case MB_2H:case MB_3H:
				if(CurrentMB.getActive_Pt().y>191)
				{
					SelectObject (hdcMem, NP_HEAD);
					BitBlt (hdc,8,0,144,14, hdcMem, 0, 0, SRCCOPY);
				}
				break;
			case MB_2V:case MB_3L:case MB_3R:
				if(CurrentMB.getActive_Pt().y>167)
				{
					SelectObject (hdcMem, NP_HEAD);
					BitBlt (hdc,8,0,144,14, hdcMem, 0, 0, SRCCOPY);
				}
				break;
			case MB_3V:
				if(CurrentMB.getActive_Pt().y>143)
				{
					SelectObject (hdcMem, NP_HEAD);
					BitBlt (hdc,8,0,144,14, hdcMem, 0, 0, SRCCOPY);
				}
				break;
			default:
				break;
		}*/
		DeleteDC(hdcMem);
	}
	void DisplayBlank4MoveLeft(HDC &hdc)
	{
		HDC hdcMem = CreateCompatibleDC (hdc);
		switch(CurrentMB.getKind()){
			case MB_1:
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0).x+1,CurrentMB.getMBposition(0).y,SB_BLANK);
				if(CurrentMB.getMBposition(0).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0).x+1,CurrentMB.getMBposition(0).y+1,SB_BLANK);
				break;
			case MB_2H:
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0).x+1,CurrentMB.getMBposition(0).y,SB_BLANK);
				if(CurrentMB.getMBposition(0).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0).x+1,CurrentMB.getMBposition(0).y+1,SB_BLANK);
				break;
			case MB_2V:
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0).x+1,CurrentMB.getMBposition(0).y,SB_BLANK);
				if(CurrentMB.getMBposition(0).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x+1,CurrentMB.getMBposition(1).y,SB_BLANK);
				if(CurrentMB.getMBposition(1).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x+1,CurrentMB.getMBposition(1).y+1,SB_BLANK);
				break;
			case MB_3L:
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0).x+1,CurrentMB.getMBposition(0).y,SB_BLANK);
				if(CurrentMB.getMBposition(0).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x+1,CurrentMB.getMBposition(1).y,SB_BLANK);
				if(CurrentMB.getMBposition(1).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x+1,CurrentMB.getMBposition(1).y+1,SB_BLANK);
				break;
			case MB_3R:
				if(CurrentMB.getMBposition(0).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x+1,CurrentMB.getMBposition(1).y,SB_BLANK);
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2).x+1,CurrentMB.getMBposition(2).y,SB_BLANK);
				if(CurrentMB.getMBposition(1).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x+1,CurrentMB.getMBposition(1).y+1,SB_BLANK);
				if(CurrentMB.getMBposition(2).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2).x+1,CurrentMB.getMBposition(2).y+1,SB_BLANK);
				break;
			case MB_3H:
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2).x+1,CurrentMB.getMBposition(2).y,SB_BLANK);
				if(CurrentMB.getMBposition(2).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2).x+1,CurrentMB.getMBposition(2).y+1,SB_BLANK);
				break;
			case MB_3V:
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0).x+1,CurrentMB.getMBposition(0).y,SB_BLANK);;
				if(CurrentMB.getMBposition(0).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x+1,CurrentMB.getMBposition(1).y,SB_BLANK);
				if(CurrentMB.getMBposition(1).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2).x+1,CurrentMB.getMBposition(2).y,SB_BLANK);
				if(CurrentMB.getMBposition(2).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2).x+1,CurrentMB.getMBposition(2).y+1,SB_BLANK);
				break;
		}/*
		switch(CurrentMB.getKind()){
			case MB_1:case MB_2H:case MB_3H:
				if(CurrentMB.getActive_Pt().y>191)
				{
					SelectObject (hdcMem, NP_HEAD);
					BitBlt (hdc,8,0,144,14, hdcMem, 0, 0, SRCCOPY);
				}
				break;
			case MB_2V:case MB_3L:case MB_3R:
				if(CurrentMB.getActive_Pt().y>167)
				{
					SelectObject (hdcMem, NP_HEAD);
					BitBlt (hdc,8,0,144,14, hdcMem, 0, 0, SRCCOPY);
				}
				break;
			case MB_3V:
				if(CurrentMB.getActive_Pt().y>143)
				{
					SelectObject (hdcMem, NP_HEAD);
					BitBlt (hdc,8,0,144,14, hdcMem, 0, 0, SRCCOPY);
				}
				break;
			default:
				break;
		}*/
		DeleteDC(hdcMem);
	}
	void DisplayBlank4MoveRight(HDC &hdc)
	{
		HDC hdcMem = CreateCompatibleDC (hdc);
		switch(CurrentMB.getKind()){

			case MB_1:
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0).x-1,CurrentMB.getMBposition(0).y,SB_BLANK);
				if(CurrentMB.getMBposition(0).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0).x-1,CurrentMB.getMBposition(0).y+1,SB_BLANK);
				break;
			case MB_2H:
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x-1,CurrentMB.getMBposition(1).y,SB_BLANK);
				if(CurrentMB.getMBposition(1).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x-1,CurrentMB.getMBposition(1).y+1,SB_BLANK);
				break;
			case MB_2V:
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0).x-1,CurrentMB.getMBposition(0).y,SB_BLANK);;
				if(CurrentMB.getMBposition(0).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x-1,CurrentMB.getMBposition(1).y,SB_BLANK);
				if(CurrentMB.getMBposition(1).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x-1,CurrentMB.getMBposition(1).y+1,SB_BLANK);
				break;
				break;
			case MB_3L:
				if(CurrentMB.getMBposition(0).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x-1,CurrentMB.getMBposition(1).y,SB_BLANK);
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2).x-1,CurrentMB.getMBposition(2).y,SB_BLANK);
				if(CurrentMB.getMBposition(1).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x-1,CurrentMB.getMBposition(1).y+1,SB_BLANK);
				if(CurrentMB.getMBposition(2).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2).x-1,CurrentMB.getMBposition(2).y+1,SB_BLANK);
				break;
			case MB_3R:	
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0).x-1,CurrentMB.getMBposition(0).y,SB_BLANK);
				if(CurrentMB.getMBposition(0).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x-1,CurrentMB.getMBposition(1).y,SB_BLANK);
				if(CurrentMB.getMBposition(1).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x-1,CurrentMB.getMBposition(1).y+1,SB_BLANK);
				break;
			case MB_3H:
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x-1,CurrentMB.getMBposition(1).y,SB_BLANK);
				if(CurrentMB.getMBposition(0).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x-1,CurrentMB.getMBposition(1).y+1,SB_BLANK);
				break;
			case MB_3V:
				DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(0).x-1,CurrentMB.getMBposition(0).y,SB_BLANK);
				if(CurrentMB.getMBposition(0).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(1).x-1,CurrentMB.getMBposition(1).y,SB_BLANK);
				if(CurrentMB.getMBposition(1).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2).x-1,CurrentMB.getMBposition(2).y,SB_BLANK);
				if(CurrentMB.getMBposition(2).y<9) DisplaySB(hdc,hdcMem,CurrentMB.getMBposition(2).x-1,CurrentMB.getMBposition(2).y+1,SB_BLANK);
				break;
		}/*
		switch(CurrentMB.getKind()){
			case MB_1:case MB_2H:case MB_3H:
				if(CurrentMB.getActive_Pt().y>191)
				{
					SelectObject (hdcMem, NP_HEAD);
					BitBlt (hdc,8,0,144,14, hdcMem, 0, 0, SRCCOPY);
				}
				break;
			case MB_2V:case MB_3L:case MB_3R:
				if(CurrentMB.getActive_Pt().y>167)
				{
					SelectObject (hdcMem, NP_HEAD);
					BitBlt (hdc,8,0,144,14, hdcMem, 0, 0, SRCCOPY);
				}
				break;
			case MB_3V:
				if(CurrentMB.getActive_Pt().y>143)
				{
					SelectObject (hdcMem, NP_HEAD);
					BitBlt (hdc,8,0,144,14, hdcMem, 0, 0, SRCCOPY);
				}
				break;
			default:
				break;
		}*/
		DeleteDC(hdcMem);
	}
	void Display4FallDown(HDC &hdc)
	{
		HDC hdcMem = CreateCompatibleDC(hdc);
		for(int i=0;i<n_width;i++)
			for(int j=0;j<HighLimit+1;j++)
				switch(column[i][j].getSbstate())
			{
				case SB_BOMB:
					{
						SelectObject (hdcMem, NP_BOMB);
						hBitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_LEVIN:
					{
						SelectObject (hdcMem, NP_LEVIN);
						hBitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_PENTACLE:
					{
						SelectObject (hdcMem, NP_PENTACLE);
						hBitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_RED:
					{
						SelectObject (hdcMem, NP_RED);
						hBitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_YELLOW:
					{
						SelectObject (hdcMem, NP_YELLOW);
						hBitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_BLUE:
					{
						SelectObject (hdcMem, NP_BLUE);
						hBitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_GREEN:
					{
						SelectObject (hdcMem, NP_GREEN);
						hBitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_PURPLE:
					{
						SelectObject (hdcMem, NP_PURPLE);
						hBitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_STONE:
					{
						SelectObject (hdcMem, NP_STONE);
						hBitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				case SB_BLANK:
					{
						SelectObject (hdcMem, NP_BLANK);
						hBitBlt (hdc, xB2C(i*SB_W),yB2C(j*SB_H),SB_W,SB_H, hdcMem, 0, 0, SRCCOPY);
					}
					break;
				default:
					break;
			}
			DeleteDC(hdcMem);
		
	}

	void DisplayGameOver(HDC &hdc)
	{//36,46
		HDC hdcMem = CreateCompatibleDC(hdc);
		SelectObject (hdcMem, NP_GAMEOVER);
		BitBlt (hdc, 36,46,88,56, hdcMem, 0, 0, SRCCOPY);
		DeleteDC(hdcMem);
	}
	void DisplayErasing(HDC &hdc)
	{	
		HDC hdcMem = CreateCompatibleDC(hdc);
		if(ErasingTimes>1)
		{//12,18
			RECT rectErasingTimes;
			rectErasingTimes.left=22;
			rectErasingTimes.top=28;
			rectErasingTimes.right=42;
			rectErasingTimes.bottom=48;
			wchar_t outErasingTimes[4];
			_itow_s(ErasingTimes-1,outErasingTimes,4,10); 
			//DrawState(hdc,NULL,NULL,(LPARAM)NP_ERASINGTIMES,0,30,30,50,50,DST_BITMAP);
			//DrawState(hdc,NULL,NULL,(LPARAM)NP_GOOD,0,229,152,45,25,DST_BITMAP);
			SelectObject (hdcMem, NP_ERASINGTIMES);
			BitBlt (hdc, 12,18,40,40, hdcMem, 0, 0, SRCCOPY);
			SetTextColor(hdc,0x00ff0000);
			SetBkColor(hdc,0x00000000) ;
			//设置字体大小
			DrawText(hdc,outErasingTimes,-1,&rectErasingTimes,DT_CENTER);
		}
		Sleep(150);
		for(int i=0;i<n_width;i++)
			for(int j=0;j<n_height;j++)
				if(erase4display[i][j])
					DisplaySB(hdc,hdcMem,i,j,NP_ERASING_1);
		Sleep(150);
		for(int i=0;i<n_width;i++)
			for(int j=0;j<n_height;j++)
				if(erase4display[i][j])
					DisplaySB(hdc,hdcMem,i,j,NP_ERASING_2);
		Sleep(150);
		for(int i=0;i<n_width;i++)
			for(int j=0;j<n_height;j++)
				if(erase4display[i][j])
					DisplaySB(hdc,hdcMem,i,j,NP_ERASING_3);
		Sleep(150);
		for(int i=0;i<n_width;i++)
			for(int j=0;j<n_height;j++)
				if(erase4display[i][j])
					DisplaySB(hdc,hdcMem,i,j,NP_ERASING_4);
		Sleep(150);
		for(int i=0;i<n_width;i++)
			for(int j=0;j<n_height;j++)
				if(erase4display[i][j]) 
					erase4display[i][j]=false;
		DeleteDC(hdcMem);
	}

	void DisplayScore(HDC &hdc)
	{
		RECT rectScore;
			rectScore.left=185;
			rectScore.top=195;
			rectScore.right=230;
			rectScore.bottom=210;
			wchar_t outScore[12];
			_itow_s(Score,outScore,12,10); 
			HDC hdcMem = CreateCompatibleDC (hdc);
			SelectObject (hdcMem, NP_SCORE);
			BitBlt (hdc, 161,166,72,44, hdcMem, 0, 0, SRCCOPY);
			SetTextColor(hdc,0x00ff0000);
			SetBkColor(hdc,0x0080ffff) ;
			//设置字体大小
			DrawText(hdc,outScore,-1,&rectScore,DT_CENTER);
			DeleteDC(hdcMem);
	}
	void DisplayLevel(HDC &hdc)
	{
			RECT rectLevel;
			rectLevel.left=185;
			rectLevel.top=230;
			rectLevel.right=230;
			rectLevel.bottom=245;
			wchar_t outLevel[12];
			_itow_s(Level,outLevel,12,10); 
			HDC hdcMem = CreateCompatibleDC (hdc);
			SelectObject (hdcMem, NP_LEVEL);
			BitBlt (hdc, 161,210,72,44, hdcMem, 0, 0, SRCCOPY);
			SetTextColor(hdc,0x00ff0000);
			SetBkColor(hdc,0x0080ffff) ;
			//设置字体大小
			DrawText(hdc,outLevel,-1,&rectLevel,DT_CENTER);
			DeleteDC(hdcMem);
	}
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
	static const int HighLimit=nBB_LH-1;
	SingleBlock column[n_width][n_height];
	vector<POINT> ErasingVec;
	POINT Active_Pt;
	bool dead_flag;
	bool need_erase;
	int next_step;
	int step_length;
	int time;
	POINT DeadPosition;
	SBState DeadState;
	int Score;
	int ErasingTimes;
	int MaxErasingTimes;
	int Level;
	int nMB;
	bool erase4display[n_width][n_height];

	bool WillSBDead(int suffix)
	{
		if( (!column[CurrentMB.getMBposition(suffix).x][CurrentMB.getMBposition(suffix).y-1].isSBEmpty()) && (CurrentMB.getMBposition(suffix).y*SB_H==(CurrentMB.getActive_Pt().y - 0)) ) return true;
		else if(CurrentMB.getActive_Pt().y==0) return true;
		else return false;
	}
	BOOL hBitBlt(HDC hdcDest,int nXDest,int nYDest,int nWidth,int nHeight,HDC hdcSrc,int nXSrc,int nYSrc,DWORD dwRop)
	{
		if(nYDest<14)
		{
			int dHeight=14-nYDest;
			nYDest=14;
			nYSrc=dHeight;
		}
		return BitBlt(hdcDest,nXDest,nYDest,nWidth,nHeight,hdcSrc,nXSrc,nYSrc,dwRop);
	}
	void DisplaySB(HDC &hdc,HDC &mHdc,POINT &pos,SBState sbstate)
	{
		HBITMAP H_NP[]={NP_BLANK,NP_RED,NP_YELLOW,NP_BLUE,NP_GREEN,NP_PURPLE,NP_STONE,NP_BOMB,NP_LEVIN,NP_PENTACLE};
		SelectObject (mHdc, H_NP[sbstate]);
		hBitBlt (hdc, xB2C(pos.x*SB_H),yB2C(pos.y*SB_W),SB_H,SB_W, mHdc, 0, 0, SRCCOPY);
	}
	void DisplaySB_r(HDC &hdc,HDC &mHdc,int real_x,int real_y,SBState sbstate)
	{
		HBITMAP H_NP[]={NP_BLANK,NP_RED,NP_YELLOW,NP_BLUE,NP_GREEN,NP_PURPLE,NP_STONE,NP_BOMB,NP_LEVIN,NP_PENTACLE};
		SelectObject (mHdc, H_NP[sbstate]);
		BitBlt (hdc, real_x,real_y,SB_H,SB_W, mHdc, 0, 0, SRCCOPY);
	}
	void DisplaySB(HDC &hdc,HDC &mHdc,int block_x,int block_y,SBState sbstate)
	{
		HBITMAP H_NP[]={NP_BLANK,NP_RED,NP_YELLOW,NP_BLUE,NP_GREEN,NP_PURPLE,NP_STONE,NP_BOMB,NP_LEVIN,NP_PENTACLE};
		SelectObject (mHdc, H_NP[sbstate]);
		hBitBlt (hdc,  xB2C(block_x*SB_H),yB2C(block_y*SB_W),SB_H,SB_W, mHdc, 0, 0, SRCCOPY);
	}
	void DisplaySB(HDC &hdc,HDC &mHdc,int block_x,int block_y,HBITMAP &hBitmap)
	{
		SelectObject (mHdc,hBitmap);
		hBitBlt (hdc,  xB2C(block_x*SB_H),yB2C(block_y*SB_W),SB_H,SB_W, mHdc, 0, 0, SRCCOPY);
	}
	void SpeedUp()
	{
		switch(Level){
			case 1:
				time=120;
				step_length=1;
				break;
			case 2:
				time=100;
				step_length=1;
				break;
			case 3:
				time=80;
				step_length=1;
				break;
			case 4:
				time=60;
				step_length=1;
				break;
			case 5:
				time=40;
				step_length=1;
				break;
			case 6:
				time=20;
				step_length=1;
				break;
			case 7:
				time=20;
				step_length=2;
				break;
			case 8:
				time=20;
				step_length=3;
				break;
			case 9:
				time=20;
				step_length=4;
				break;
			case 10:
				time=20;
				step_length=6;
				break;
			default:
				break;
		}
	}
};