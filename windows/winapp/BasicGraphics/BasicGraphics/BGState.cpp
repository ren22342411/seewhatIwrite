#include "stdafx.h"
#include "BGState.h"

BGState::BGState()
{
	this->ESInitial();
}

EditState BGState::getES()
{
	return this->edit_state;
}

void BGState::setES(EditState es)
{
	this->edit_state = es;
	this->EditStateReset();
}

void BGState::ESReset()
{
	EditStateReset();
}

void BGState::EditStateReset()
{
	this->LeftButtonDown = 0;
	this->RightButtonDown = 0;
	this->vPoint_LBD.clear();
	this->zoom_value = ZOOM_100;
}

void BGState::ESInitial()
{
	this->edit_state = DRAW_LINE;
	this->EditStateReset();
}

int BGState::TimesOfLBD()
{
	return this->LeftButtonDown;
}

int BGState::TimesOfRBD()
{
	return this->RightButtonDown;
}

void BGState::PPTimesOfLBD()
{
	++LeftButtonDown;
}

void BGState::PPTimesOfRBD()
{
	++RightButtonDown;
}

bool BGState::isLBD0()
{
	return 0==LeftButtonDown;
}

bool BGState::isLBD1()
{
	return 1 == LeftButtonDown;
}

bool BGState::isLBDMoreThan3()
{
	return LeftButtonDown >= 3;
}

void BGState::setLBD0()
{
	this->LeftButtonDown = 0;
}

void BGState::AddLBDPoint(int x, int y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;
	this->vPoint_LBD.push_back(pt);
}

void BGState::AddLBDPoint(POINT pt)
{
	this->vPoint_LBD.push_back(pt);
}

bool BGState::getLastPoint(POINT &pt)
{
	if(this->vPoint_LBD.size() > 0)
	{
		pt = this->vPoint_LBD[vPoint_LBD.size() - 1];
		return true;
	}
	else return false;
}

bool BGState::getPointByIndex(POINT &pt,int index)
{
	if( index >= 0 && index < (int)this -> vPoint_LBD.size() )
	{
		pt = this->vPoint_LBD[index];
		return true;
	}
	else return false;
}

bool BGState::LBD(int x,int y)
{
	POINT temp;
	if(this->getLastPoint(temp))
	{
		if( temp.x == x && temp.y == y )
		{
			return false;
		}
		else
		{
			PPTimesOfLBD();
			AddLBDPoint( x,  y);
			return true;
		}
	}
	else
	{
		PPTimesOfLBD();
		AddLBDPoint( x,  y);
		return true;
	}
}

bool BGState::LBD(POINT pt)
{
	POINT temp;
	if(this->getLastPoint(temp))
	{
		if( temp.x == pt.x && temp.y == pt.y )
		{
			return false;
		}
		else
		{
			PPTimesOfLBD();
			AddLBDPoint( pt );
			return true;
		}
	}
	else
	{
		PPTimesOfLBD();
		AddLBDPoint( pt );
		return true;
	}
}

void BGState::CopyPointV(std::vector<POINT> &vP)
{
	if( vPoint_LBD.size() > 0 )
	{
		for(int i=0;i<(int)vPoint_LBD.size();i++)
		{
			vP.push_back(vPoint_LBD[i]);
		}
	}
}

ZoomValue BGState::getZoomValue()
{
	return this->zoom_value;
}


bool BGState::ZoomIn()
{
	if( this->zoom_value == ZOOM_400 )
	{
		return false;
	}
	else
	{
		switch(this->zoom_value)
		{
		case ZOOM_25:
			this->zoom_value = ZOOM_50;
			break;
					
		case ZOOM_50:
			this->zoom_value = ZOOM_75;
			break;
					
		case ZOOM_75:
			this->zoom_value = ZOOM_100;
			break;
					
		case ZOOM_100:
			this->zoom_value = ZOOM_150;
			break;
					
		case ZOOM_150:
			this->zoom_value = ZOOM_200;
			break;
					
		case ZOOM_200:
			this->zoom_value = ZOOM_400;
			break;
		}
		return true;
	}
}

bool BGState::ZoomOut()
{
	if( this->zoom_value == ZOOM_25 )
	{
		return false;
	}
	else
	{
		switch(this->zoom_value)
		{
		case ZOOM_50:
			this->zoom_value = ZOOM_25;
			break;
					
		case ZOOM_75:
			this->zoom_value = ZOOM_50;
			break;
					
		case ZOOM_100:
			this->zoom_value = ZOOM_75;
			break;
					
		case ZOOM_150:
			this->zoom_value = ZOOM_100;
			break;
					
		case ZOOM_200:
			this->zoom_value = ZOOM_150;
			break;
					
		case ZOOM_400:
			this->zoom_value = ZOOM_200;
			break;
		}
		return true;
	}
}