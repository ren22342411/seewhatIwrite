#include "stdafx.h"
#include "math.h"
#include "BGState.h"
#include "Resource.h"
#include <vector>
#include <list>

enum GraphicsType {OBJECT,LINE,CIRCLE,RECTANGULAR,POLYGON,TEXT};

typedef struct _MyLineStruct
{
	int pen_style;
	int line_width;
	COLORREF color;
	bool selected_flag;
	int SpecialPointRadius;
	POINT start;
	POINT end;
}MyLineStruct;

typedef struct _MyCircleStruct
{
	int pen_style;
	int line_width;
	COLORREF color;
	bool selected_flag;
	int SpecialPointRadius;
	POINT Central;
	int Radius;
}MyCircleStruct;

typedef struct _MyRectStruct
{
	int pen_style;
	int line_width;
	COLORREF color;
	bool selected_flag;
	int SpecialPointRadius;
	int nLeftRect;
	int nTopRect;
	int nRightRect;
	int nBottomRect;
}MyRectStruct;

typedef struct _MyPolyStruct
{
	int pen_style;
	int line_width;
	COLORREF color;
	bool selected_flag;
	int SpecialPointRadius;
	int points_size;
	POINT Points[128];
}MyPolyStruct;

typedef struct _MyTextStruct
{
	int pen_style;
	int line_width;
	COLORREF color;
	bool selected_flag;
	int SpecialPointRadius;
	int text_lenth;
	wchar_t text[128];
	RECT text_area;
	LOGFONT logfont;
}MyTextStruct;

class GraphicsObject
{
public:
	GraphicsObject()
	{
		//OutputDebugString(L"GraphicsObject()\n");
		pen_style = DefaltPenStyle;
		line_width = DefaltLineWidth;
		color = DefaltColor;
		selected_flag = DefaultSelectedFlag;
		SpecialPointRadius = 4;
	}
/*
	GraphicsObject(int ps,int lw,COLORREF clr,bool sf)
	{
		//OutputDebugString(L"GraphicsObject(int ps,int lw,COLORREF clr)\n");
		pen_style = ps;
		line_width = lw;
		color = clr;
		selected_flag = sf;
	}

	GraphicsObject(GraphicsObject &go)
	{
		//OutputDebugString(L"GraphicsObject(GraphicsObject &go)\n");
		pen_style = go.pen_style;
		line_width = go.line_width;
		color = go.color;
		selected_flag = go.selected_flag;
	}

	*/
	virtual BOOL Save(HANDLE hFile) { return FALSE; }
	virtual BOOL Load(HANDLE hFile) { return FALSE; }
	virtual void display(HDC hdc/*,ZoomValue zv*/) {}
	virtual bool didSelectMe(POINT &pt) const { return false; }
	virtual void getFreshRect(RECT &fRect){}
	virtual void getFreshRect(RECT &fRect,HWND hWnd){}
	virtual GraphicsType getGraphicsType() const { return OBJECT; };
	
	void FreshMyArea(HWND hWnd,BOOL bErase)
	{
		RECT temp;
		if( this->getGraphicsType() == TEXT )
			this->getFreshRect(temp,hWnd);
		else
			this->getFreshRect(temp);
		InvalidateRect(hWnd,&temp,bErase);
	}

	COLORREF getColor() const
	{
		return color;
	}

	void setColor(COLORREF clr)
	{
		color = clr;
	}

	int getPenStyle() const
	{
		return pen_style;
	}

	void setPenStyle(int ps)
	{
		pen_style = ps;
	}

	int getLineWidth() const
	{
		return line_width;
	}

	void setLineWidth(int lw)
	{
		line_width = lw;
	}

	bool isSelected() const
	{
		return selected_flag;
	}

	void setSelectedFlag(bool sf)
	{
		selected_flag = sf;
	}

	static COLORREF getDefaultColor() 
	{
		return DefaltColor;
	}

	static void setDefaultColor(COLORREF clr)
	{
		DefaltColor = clr;
	}

	static int getDefaultPenStyle() 
	{
		return DefaltPenStyle;
	}

	static void setDefaultPenStyle(int ps)
	{
		DefaltPenStyle = ps;
	}

	static int getDefaultLineWidth() 
	{
		return DefaltLineWidth;
	}

	static void setDefaultLineWidth(int lw)
	{
		DefaltLineWidth = lw;
	}

	static int getSelectingSensitivity() 
	{
		return SelectingSensitivity;
	}

	static void setSelectingSensitivity(int ss)
	{
		SelectingSensitivity = ss;
	}

	void setSPR(int r)
	{
		if(r>=1)
		{
			SpecialPointRadius = r;
		}
	}

	int getSPR() const
	{
		return SpecialPointRadius;
	}

protected:
	bool isPointNearLine(POINT start,POINT end,POINT &pt,double d = SelectingSensitivity) const
	{
		bool flag = false;
		if(start.x == end.x && start.y == end.y)
		{
			flag = d*d >= (double)( (pt.x-start.x)*(pt.x-start.x) + (pt.y-start.y)*(pt.y-start.y) );
		}
		else if(start.x == end.x && start.y != end.y)
		{
			if( (pt.y >= min(start.y,end.y) - 1)&&(pt.y <= max(start.y,end.y) + 1) )
			{
				flag = d >= (double)abs(pt.x - start.x);
			}
			else flag = false;
		}
		else if(start.x != end.x && start.y == end.y)
		{
			if( (pt.x >= min(start.x,end.x) - 1)&&(pt.x <= max(start.x,end.x) + 1))
			{
				flag = d >= (double)abs(pt.y - start.y);
			}
			else flag = false;
		}
		else
		{
			if( (pt.x >= min(start.x,end.x) - 1)&&(pt.x <= max(start.x,end.x) + 1) && (pt.y >= min(start.y,end.y) - 1)&&(pt.y <= max(start.y,end.y) + 1) )
			{
				double A,B,C,x1,y1,x2,y2;
				x1 = (double)start.x;
				y1 = (double)start.y;
				x2 = (double)end.x;
				y2 = (double)end.y;
				A = 1/(x2-x1);
				B = 1/(y1-y2);
				C = (x2*y1-x1*y2)/((y2-y1)*(x2-x1));
				flag = (d*d > ((A*(double)(pt.x)+B*(double)(pt.y)+C)*(A*(double)(pt.x)+B*(double)(pt.y)+C))/(A*A + B*B));
			}
			else flag = false;
		}

		return flag;
	}

	bool isPointNearLine(int startX,int startY,int endX,int endY,POINT &pt,double d = SelectingSensitivity) const
	{
		bool flag = false;
		if(startX == endX && startY == endY)
		{
			flag = d*d >= (double)( (pt.x-startX)*(pt.x-startX) + (pt.y-startY)*(pt.y-startY) );
		}
		else if(startX == endX && startY != endY)
		{
			if( (pt.y >= min(startY,endY) - 1)&&(pt.y <= max(startY,endY) + 1) )
			{
				flag = d >= (double)abs(pt.x - startX);
			}
			else flag = false;
		}
		else if(startX != endX && startY == endY)
		{
			if( (pt.x >= min(startX,endX) - 1)&&(pt.x <= max(startX,endX) + 1))
			{
				flag = d >= (double)abs(pt.y - startY);
			}
			else flag = false;
		}
		else
		{
			if( (pt.x >= min(startX,endX) - 1)&&(pt.x <= max(startX,endX) + 1) && (pt.y >= min(startY,endY) - 1)&&(pt.y <= max(startY,endY) + 1) )
			{
				double A,B,C,x1,y1,x2,y2;
				x1 = (double)startX;
				y1 = (double)startY;
				x2 = (double)endX;
				y2 = (double)endY;
				A = 1/(x2-x1);
				B = 1/(y1-y2);
				C = (x2*y1-x1*y2)/((y2-y1)*(x2-x1));
				flag = (d*d > ((A*(double)(pt.x)+B*(double)(pt.y)+C)*(A*(double)(pt.x)+B*(double)(pt.y)+C))/(A*A + B*B));
			}
			else flag = false;
		}

		return flag;
	}

	void drawSpecialPoint(HDC hdc,POINT pt) const
	{
		Ellipse(hdc,pt.x - SpecialPointRadius,pt.y - SpecialPointRadius,pt.x + SpecialPointRadius,pt.y + SpecialPointRadius);
	}
	void drawSpecialPoint(HDC hdc,int X,int Y) const
	{
		Ellipse( hdc, X - SpecialPointRadius, Y - SpecialPointRadius, X + SpecialPointRadius, Y + SpecialPointRadius );
	}

	
	static int SelectingSensitivity;
//private:
	int pen_style;
	int line_width;
	COLORREF color;
	bool selected_flag;
	int SpecialPointRadius;

	static COLORREF DefaltColor;
	static int DefaltPenStyle;
	static int DefaltLineWidth;
	static bool DefaultSelectedFlag;
};

class MyLine:public GraphicsObject
{
public:
	MyLine(){}
	MyLine(	POINT &s, POINT &e)
	{
		start = s;
		end = e;
	}
/*
	MyLine(	MyLine &ml )
	{
		start = ml.start;
		end = ml.end;
	}*/
	virtual void display(HDC hdc) 
	{
		//OutputDebugString(L"Draw Line\n");

		HPEN hPen,hOldPen;
		hPen = (HPEN)CreatePen(getPenStyle(),getLineWidth(),getColor());
		hOldPen = (HPEN)::SelectObject(hdc,hPen);

		MoveToEx(hdc,start.x,start.y,NULL);
		LineTo(hdc,end.x,end.y);
		if(this->isSelected()) 
		{
			drawSpecialPoint(hdc,start);
			drawSpecialPoint(hdc,end);
		}

		SelectObject(hdc,hOldPen);
		DeleteObject(hPen);
	}

	virtual bool didSelectMe(POINT &pt) const
	{
		return isPointNearLine(start,end,pt);
	}

	virtual GraphicsType getGraphicsType() const
	{
		return graphics_type;
	}
	virtual void getFreshRect(RECT &fRect) 
	{
		fRect.left = min(start.x,end.x) - getSPR() - 1;
		fRect.top  = min(start.y,end.y) - getSPR() - 1;
		fRect.right = max(start.x,end.x) + getSPR() + 1;
		fRect.bottom  = max(start.y,end.y) + getSPR() + 1;
	}
	virtual BOOL Save(HANDLE hFile)
	{
		unsigned long lWrite;
		BOOL fResult = WriteFile(hFile,&graphics_type,sizeof(GraphicsType),&lWrite,NULL)
			&& WriteFile(hFile,&pen_style,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&line_width,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&color,sizeof(COLORREF),&lWrite,NULL)
			&& WriteFile(hFile,&selected_flag,sizeof(bool),&lWrite,NULL)
			&& WriteFile(hFile,&SpecialPointRadius,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&start,sizeof(POINT),&lWrite,NULL)
			&& WriteFile(hFile,&end,sizeof(POINT),&lWrite,NULL);
		return fResult;
	}

	virtual BOOL Load(HANDLE hFile)
	{
		unsigned long lRead;
		BOOL fResult =
			ReadFile(hFile,&pen_style,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&line_width,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&color,sizeof(COLORREF),&lRead,NULL)
			&& ReadFile(hFile,&selected_flag,sizeof(bool),&lRead,NULL)
			&& ReadFile(hFile,&SpecialPointRadius,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&start,sizeof(POINT),&lRead,NULL)
			&& ReadFile(hFile,&end,sizeof(POINT),&lRead,NULL);
		return fResult;
	}
	void ImportStruct(MyLineStruct *pmls)
	{
		this->color = pmls->color;
		this->line_width = pmls->line_width;
		this->pen_style = pmls->pen_style;
		this->selected_flag = pmls->selected_flag;
		this->SpecialPointRadius = pmls->SpecialPointRadius;
		this->start = pmls->start;
		this->end = pmls->end;
	}

	void ExportStruct(MyLineStruct &mls)
	{
		mls.color = this->color;
		mls.line_width = this->line_width;
		mls.pen_style = this->pen_style;
		mls.selected_flag = this->selected_flag;
		mls.SpecialPointRadius = this->SpecialPointRadius;
		mls.start = this->start;
		mls.end = this->end;
	}

private:
	static const GraphicsType graphics_type = LINE;
	POINT start;
	POINT end;
};

class MyCircle:public GraphicsObject
{
public:
	MyCircle(){};
	MyCircle(POINT C,int R)
	{
		Central = C;
		Radius=R;
	}

	virtual void display(HDC hdc) 
	{
		//OutputDebugString(L"Draw Circle\n");

		HPEN hPen,hOldPen;
		hPen = (HPEN)CreatePen(getPenStyle(),getLineWidth(),getColor());
		hOldPen = (HPEN)::SelectObject(hdc,hPen);

		Arc(hdc,Central.x - Radius,Central.y - Radius,Central.x + Radius,Central.y + Radius,Central.x,Central.y-Radius,Central.x,Central.y-Radius);
		if(this->isSelected()) 
		{
			drawSpecialPoint(hdc,Central);
			drawSpecialPoint(hdc,Central.x - Radius,Central.y);
			drawSpecialPoint(hdc,Central.x + Radius,Central.y);
			drawSpecialPoint(hdc,Central.x,Central.y - Radius);
			drawSpecialPoint(hdc,Central.x,Central.y + Radius);
		}

		SelectObject(hdc,hOldPen);
		DeleteObject(hPen);
	}

	virtual bool didSelectMe(POINT &pt) const
	{
		bool flag = SelectingSensitivity > fabs((double)Radius - sqrt((double)((pt.x - Central.x)*(pt.x - Central.x) + (pt.y - Central.y)*(pt.y - Central.y))));
		return flag;
	}

	virtual GraphicsType getGraphicsType() const
	{
		return graphics_type;
	}
	virtual void getFreshRect(RECT &fRect) 
	{
		fRect.left = Central.x - Radius - getSPR() - 1;
		fRect.top  = Central.y - Radius - getSPR() - 1;
		fRect.right = Central.x + Radius + getSPR() + 1;
		fRect.bottom  = Central.y + Radius + getSPR() + 1;
	}
	
	virtual BOOL Save(HANDLE hFile)
	{
		unsigned long lWrite;
		BOOL fResult = WriteFile(hFile,&graphics_type,sizeof(GraphicsType),&lWrite,NULL)
			&& WriteFile(hFile,&pen_style,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&line_width,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&color,sizeof(COLORREF),&lWrite,NULL)
			&& WriteFile(hFile,&selected_flag,sizeof(bool),&lWrite,NULL)
			&& WriteFile(hFile,&SpecialPointRadius,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&Central,sizeof(POINT),&lWrite,NULL)
			&& WriteFile(hFile,&Radius,sizeof(int),&lWrite,NULL);
		return fResult;
	}

	virtual BOOL Load(HANDLE hFile)
	{
		unsigned long lRead;
		BOOL fResult =
			ReadFile(hFile,&pen_style,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&line_width,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&color,sizeof(COLORREF),&lRead,NULL)
			&& ReadFile(hFile,&selected_flag,sizeof(bool),&lRead,NULL)
			&& ReadFile(hFile,&SpecialPointRadius,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&Central,sizeof(POINT),&lRead,NULL)
			&& ReadFile(hFile,&Radius,sizeof(int),&lRead,NULL);
		return fResult;
	}

	void ImportStruct(MyCircleStruct *pmcs)
	{
		this->color = pmcs->color;
		this->line_width = pmcs->line_width;
		this->pen_style = pmcs->pen_style;
		this->selected_flag = pmcs->selected_flag;
		this->SpecialPointRadius = pmcs->SpecialPointRadius;
		this->Central = pmcs->Central;
		this->Radius = pmcs->Radius;
	}

	void ExportStruct(MyCircleStruct &mcs)
	{
		mcs.color = this->color;
		mcs.line_width = this->line_width;
		mcs.pen_style = this->pen_style;
		mcs.selected_flag = this->selected_flag;
		mcs.SpecialPointRadius = this->SpecialPointRadius;
		mcs.Central = this->Central;
		mcs.Radius = this->Radius;	
	}
private:
	static const GraphicsType graphics_type = CIRCLE;
	POINT Central;
	int Radius;
};

class MyRectangular:public GraphicsObject
{
public:
	MyRectangular(){}
	MyRectangular(int left,int top,int right,int bottom)
	{
		nLeftRect=min(left,right);
		nTopRect=min(top,bottom);
		nRightRect=max(left,right);
		nBottomRect=max(top,bottom);
	}

	virtual void display(HDC hdc) 
	{
		//OutputDebugString(L"Draw Rectangular\n");

		HPEN hPen,hOldPen;
		hPen = (HPEN)CreatePen(getPenStyle(),getLineWidth(),getColor());
		hOldPen = (HPEN)::SelectObject(hdc,hPen);

		MoveToEx(hdc,nLeftRect,nTopRect,NULL);
		LineTo(hdc,nRightRect,nTopRect);
		LineTo(hdc,nRightRect,nBottomRect);
		LineTo(hdc,nLeftRect,nBottomRect);
		LineTo(hdc,nLeftRect,nTopRect);
		if(this->isSelected()) 
		{
			drawSpecialPoint(hdc,nRightRect,nTopRect);
			drawSpecialPoint(hdc,nRightRect,nBottomRect);
			drawSpecialPoint(hdc,nLeftRect,nBottomRect);
			drawSpecialPoint(hdc,nLeftRect,nTopRect);
		}

		SelectObject(hdc,hOldPen);
		DeleteObject(hPen);
	}

	virtual bool didSelectMe(POINT &pt) const
	{
		return isPointNearLine(nLeftRect,nTopRect,nRightRect,nTopRect,pt)
			|| isPointNearLine(nRightRect,nTopRect,nRightRect,nBottomRect,pt)
			|| isPointNearLine(nRightRect,nBottomRect,nLeftRect,nBottomRect,pt)
			|| isPointNearLine(nLeftRect,nBottomRect,nLeftRect,nTopRect,pt);
	}

	virtual GraphicsType getGraphicsType() const
	{
		return graphics_type;
	}

	virtual void getFreshRect(RECT &fRect) 
	{
		fRect.left = nLeftRect - getSPR() - 1;
		fRect.top  = nTopRect - getSPR() - 1;
		fRect.right = nRightRect + getSPR() + 1;
		fRect.bottom  = nBottomRect + getSPR() + 1;
	}
	virtual BOOL Save(HANDLE hFile)
	{
		unsigned long lWrite;
		BOOL fResult = WriteFile(hFile,&graphics_type,sizeof(GraphicsType),&lWrite,NULL)
			&& WriteFile(hFile,&pen_style,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&line_width,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&color,sizeof(COLORREF),&lWrite,NULL)
			&& WriteFile(hFile,&selected_flag,sizeof(bool),&lWrite,NULL)
			&& WriteFile(hFile,&SpecialPointRadius,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&nLeftRect,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&nTopRect,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&nRightRect,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&nBottomRect,sizeof(int),&lWrite,NULL);
		return fResult;
	}

	virtual BOOL Load(HANDLE hFile)
	{
		unsigned long lRead;
		BOOL fResult =
			ReadFile(hFile,&pen_style,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&line_width,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&color,sizeof(COLORREF),&lRead,NULL)
			&& ReadFile(hFile,&selected_flag,sizeof(bool),&lRead,NULL)
			&& ReadFile(hFile,&SpecialPointRadius,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&nLeftRect,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&nTopRect,sizeof(int),&lRead,NULL)			
			&& ReadFile(hFile,&nRightRect,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&nBottomRect,sizeof(int),&lRead,NULL);
		return fResult;
	}

	void ImportStruct(MyRectStruct *pmrs)
	{
		this->color = pmrs->color;
		this->line_width = pmrs->line_width;
		this->pen_style = pmrs->pen_style;
		this->selected_flag = pmrs->selected_flag;
		this->SpecialPointRadius = pmrs->SpecialPointRadius;
		this->nLeftRect = pmrs->nLeftRect;
		this->nTopRect = pmrs->nTopRect;
		this->nRightRect = pmrs->nRightRect;
		this->nBottomRect = pmrs->nBottomRect;
	}

	void ExportStruct(MyRectStruct &mrs)
	{
		mrs.color = this->color;
		mrs.line_width = this->line_width;
		mrs.pen_style = this->pen_style;
		mrs.selected_flag = this->selected_flag;
		mrs.SpecialPointRadius = this->SpecialPointRadius;
		mrs.nLeftRect = this->nLeftRect;
		mrs.nTopRect = this->nTopRect;
		mrs.nRightRect = this->nRightRect;
		mrs.nBottomRect = this->nBottomRect;	
	}
private:
	static const GraphicsType graphics_type = RECTANGULAR;
	int nLeftRect;
	int nTopRect;
	int nRightRect;
	int nBottomRect;
};

class MyPolygon:public GraphicsObject
{
public:
	MyPolygon(){}
	MyPolygon(POINT *pPoints,int cPoints)
	{
		for(int i=0;i<cPoints;i++)
		{
			Point.push_back(*(pPoints+i));
		}
	}
	virtual void display(HDC hdc) 
	{
		//OutputDebugString(L"Draw MyPolygon\n");

		HPEN hPen,hOldPen;
		hPen = (HPEN)CreatePen(getPenStyle(),getLineWidth(),getColor());
		hOldPen = (HPEN)::SelectObject(hdc,hPen);

		if((int)Point.size() > 0)
		{
			MoveToEx(hdc,Point[0].x,Point[0].y,NULL);
			PolylineTo(hdc,&Point[0],(int)Point.size());
			LineTo(hdc,Point[0].x,Point[0].y);
			if(this->isSelected()) 
			{
				for(int i=0;i<(int)Point.size();i++)
				{
					drawSpecialPoint(hdc,Point[i]);
				}
			}
		}
		SelectObject(hdc,hOldPen);
		DeleteObject(hPen);
	}

	virtual bool didSelectMe(POINT &pt) const
	{
		for(int i=0;i<(int)Point.size();i++)
		{
			if(i==0){
				if(this->isPointNearLine(Point[Point.size()-1],Point[i],pt)) 
					return true;
			}
			else{
				if(this->isPointNearLine(Point[i-1],Point[i],pt)) 
					return true;
			}
		}
		return false;
	}
	virtual GraphicsType getGraphicsType() const
	{
		return graphics_type;
	}

	virtual void getFreshRect(RECT &fRect) 
	{
		RECT tempRect;
		if((int)Point.size() > 0)
		{
			tempRect.left = Point[0].x;
			tempRect.top  = Point[0].y;
			tempRect.right = Point[0].x;
			tempRect.bottom  = Point[0].y;

			for(int i = 0; i < (int)Point.size(); i++)
			{
				if(Point[i].x < tempRect.left ) tempRect.left = Point[i].x;
				else if(Point[i].x > tempRect.right ) tempRect.right = Point[i].x;
				if(Point[i].y < tempRect.top ) tempRect.top = Point[i].y;
				else if(Point[i].y > tempRect.bottom ) tempRect.bottom = Point[i].y;
			}

			fRect.left = tempRect.left - getSPR() - 1;
			fRect.top  = tempRect.top - getSPR() - 1;
			fRect.right = tempRect.right + getSPR() + 1;
			fRect.bottom  = tempRect.bottom + getSPR() + 1;

		}
		else
		{
			fRect.left = 0;
			fRect.top  = 0;
			fRect.right = 0;
			fRect.bottom  = 0;
		}
	}
	virtual BOOL Save(HANDLE hFile)
	{
		unsigned long lWrite;
		int PointSize = (int)Point.size();
		BOOL fResult1 = WriteFile(hFile,&graphics_type,sizeof(GraphicsType),&lWrite,NULL)
			&& WriteFile(hFile,&pen_style,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&line_width,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&color,sizeof(COLORREF),&lWrite,NULL)
			&& WriteFile(hFile,&selected_flag,sizeof(bool),&lWrite,NULL)
			&& WriteFile(hFile,&SpecialPointRadius,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&PointSize,sizeof(int),&lWrite,NULL);

		BOOL fResult2 = TRUE;
		for(int i = 0; i< (int)Point.size(); i++)
		{
			if(!WriteFile(hFile,&Point[i],sizeof(POINT),&lWrite,NULL))
			{
				fResult2 = FALSE;
				break;
			}
		}
		return fResult1 && fResult2;
	}

	virtual BOOL Load(HANDLE hFile)
	{
		unsigned long lRead;
		int PointSize = -1;
		BOOL fResult1 =
			ReadFile(hFile,&pen_style,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&line_width,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&color,sizeof(COLORREF),&lRead,NULL)
			&& ReadFile(hFile,&selected_flag,sizeof(bool),&lRead,NULL)
			&& ReadFile(hFile,&SpecialPointRadius,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&PointSize,sizeof(int),&lRead,NULL);

		BOOL fResult2 = TRUE;
		for(int i = 0; i< PointSize; i++)
		{
			POINT tmpPT;
			if(!ReadFile(hFile,&tmpPT,sizeof(POINT),&lRead,NULL))
			{
				fResult2 = FALSE;
				break;
			}
			else
			{
				Point.push_back(tmpPT);
			}
		}
		return fResult1 && fResult2;
	}

	void ImportStruct(MyPolyStruct *pmps)
	{
		this->color = pmps->color;
		this->line_width = pmps->line_width;
		this->pen_style = pmps->pen_style;
		this->selected_flag = pmps->selected_flag;
		this->SpecialPointRadius = pmps->SpecialPointRadius;
		for(int i = 0;i < pmps->points_size; i++)
		{
			this->Point.push_back(pmps->Points[i]);
		}
	}

	void ExportStruct(MyPolyStruct &mps)
	{
		mps.color = this->color;
		mps.line_width = this->line_width;
		mps.pen_style = this->pen_style;
		mps.selected_flag = this->selected_flag;
		mps.SpecialPointRadius = this->SpecialPointRadius;
		mps.points_size = (int)this->Point.size();
		for(int i = 0;i < (int)this->Point.size(); i++)
		{
			mps.Points[i] = this->Point[i];
		}
	}
private:
	static const GraphicsType graphics_type = POLYGON;
	std::vector<POINT> Point;
};

class MyText:public GraphicsObject
{
public:
	MyText(){}
	MyText(const wchar_t *t,int x,int y,int tl,HDC hdc)
	{
		text = new wchar_t[tl];
		wcscpy(text,t);
		text_lenth = tl;
		text_area.bottom = y;
		text_area.top = y;
		text_area.left = x;
		text_area.right = x;
		logfont = DefaultLogFont;
		
		HFONT hFont,hOldFont;
		hFont = CreateFontIndirect(&logfont);
		hOldFont = (HFONT)::SelectObject(hdc,hFont);
		DrawText(hdc,text,text_lenth,&text_area,DT_CALCRECT);
		SelectObject(hdc,hOldFont);
		DeleteObject(hFont);
	}/*
	~MyText()
	{
		delete [] text;
	}*/
	virtual void display(HDC hdc) 
	{
		//OutputDebugString(L"Draw Text\n");

		HFONT hFont,hOldFont;
		hFont = CreateFontIndirect(&logfont);
		hOldFont = (HFONT)::SelectObject(hdc,hFont);
		SetTextColor(hdc,getColor());
		DrawText(hdc,text,text_lenth,&text_area,DT_CALCRECT);
		DrawText(hdc,text,text_lenth,&text_area,DT_CENTER);
		if(this->isSelected()) 
		{
			HPEN hPen,hOldPen;
			hPen = (HPEN)CreatePen(PS_DOT,1,getColor());
			hOldPen = (HPEN)SelectObject(hdc,hPen);

			MoveToEx(hdc,text_area.left,text_area.top,NULL);
			LineTo(hdc,text_area.right,text_area.top);
			LineTo(hdc,text_area.right,text_area.bottom);
			LineTo(hdc,text_area.left,text_area.bottom);
			LineTo(hdc,text_area.left,text_area.top);

			SelectObject(hdc,hOldPen);
			DeleteObject(hPen);
		}

		SelectObject(hdc,hOldFont);
		DeleteObject(hFont);
		
	}

	virtual bool didSelectMe(POINT &pt) const
	{
		return pt.x <= text_area.right && pt.x >= text_area.left && pt.y >= text_area.top && pt.y <= text_area.bottom;
	}

	virtual GraphicsType getGraphicsType() const
	{
		return graphics_type;
	}

	virtual void getFreshRect(RECT &fRect,HWND hWnd) 
	{
		HDC hdc=::GetDC(hWnd);

		HFONT hFont,hOldFont;
		hFont = CreateFontIndirect(&logfont);
		hOldFont = (HFONT)::SelectObject(hdc,hFont);

		RECT oldRect = text_area;
		DrawText(hdc,text,text_lenth,&text_area,DT_CALCRECT);
		fRect.left = min(text_area.left,oldRect.left) - 1;
		fRect.top  = min(text_area.top,oldRect.top) - 1;
		fRect.right = max(text_area.right,oldRect.right) + 1;
		fRect.bottom  = max(text_area.bottom,oldRect.bottom) + 1;
		
		SelectObject(hdc,hOldFont);
		DeleteObject(hFont);
		::ReleaseDC(hWnd,hdc);
	}

	LOGFONT getLogFont() const
	{
		return logfont;
	}

	void setLogFont(LOGFONT &lf)
	{
		logfont = lf;
	}

	static LOGFONT getDefaultLogFont() 
	{
		return DefaultLogFont;
	}

	static void setDefaultLogFont(LOGFONT lf)
	{
		DefaultLogFont = lf;
	}
	virtual BOOL Save(HANDLE hFile)
	{
		unsigned long lWrite;
		BOOL fResult = WriteFile(hFile,&graphics_type,sizeof(GraphicsType),&lWrite,NULL)
			&& WriteFile(hFile,&pen_style,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&line_width,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&color,sizeof(COLORREF),&lWrite,NULL)
			&& WriteFile(hFile,&selected_flag,sizeof(bool),&lWrite,NULL)
			&& WriteFile(hFile,&SpecialPointRadius,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,&text_lenth,sizeof(int),&lWrite,NULL)
			&& WriteFile(hFile,text,text_lenth*sizeof(wchar_t),&lWrite,NULL)
			&& WriteFile(hFile,&text_area,sizeof(RECT),&lWrite,NULL)
			&& WriteFile(hFile,&logfont,sizeof(LOGFONT),&lWrite,NULL);
		return fResult;
	}

	virtual BOOL Load(HANDLE hFile)
	{
		unsigned long lRead;
		BOOL fResult1 =
			ReadFile(hFile,&pen_style,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&line_width,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&color,sizeof(COLORREF),&lRead,NULL)
			&& ReadFile(hFile,&selected_flag,sizeof(bool),&lRead,NULL)
			&& ReadFile(hFile,&SpecialPointRadius,sizeof(int),&lRead,NULL)
			&& ReadFile(hFile,&text_lenth,sizeof(int),&lRead,NULL);
			text = new wchar_t[text_lenth];
		BOOL fResult2 = ReadFile(hFile,text,text_lenth*sizeof(wchar_t),&lRead,NULL)
			&& ReadFile(hFile,&text_area,sizeof(RECT),&lRead,NULL)
			&& ReadFile(hFile,&logfont,sizeof(LOGFONT),&lRead,NULL);
		return fResult1 && fResult2;
	}

	void ImportStruct(MyTextStruct *pmts)
	{
		this->color = pmts->color;
		this->line_width = pmts->line_width;
		this->pen_style = pmts->pen_style;
		this->selected_flag = pmts->selected_flag;
		this->SpecialPointRadius = pmts->SpecialPointRadius;
		this->logfont = pmts->logfont;
		this->text_area = pmts->text_area;
		this->text_lenth = pmts->text_lenth;
		if( this->text == NULL ) delete this->text;
		this->text = new wchar_t[this->text_lenth];
		wcscpy(this->text,pmts->text);
	}

	void ExportStruct(MyTextStruct &mts)
	{
		mts.color = this->color;
		mts.line_width = this->line_width;
		mts.pen_style = this->pen_style;
		mts.selected_flag = this->selected_flag;
		mts.SpecialPointRadius = this->SpecialPointRadius;
		mts.logfont = this->logfont;
		mts.text_area = this->text_area;
		mts.text_lenth = this->text_lenth;
		wcscpy(mts.text,this->text);
	}

private:
	int text_lenth;
	wchar_t *text;
	RECT text_area;
	LOGFONT logfont;
	static LOGFONT DefaultLogFont;
	static const GraphicsType graphics_type = TEXT;
};

class CurrentGraphicsObjects
{
public:
	std::list<GraphicsObject *> LpGO;
	std::list<GraphicsObject *>::iterator LpGO_it;

	BOOL SaveFile(HANDLE hFile)
	{
		BOOL bResult1 = TRUE;
		for(LpGO_it = LpGO.begin(); LpGO_it != LpGO.end(); ++LpGO_it)
		{
			if(!(*LpGO_it)->Save(hFile))
			{
				bResult1 = FALSE;
				break;
			}
		}
		unsigned long lWrite;
		GraphicsType end_graphics_type = OBJECT;
		BOOL bResult2 = WriteFile(hFile,&end_graphics_type,sizeof(GraphicsType),&lWrite,NULL);
		return bResult1 && bResult2;
	}

	BOOL LoadFile(HANDLE hFile)
	{
		for (LpGO_it= LpGO.begin(); LpGO_it!= LpGO.end(); ++LpGO_it)
		{
			delete *LpGO_it;
		}
		LpGO.clear();
		
		BOOL bResult = TRUE;
		unsigned long lWrite = -1;
		while( lWrite != 0 )
		{
			GraphicsType gt;
			BOOL bTempResult1 = ReadFile(hFile,&gt,sizeof(GraphicsType),&lWrite,NULL);
			
			BOOL bTempResult2 = TRUE;
			switch(gt)
			{
			case LINE:
				{
					MyLine *pML = new MyLine();
					if(pML->Load(hFile))
					{
						LpGO.push_back(pML);
					}
					else
					{
						bTempResult2 = FALSE;
					}
				}
				break;

			case CIRCLE:
				{
					MyCircle *pMC = new MyCircle();
					if(pMC->Load(hFile))
					{
						LpGO.push_back(pMC);
					}
					else
					{
						bTempResult2 = FALSE;
					}
				}
				break;

			case RECTANGULAR:
				{
					MyRectangular *pMR = new MyRectangular();
					if(pMR->Load(hFile))
					{
						LpGO.push_back(pMR);
					}
					else
					{
						bTempResult2 = FALSE;
					}
				}
				break;

			case POLYGON:
				{
					MyPolygon *pMP = new MyPolygon();
					if(pMP->Load(hFile))
					{
						LpGO.push_back(pMP);
					}
					else
					{
						bTempResult2 = FALSE;
					}
				}
				break;

			case TEXT:
				{
					MyText *pMT = new MyText();
					if(pMT->Load(hFile))
					{
						LpGO.push_back(pMT);
					}
					else
					{
						bTempResult2 = FALSE;
					}
				}
				break;

			case OBJECT:
				lWrite = 0;
				break;
			}

			if(!(bTempResult1 && bTempResult2))
			{
				bResult = FALSE;
				break;
			}

		}
		return bResult;
	}

	std::list<GraphicsObject *>::iterator getSeletingGO()
	{
		std::list<GraphicsObject *>::iterator _it;
		for(_it = LpGO.begin(); _it != LpGO.end(); ++_it)
		{
			if((*_it)->isSelected()) return _it;
		}
		return LpGO.end();
	}
};

