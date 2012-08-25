//**************************************************************
//	File: BmpStatic.cpp
//	
// 	Description: This file is the Implementation of a bitmap Static
//
//	its functions contain:
//	 1. a normal static , you can custom it background color and text color
//   2. a hyperlink static , you can set link address, link text , link color,
//       visited color and so on
//   3. a roll text static , you can add some  display texts, than it can display
//      every text in turn.
//	History:
//	    2005-05-05 Alex Zhu 	Created
//      2005-06-05 Alex Zhu     Modify    Add HyperLink functions
//      2005-06-22 Alex Zhu     Modify    Add Roll Text functions
//      2005-07-15 Alex Zhu     Modify    Modify Roll Text functions
//      2005-12-01 ALex Zhu		Modify    Add Displaying multiline text
//	Copyright C  WayTech Inc. 2005. All rights reserved.
//***************************************************************

#include "stdafx.h"
#include "BmpStatic.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CBmpStatic
CBmpStatic::CBmpStatic()
{
	m_clrLinkColor = RGB(0,128,255);//RGB(36,36,36);
	m_clrHoverColor =  RGB(0,0,255);
	m_clrVisitedColor = RGB(36,36,36);
	m_clrLinkBkColor = RGB(36,36,36);

	m_clrNormalTextColor = RGB(36,36,36);
	m_clrNormalBgColor =RGB(0,0,0);// RGB(255,255,255);

	m_bMouseOver = FALSE;
	m_bVisited =  FALSE;	
	m_bLinkMode = FALSE;

	m_bScrollMode = FALSE;

	
	//roll text
	m_iTotalScrollTextCount = 0;
//	m_iShowTextIndex = -1;
	m_iInterval = 5000;
	m_iCenterTop = (24 - 16) / 2;
	m_iBeginTopTempBmp = 0 - m_iCenterTop;

	m_bPhotoMode = FALSE;

	m_bBrightGloomyMode = FALSE;
	
	int nFontSize = 16;
	m_fontText.CreateFont(nFontSize,0,0,0,400,
		FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE,"Arial");

	m_bTransparent = FALSE;
	m_uiDTFormat = DT_CENTER | DT_SINGLELINE | DT_VCENTER;

	m_bScrollBgColorMode = FALSE;

	//m_bItalic = FALSE;
//	m_lWeight = 400;


	m_bDefaultPhoto = TRUE;
	m_bLButtonDown = FALSE;
	
	m_bDrawMultiLineMode = FALSE;
}

CBmpStatic::~CBmpStatic()
{

}

BEGIN_MESSAGE_MAP(CBmpStatic, CStatic)
	//{{AFX_MSG_MAP(CBmpStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_PAINT()   
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
    ON_WM_ERASEBKGND()    
    ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBmpStatic message handlers

//Sets the Link Color
void CBmpStatic::SetLinkColor(COLORREF sLinkColor)
{
	m_clrLinkColor = sLinkColor;
}

//open the URL by Windows ShellExecute()
bool CBmpStatic::GoToLinkUrl(CString csLink)
{
	if(m_bLinkMode == FALSE)
	{
		return false;
	}
	HINSTANCE hInstance = (HINSTANCE)ShellExecute(NULL, _T("open"), csLink.operator LPCTSTR(), NULL, NULL, SW_SHOW);
	return true;
}

//The Mouse Move Message
void CBmpStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	CStatic::OnMouseMove(nFlags, point);
	if (m_bMouseOver)
	{
		CRect oRect;
		GetClientRect(&oRect);
        if(!m_bLinkMode)
            return;
		//check if the mouse is in the rect
		if (oRect.PtInRect(point) == false)
		{
			m_bMouseOver = FALSE;
			//Release the Mouse capture previously take
			ReleaseCapture();
			RedrawWindow();
			return;
		}
	}
	else
	{
		m_bMouseOver = TRUE;  
        if (!m_bLinkMode) 
            return;
        RedrawWindow();         
		//capture the mouse
		SetCapture();
	} 
}

void CBmpStatic::PreSubclassWindow() 
{
	CStatic::PreSubclassWindow();
}

void CBmpStatic::SetLinkText(CString csLinkText)
{
	m_csLinkText = csLinkText;
	this->SetWindowText(csLinkText.operator LPCTSTR());
}

BOOL CBmpStatic::PreTranslateMessage(MSG* pMsg) 
{
	return CStatic::PreTranslateMessage(pMsg);
}

//////////////////EVENT WILL GET HERE //////////////////////
void CBmpStatic::OnClicked() 
{
	if(m_bLinkMode == TRUE)
	{
		GoToLinkUrl(m_csUrl);
		m_bVisited = TRUE;
		m_bMouseOver = FALSE;
		this->Invalidate(true);
	}
	else if(m_bPhotoMode == TRUE)
	{

	}

}

HBRUSH CBmpStatic::CtlColor(CDC *pDC,UINT nCtlColor)
{       
    if (nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkMode(TRANSPARENT);        
    }
    return (HBRUSH)::GetStockObject(NULL_BRUSH);
}

CString CBmpStatic::GetLinkText()  {
	if (m_csLinkText.IsEmpty())
	{
		return CString("");
	}
	return m_csLinkText;
}

void CBmpStatic::SetLinkUrl(CString csUrl) {
	m_csUrl= csUrl;
}

CString CBmpStatic::GetLinkUrl() {
	return m_csUrl;
}

void CBmpStatic::SetVisitedColor(COLORREF sVisitedColor) {
	m_clrVisitedColor = sVisitedColor ;
}
void CBmpStatic::SetHoverColor(COLORREF cHoverColor) {
	m_clrHoverColor = cHoverColor;
}

BOOL CBmpStatic::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR* pMsgHdr;
	pMsgHdr = (NMHDR*) lParam;

	switch (pMsgHdr->code){
	case NM_RCLICK:
		break;
	default:
	;
	}
	
	return CStatic::OnNotify(wParam, lParam, pResult);
}
void CBmpStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bLinkMode == TRUE)
	{
		GoToLinkUrl(GetLinkUrl());
		m_bVisited = TRUE;
		m_bMouseOver = FALSE;
		this->Invalidate(true);
	}
	else if(m_bPhotoMode == TRUE)
	{
		m_bLButtonDown = TRUE;
	}
	CStatic::OnLButtonDown(nFlags,point);
}

void CBmpStatic::OnTimer(UINT nIDEvent)
{
	CRect rect;
	GetClientRect(rect);
	static BOOL iFirstThisMessage = TRUE;
	if(nIDEvent == SCROLL_TIMER)
	{
		if( (m_iBeginTopTempBmp + m_iCenterTop) % rect.Height() != 0 )
		{
			if(iFirstThisMessage == TRUE)
			{
				KillTimer(SCROLL_TIMER);
				m_iInterval = 40;
				SetTimer(SCROLL_TIMER, m_iInterval, NULL);
				iFirstThisMessage = FALSE;
			}
			m_iBeginTopTempBmp += 1;
			if(m_iBeginTopTempBmp >= rect.Height() * m_iTotalScrollTextCount - 4)
			{
				m_iBeginTopTempBmp = 0 - m_iCenterTop;

			}
			Invalidate();
		}
		else
		{
			m_iInterval = 5000;
			m_iBeginTopTempBmp++;
			iFirstThisMessage = TRUE;
			SetTimer(SCROLL_TIMER, m_iInterval, NULL);
		}
	}
	CStatic::OnTimer(nIDEvent);
}

void CBmpStatic::SetLinkMode(BOOL bLindMode)
{
	m_bLinkMode = bLindMode;
	if(m_bLinkMode == TRUE)
	{
		m_bScrollMode = FALSE;
	}
	if(bLindMode == TRUE)
	{
		ModifyStyle(0,SS_NOTIFY);
	}
	else
	{
		ModifyStyle(SS_NOTIFY,0);
	}
}


void CBmpStatic::ShowScrollText()
{

}


BOOL CBmpStatic::OnEraseBkgnd(CDC* pDC)
{
    //TRACE("On EraseText Text\n");
    RECT r;
    GetClientRect(&r);    
    //DrawBitmap(pDC, m_bkBitmap, r, FALSE);	
    return TRUE;
}


void CBmpStatic::SetTextFont(int nFontSize, LPCTSTR lpszFaceName, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut)
{
	m_fontText.DeleteObject();
#pragma warning(disable : 4244)
	m_fontText.CreateFont(nFontSize,0,0,0,fnWeight,
		fdwItalic,fdwUnderline,fdwStrikeOut,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE,lpszFaceName);
#pragma warning(default: 4244)
}
void CBmpStatic::SetScrollMode(BOOL bRollMode)
{
	m_bScrollMode = bRollMode;
	ModifyStyle(0, SS_NOTIFY);
	ModifyStyle(0, SS_CENTER);
	if(m_bScrollMode == TRUE)
	{
		m_bLinkMode = FALSE;
	}
}

void CBmpStatic::AddScrollText(CString& strText, BOOL bNewVM)
{
	// if roll text count is equal to 5, we remove the first one whose type is not new mail
	if(m_bScrollMode == FALSE)
	{
		return;
	}
	int iSetAt = -1;
	ScrollText rt;
	if(bNewVM == TRUE)
	{
		for(int i = 0; i < m_iTotalScrollTextCount; i++)
		{
			if(m_ScrollTexts.ElementAt(i).dwType == ScrollText::NEWVOICEMAIL)
			{
				iSetAt = i;
				break;
			}
		}
		//如果已经有voicemail的信息了，就覆盖这个
		if(iSetAt != -1)
		{
			
		}
		//否则新加一个
		else
		{
			//如果已经满了,就找到第一个不是voicemail的信息覆盖
			if(m_iTotalScrollTextCount >= MAX_ROLLTEXT_NUMBER)
			{
				for(int i = 0; i < MAX_ROLLTEXT_NUMBER; i++)
				{
					if(m_ScrollTexts.ElementAt(i).dwType != ScrollText::NEWVOICEMAIL)
					{
						iSetAt = i;
						break;
					}
				}
			}
			//否则就增加一个
			else
			{
				iSetAt = m_iTotalScrollTextCount;
				m_iTotalScrollTextCount++;
			}
			//出现特殊情况就设为第0个
			if(iSetAt == -1)
			{
				iSetAt = 0;
			}
		}

	}

   else
   {
	   //如果已经满了,就找到第一个不是voicemail的信息覆盖
	   if(m_iTotalScrollTextCount >= MAX_ROLLTEXT_NUMBER)
	   {
		   for(int i = 0; i < MAX_ROLLTEXT_NUMBER; i++)
		   {
			   if(m_ScrollTexts.ElementAt(i).dwType != ScrollText::NEWVOICEMAIL)
			   {
				   iSetAt = i;
				   break;
			   }
		   }
	   }
	   //否则就增加一个
	   else
	   {
		   iSetAt = m_iTotalScrollTextCount;
		   m_iTotalScrollTextCount++;
	   }
	   //出现特殊情况就设为第0个
	   if(iSetAt == -1)
	   {
		   iSetAt = 0;
	   }
   }
   
	if(bNewVM == TRUE)
	{
		rt.dwType = ScrollText::NEWVOICEMAIL;
	}
	else
	{
		rt.dwType = ScrollText::OTHER;
	}
	
	if(strText.GetLength() > MAX_ROLLTEXT_LENGTH)
	{
		TCHAR tchText[MAX_ROLLTEXT_LENGTH];
		LPTSTR lpStr = strText.GetBuffer(MAX_ROLLTEXT_LENGTH - 1);
		CopyMemory(tchText, lpStr, MAX_ROLLTEXT_LENGTH - 1);
		tchText[MAX_ROLLTEXT_LENGTH - 1] = '\0';
		ASSERT(_tcslen(tchText) != MAX_ROLLTEXT_LENGTH);
		strText.ReleaseBuffer();
	}
	else
	{
		rt.strText = strText;
	}

	m_ScrollTexts.SetAtGrow(iSetAt, rt);

	if(m_ScrollTexts.GetSize() > 0)
	{
		//Get the dimension of the label
		CRect rect;
		GetClientRect(rect);

		//draw canvas
		CDC* pDC = GetDC();
	
		CBrush brushBK;	
		if(m_bScrollBgColorMode == FALSE)
		{
			m_BmpBkg.DeleteObject();
//			m_BmpBkg.LoadBitmap(IDB_BITMAP_BRUSH);
			brushBK.CreatePatternBrush(&m_BmpBkg);
		}
		else
		{
			brushBK.CreateSolidBrush(m_clrScrollBgColor);
		}
		m_dcMemory.DeleteDC();
		m_dcMemory.CreateCompatibleDC(pDC);
		m_bmpTemp.DeleteObject();

		m_bmpTemp.CreateCompatibleBitmap(pDC, rect.Width() - 5, rect.Height() * m_iTotalScrollTextCount);
		
		m_pOldBmp = m_dcMemory.SelectObject(&m_bmpTemp);
		
		BITMAP bmp;
		m_bmpTemp.GetBitmap(&bmp);
		CRect rectTemp(0,0, bmp.bmWidth, bmp.bmHeight);
		m_dcMemory.FillRect(rectTemp,&brushBK);

		//draw text on the canvas
		CFont* pOldFont=(CFont*)m_dcMemory.SelectObject(&m_fontText);
		CSize size = pDC->GetTextExtent(CString("ABCDabcd1234中国人"));
	//	m_iCenterTop = (rect.Height() - size.cy) / 2.0;
		int ixTop = 5;
		//int iyTop = (rect.Height() - size.cy) / 2.0;
		int iyTop = 0;
		if(iyTop < 0)
		{
//			AfxMessageBox("Dispaly Information Erro!");
		}

		m_dcMemory.SetBkMode(TRANSPARENT);
		//RGB(2,72,136)
		m_dcMemory.SetTextColor(m_clrNormalTextColor);

		for(int iLine = 0; iLine < m_iTotalScrollTextCount; iLine++)
		{
			CRect rectCanvas;
			rectCanvas.left = ixTop;
			rectCanvas.top = iyTop + iLine * rect.Height();
			rectCanvas.right = rect.right;
			rectCanvas.bottom = iyTop + (iLine + 1) * rect.Height();
			m_dcMemory.DrawText(m_ScrollTexts.GetAt(iLine).strText, rectCanvas, DT_LEFT);
		}
		
		m_dcMemory.SelectObject(pOldFont);
		
		ReleaseDC(pDC);
		KillTimer(SCROLL_TIMER);
		SetTimer(SCROLL_TIMER, 105, NULL);
	}
}

void CBmpStatic::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(rect);

	if(m_bLinkMode == TRUE)
	{
             //   TRACE("On Paint Text7\n");
		if (m_bMouseOver )
		{
			if (m_bVisited)
			{
				dc.SetTextColor(m_clrVisitedColor);
			}
			else
			{
				dc.SetTextColor(m_clrHoverColor);
			}
		}
		else 
		{
			if (m_bVisited)
			{
				dc.SetTextColor(m_clrVisitedColor);
			}
			else
			{
				dc.SetTextColor(m_clrLinkColor);
			}
		}
		

		//Repaint the Window Text
		dc.SetBkMode(TRANSPARENT);
		CFont *pOldFont = NULL;
		if(m_bGrayMode == TRUE)
		{
    
			if(m_bMouseOver)
			{
				CBrush brush;
				brush.CreateSolidBrush(RGB(128,128,128));
				dc.FillRect(rect, &brush);
				brush.DeleteObject();
			}
			else
			{
				CBrush brush;
				brush.CreateSolidBrush(RGB(225,225,225));
				dc.FillRect(rect, &brush);
				brush.DeleteObject();
			}
		}
		else
		{
			pOldFont = dc.SelectObject(&m_fontText);
		}

		CString strText;
		GetWindowText(strText);
		dc.DrawText(strText, rect, m_uiDTFormat);
		if(pOldFont != NULL && m_bGrayMode != TRUE )
		{
			dc.SelectObject(pOldFont);
		}

	}
	else if(m_bScrollMode == TRUE )
	{
        //TRACE("On Paint Text6\n");
		//If successful, a handle to the Windows GDI 
		//object represented by the CBitmap object; otherwise NULL
		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcMemory, 0, m_iBeginTopTempBmp, SRCCOPY);
	}
	else if(m_bPhotoMode == TRUE)
	{
        //        TRACE("On Paint Text5\n");
		ShowPhoto();
	}
	else if(m_bBrightGloomyMode == TRUE)
	{
        //TRACE("On Paint Text4\n");
	     CBrush brush; 
		 dc.SetBkMode(TRANSPARENT);

		 if(m_bBright == TRUE)
		  {
			 dc.SetTextColor(m_clrBrightText);
			 brush.CreateSolidBrush(m_clrBrightBg);
		  }
		  else
		  {			
			 dc.SetTextColor(m_clrGloomyText);
			 brush.CreateSolidBrush(m_clrGloomyBg);
		  }	
		  if(m_bTransparent == FALSE)
		  {
			 dc.FillRect(rect,&brush);
		  }

	      CString strText;
		  GetWindowText(strText);
          CFont *oldFont = dc.SelectObject(&m_fontText);
		  dc.DrawText(strText, rect, m_uiDTFormat);
          dc.SelectObject(oldFont);
		  brush.DeleteObject();
	}
	else if(m_bDrawMultiLineMode == TRUE)	
	{
         //TRACE("On Paint Text3\n");
	}
    else if(m_bGrayMode == TRUE)
    {
        //TRACE("test\n");
    }
    else 
	{  		
        //TRACE("On Paint Text2\n");
		CFont* pOldFont = NULL;
		pOldFont = dc.SelectObject(&m_fontText);  		        
        dc.SetBkMode(TRANSPARENT);
        if(m_bTransparent == FALSE)
		{	
			CBrush brush;
			brush.CreateSolidBrush(m_clrNormalBgColor);
			dc.FillRect(rect,&brush);
			brush.DeleteObject();
		}
        else
            dc.SetBkMode(TRANSPARENT);
		//CString strText;
		//GetWindowText(strText);  
        dc.SetTextColor(m_clrNormalTextColor);
		dc.DrawText(m_strNormalText, rect, m_uiDTFormat);
		dc.SelectObject(pOldFont);
   }    
}


BOOL CBmpStatic::LoadDefaultPhoto(BOOL bDefaultPhoto)
{
	if(m_bPhotoMode == FALSE)
	{
		return FALSE;
	}
	m_bDefaultPhoto = bDefaultPhoto;
	Invalidate();
	return TRUE;
}

BOOL CBmpStatic::ShowPhoto()
{

	if(m_bPhotoMode == FALSE)
	{
		return FALSE;
	}
	CRect rect;
	GetClientRect(rect);
	//ASSERT(rect.Width() == rect.Height());
    
	int nWidth = 0, nHeight = 0;
    int nxStart = 0, nyStart = 0;
    CBitmap* pBitmap = NULL;
	CSize* pSize = NULL;
	if(m_bDefaultPhoto == TRUE)
	{
		pBitmap = &m_bmpDefaultPhoto;
		pSize = &m_sizeDefaultPhoto;
	}
	else
	{
		pBitmap = &m_BmpPhoto;
		pSize = &m_SizePhoto;
	}
	if(pBitmap->GetSafeHandle() == NULL)
	{
		return FALSE;
	}
	CDC* dc = GetDC();
	if(pSize->cx >= rect.Width() || pSize->cy >= rect.Height())
	{
		int iMax = pSize->cx > pSize->cy ? pSize->cx : pSize->cy;

		nWidth =(int)(pSize->cx * rect.Width() / 1.0 / iMax) ;
		nHeight =(int)(pSize->cy * rect.Height()/ 1.0 / iMax) ;
		
		int iMax2 = nWidth > nHeight ? nWidth : nHeight;

		nxStart = (int) ((rect.Width() - nWidth) / 2.0);
		nyStart = (int) ((rect.Height() - nHeight) / 2.0);
	}
	else
	{
		nWidth = pSize->cx;
		nHeight = pSize->cy;
		
		nxStart = (int) (( rect.Width() - nWidth ) / 2.0);
		nyStart = (int) (( rect.Height() - nHeight ) / 2.0);
	}
	
	CDC memdc;
	memdc.CreateCompatibleDC(dc);
	CBitmap* pOldBmp = memdc.SelectObject(pBitmap);
    
	//The SetStretchBltMode function sets the bitmap stretching
	//mode in the specified device context. 
	dc->SetStretchBltMode(HALFTONE);
	dc->StretchBlt(nxStart, nyStart, nWidth, nHeight, &memdc, 0, 0, pSize->cx, pSize->cy, SRCCOPY);

	memdc.SelectObject(pOldBmp);
	DeleteDC(memdc);

	ReleaseDC(dc);
	return TRUE;
}

BOOL CBmpStatic::DestroyWindow()
{
	if(HFONT(m_fontText))
	{
		m_fontText.DeleteObject();
	}
	if(m_BmpPhoto.GetSafeHandle() != NULL)
	{
		m_BmpPhoto.DeleteObject();
	}
	if(m_bmpDefaultPhoto.GetSafeHandle() != NULL)
	{
		m_bmpDefaultPhoto.DeleteObject();
	}
	m_dcMemory.SelectObject(m_pOldBmp);

	if(HBITMAP(m_bmpTemp))
	{
		m_bmpTemp.DeleteObject();
	}
	m_dcMemory.DeleteDC();
	m_ScrollTexts.RemoveAll();

	return CStatic::DestroyWindow();
}
//BOOL CBmpStatic::OnEraseBkgnd(CDC* pDC)
//{
//	return TRUE;
// }

void CBmpStatic::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bPhotoMode == TRUE)
	{
//		if(m_strIDName.CompareNoCase("STATIC_PHOTO") == 0)
//		{
//			CSkinDialog* pDlg = (CSkinDialog*)GetParent();
//			if(pDlg->m_strDlgName.CompareNoCase("DLG_GENERAL") == 0)
//			{
//				CGeneralDlg* pGenDlg = (CGeneralDlg*)pDlg;
//				pGenDlg->LoadPhoto();
//			}
//			else
//			{
//				Invalidate();
//			}
//		}

	}
	CStatic::OnLButtonDblClk(nFlags, point);
}




void CBmpStatic::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bPhotoMode == TRUE)
	{
		if(m_bLButtonDown == TRUE)
		{
		}
		m_bLButtonDown = FALSE;
	}
	CStatic::OnLButtonUp(nFlags, point);
}

LRESULT CBmpStatic::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_SETTEXT)
    {
        //CStatic::DefWindowProc(message,wParam,lParam);
        if (!m_bPhotoMode && !m_bScrollBgColorMode && !m_bScrollMode && !m_bBrightGloomyMode 
            && !m_bLinkMode ) 
        {            
            m_strNormalText = (char *)lParam;
            Invalidate();
            return true;
        }
        
    }
    return CStatic::DefWindowProc(message,wParam,lParam);
}

void CBmpStatic::InvalidateRectParent()
{
	CWnd* pParent = this->GetParent();
	if(pParent != NULL)
	{
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);
		pParent->ScreenToClient(rect);
		pParent->InvalidateRect(rect);
	}
}