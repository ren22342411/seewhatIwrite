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

extern HBITMAP NP_BB;
extern HBITMAP NP_COVER;
extern HBITMAP NP_BLANK;
extern HBITMAP NP_RED;
extern HBITMAP NP_YELLOW;
extern HBITMAP NP_BLUE;
extern HBITMAP NP_GREEN;
extern HBITMAP NP_PURPLE;
extern HBITMAP NP_STONE;
extern HBITMAP NP_BOMB;
extern HBITMAP NP_LEVIN;
extern HBITMAP NP_PENTACLE;
extern HBITMAP NP_ERASING_1;
extern HBITMAP NP_ERASING_2;
extern HBITMAP NP_ERASING_3;
extern HBITMAP NP_ERASING_4;
extern HBITMAP NP_DEADLINE;
extern HBITMAP NP_SMBB;
extern HBITMAP NP_GAMEOVER;
extern HBITMAP NP_ERASINGTIMES;
extern HBITMAP NP_GOOD;
extern HBITMAP NP_BKG;

typedef struct
{
	int x;
	int y;
}SBPosition;

int MyRandom();

class SingleBlock
{
	friend class MultiBlock;//for SAVE and LOAD
	friend class BlockBoard;//for SAVE and LOAD
public:
	SingleBlock(SingleBlock &rhs);
	SingleBlock(int x=0,int y=0);
	SingleBlock(SBState sbstate,int x=0,int y=0);
	SBState GetState();
	SBPosition GetPosition();
	int GetOrder();
	void PutState( SBState st );
	void PutPosition(SBPosition &pos);
	void PutPosition(int x,int y);
	void SetOrder(int i);
	bool isSBEmpty();
	void makeSBEmpty();
	SingleBlock operator=(SingleBlock &rhs);

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
	MultiBlock();
	void MBInitial( MBKind mbk );
	void Transposition();
	MBKind GetKind();
	int GetNumSB();
	int GetLeft();
	int GetRight();
	int GetHigh();

	MultiBlock operator=(MultiBlock &rhs);


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
	BlockBoard();

	SBState GetSBState( SBPosition pos );
	SBState GetSBState( int x,int y);
	void GenerateMB(MultiBlock &MB);

	void MBMoveLeft();
	void MBMoveRight();
	void MBTransposition();
	void MBFallDown();
	void MBFallStep();
	bool WillMBDead();
	SBState ReceiveWhat();
	SBState ReceiveWhat( int i);
	void Find4NormalErasing();
	void Find4BOMBErasing(SBPosition &pos);
	void Find4LEVINErasing(SBPosition &pos);
	void Find4PENTACLEErasing(SBPosition &pos);
	void Erasing();
	void DisplayErasing(HDC &hdc);
	void DisplayCurrentMB(HDC &hdc);
	void DisplayNextMB(HDC &hdc);
	void DisplayBB(HDC &hdc);

	int GetSpeed();
	int GetLevel();
	int GetMaxErasingTimes();
	int GetnMB();
	int GetScore();
	void BBInitial();
	SBPosition GetDeadPosition();
	bool NeedErasing();
	bool IsLevelUp();
	bool IsGameOver();
	void MB_Next2Current();

	void DisplayGameOver(HDC &hdc);
	void SAVE(HANDLE &hFile);
	void LOAD(HANDLE &hFile);


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
	bool WillSBDead(int suffix);
};
class Record
{
	friend void BlockBoard::Read_Dispaly_Record(HDC &);
public:
	Record();
	Record(BlockBoard &rhs);
	void Record2File();

	int GetScore();
	int GetMaxErasingTimes();
	int GetLevel();
	int GetnMB();
	bool operator<(Record &rhs);
	wchar_t * GetTime();
private:
	int Score;
	int MaxErasingTimes;
	int Level;
	int nMB; 
	wchar_t Time[20];
};
