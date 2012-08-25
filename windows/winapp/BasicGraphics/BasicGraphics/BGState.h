#ifndef BGSTATE
#define BGSTATE
#include <vector>

enum EditState {DRAW_LINE,DRAW_CIRCLE,DRAW_RECTANGULAR,DRAW_POLYGON,DRAW_TEXT,SELECT,ZOOM};
enum ZoomValue {ZOOM_25 = -3,ZOOM_50 = -2,ZOOM_75 = -1,ZOOM_100 = 0,ZOOM_150 = 1,ZOOM_200 = 2,ZOOM_400 = 3};
class BGState
{
public:
	BGState();
	EditState getES();
	void setES(EditState);
	int TimesOfLBD();
	int TimesOfRBD();
	void PPTimesOfLBD();
	void PPTimesOfRBD();
	bool isLBD0();
	bool isLBD1();
	bool isLBDMoreThan3();
	void setLBD0();
	void AddLBDPoint(int,int);
	void AddLBDPoint(POINT);
	bool getLastPoint(POINT &);
	bool getPointByIndex(POINT &,int );
	bool LBD(int,int);
	bool LBD(POINT);
	void ESReset();
	void CopyPointV(std::vector<POINT> &);
	ZoomValue getZoomValue();
	bool ZoomIn();
	bool ZoomOut();

private:
	void ESInitial();
	void EditStateReset();
	EditState edit_state;
	int LeftButtonDown;
	int RightButtonDown;
	ZoomValue zoom_value;
	std::vector<POINT> vPoint_LBD;

	//int LeftButtonDoubleClick;
};
#endif