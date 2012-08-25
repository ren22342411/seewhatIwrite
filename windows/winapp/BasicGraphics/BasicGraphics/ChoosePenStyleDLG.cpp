#include "stdafx.h"
#include "ChoosePenStyleDLG.h"

void DrawLine(HDC hdc,int x1,int y1,int x2,int y2,int ps,COLORREF color) 
{
	SetBkColor(hdc,color);

	HPEN hPen,hOldPen;
	hPen = (HPEN)CreatePen(ps,1,0);
	hOldPen = (HPEN)::SelectObject(hdc,hPen);

	MoveToEx(hdc,x1,y1,NULL);
	LineTo(hdc,x2,y2);

	SelectObject(hdc,hOldPen);
	DeleteObject(hPen);
}

INT_PTR CALLBACK ChoosePSProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//UNREFERENCED_PARAMETER(lParam);
	PAINTSTRUCT ps;
	HDC hdc;

	static int tempIDC_RADIO = 0;

	switch (message)
	{
	case WM_INITDIALOG:
		{
			if( PS_SOLID == iRadioPS )
				tempIDC_RADIO = IDC_RADIO_SOLID;
			else if( PS_DASH == iRadioPS )
				tempIDC_RADIO = IDC_RADIO_DASH;
			else if( PS_DOT == iRadioPS )
				tempIDC_RADIO = IDC_RADIO_DOT;
			else if( PS_DASHDOT == iRadioPS )
				tempIDC_RADIO = IDC_RADIO_DASHDOT;
			else if( PS_DASHDOTDOT == iRadioPS )
				tempIDC_RADIO = IDC_RADIO_DASHDOTDOT;
			CheckRadioButton(hDlg, IDC_RADIO_SOLID, IDC_RADIO_DASHDOTDOT, tempIDC_RADIO) ;
			SetFocus(GetDlgItem (hDlg, tempIDC_RADIO)) ;
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON2://Cancel
			bChoosePenStyle = false;
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;
		case IDC_BUTTON1://OK
			if(tempIDC_RADIO==IDC_RADIO_SOLID) iRadioPS = PS_SOLID;
			else if(tempIDC_RADIO==IDC_RADIO_DASH) iRadioPS = PS_DASH;
			else if(tempIDC_RADIO==IDC_RADIO_DOT) iRadioPS = PS_DOT;
			else if(tempIDC_RADIO==IDC_RADIO_DASHDOT) iRadioPS = PS_DASHDOT;
			else if(tempIDC_RADIO==IDC_RADIO_DASHDOTDOT) iRadioPS = PS_DASHDOTDOT;
			bChoosePenStyle = true;
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;

		case   IDC_RADIO_SOLID:
		case   IDC_RADIO_DASH:
		case   IDC_RADIO_DOT:
		case   IDC_RADIO_DASHDOT:
		case   IDC_RADIO_DASHDOTDOT:
			tempIDC_RADIO =  LOWORD (wParam);
			CheckRadioButton (hDlg, IDC_RADIO_SOLID, IDC_RADIO_DASHDOTDOT, tempIDC_RADIO) ;
			return (INT_PTR)TRUE;
			break;

		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hDlg, &ps);
		COLORREF clr = GetPixel(hdc,1,1);
		DrawLine(hdc,90,74,210,74,PS_SOLID,clr);
		DrawLine(hdc,90,106,210,106,PS_DASH,clr);
		DrawLine(hdc,90,138,210,138,PS_DOT,clr);
		DrawLine(hdc,90,170,210,170,PS_DASHDOT,clr);
		DrawLine(hdc,90,202,210,202,PS_DASHDOTDOT,clr);

		EndPaint(hDlg, &ps);
		break;
	}
	return (INT_PTR)FALSE;
}

bool ChoosePenStyleDLG(int &ps,HINSTANCE hInst,HWND hWnd)
{
	iRadioPS = ps;
	DialogBox(hInst, MAKEINTRESOURCE(IDD_PROPPAGE_SMALL2), hWnd, ChoosePSProc);
	ps = iRadioPS;
	return bChoosePenStyle;
}