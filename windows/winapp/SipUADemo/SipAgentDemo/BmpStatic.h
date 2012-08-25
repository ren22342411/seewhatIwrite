#if !defined(AFX_MYHYPERLINK_H__699B2FB4_0C03_4B12_B117_210A97860E0D__INCLUDED_)
#define AFX_MYHYPERLINK_H__699B2FB4_0C03_4B12_B117_210A97860E0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyHyperLink.h : header file
//
#include <afxtempl.h>


//This is the EventID , Which Will  send to the Parent
//by the hyperlink  control
#define SCROLL_TIMER 0x3721
#define MAX_ROLLTEXT_NUMBER 6
#define MAX_ROLLTEXT_LENGTH 64
/////////////////////////////////////////////////////////////////////////////
// CBmpStatic window
struct ScrollText {
	enum enumType{NEWVOICEMAIL = 0, OTHER};
	DWORD dwType;
	CString strText;
};


class CBmpStatic : public CStatic
{
// Construction
public:
	CBmpStatic();
	CString m_strIDName;
	CRect   m_rectPosition;
// Attributes
protected:
	//Hyper Link Static
	BOOL m_bMouseOver;
	BOOL m_bVisited;

	CString m_csLinkText;
	CString m_csUrl;

	COLORREF m_clrHoverColor;
	COLORREF m_clrLinkColor;
	COLORREF m_clrVisitedColor;
	COLORREF m_clrLinkBkColor;
	BOOL m_bLinkMode;
	BOOL m_bGrayMode;
public:
	void SetGrayMode(BOOL BGrayMode){m_bGrayMode = BGrayMode;}
	CString GetLinkText();
	CString GetLinkUrl();
	bool GoToLinkUrl(CString csLink);
	
	void SetHoverColor(COLORREF cHoverColor);
	void SetVisitedColor(COLORREF sVisitedColor);
	void SetLinkUrl(CString csUrl);
	void SetLinkText(CString csLinkText);
	void SetLinkColor(COLORREF sLinkColor);
	void SetLinkMode(BOOL bLindMode) ;
	BOOL GetLinkMode() {return m_bLinkMode;}

protected:
	///Normal Static, not Link Mode
	COLORREF m_clrNormalTextColor;
	COLORREF m_clrNormalBgColor;
public:	
	void SetNormalTextColor(COLORREF clrText){m_clrNormalTextColor = clrText;}
	void SetNormalBgColor(COLORREF clrBk){m_clrNormalBgColor = clrBk;}

protected:
	//used by user name display static and scroll text on header
//	CBitmap m_BmpBkg;

	//used for display roll infomation
protected:
// Old Version
//	CString m_strRollText;
//	int m_iStartIndex;
//	int m_iShowTextIndex;

	CBitmap m_BmpBkg;
	BOOL m_bScrollMode;
	CArray<ScrollText, ScrollText&> m_ScrollTexts;
	int m_iTotalScrollTextCount;
	CBitmap m_bmpTemp;
	CBitmap* m_pOldBmp;
	int m_iInterval;
	CDC m_dcMemory;
	int m_iBeginTopTempBmp;
	int m_iCenterTop;
	BOOL m_bScrollBgColorMode;
	COLORREF m_clrScrollBgColor;
public:
	void SetTextFont(int, LPCTSTR, int fnWeight = 400,DWORD fdwItalic = 0, 
		DWORD fdwUnderline = 0, DWORD fdwStrikeOut =0   );
	void AddScrollText(CString&, BOOL bNewVM);
	void ClearScrollTexts(){m_iTotalScrollTextCount = 0;}
	void ShowScrollText();
	void SetScrollMode(BOOL bRollMode);
	void SetScrollInterval(int iInterval){m_iInterval = iInterval;}
	void SetScrollBgColorMode(BOOL bMode){m_bScrollBgColorMode = bMode;}
	void SetScrollBgColor(COLORREF color){m_clrScrollBgColor = color;}
// used for show bitmap
protected:          
	BOOL m_bPhotoMode;
	BOOL m_bLButtonDown;
	CBitmap m_bmpDefaultPhoto;
	CSize m_sizeDefaultPhoto;
	BOOL m_bDefaultPhoto;
public:
    CBitmap m_bkBitmap;
	CBitmap m_BmpPhoto;
	CSize   m_SizePhoto;
	BOOL ShowPhoto();
	BOOL LoadDefaultPhoto(BOOL bDefaultPhoto);
	void SetShowPhotoMode(BOOL bPhotoMode);
// Operations

protected:
	BOOL m_bBrightGloomyMode;
	BOOL m_bBright;
	COLORREF m_clrBrightText;
	COLORREF m_clrGloomyText;
	COLORREF m_clrGloomyBg;
	COLORREF m_clrBrightBg;
public:
	void SetBrightGloomyMode(BOOL bMode){m_bBrightGloomyMode = bMode;}
	void SetBright(BOOL bBright){m_bBright = bBright;}
	void SetBrightTextColor(COLORREF color){m_clrBrightText = color;}
	void SetGloomyTextColor(COLORREF color){m_clrGloomyText = color;}
	void SetBrightBgColor(COLORREF color){m_clrBrightBg = color;}
	void SetGloomyBgColor(COLORREF color){m_clrGloomyBg = color;}
protected:
	BOOL m_bTransparent;
    CString m_strNormalText;
public:
	void SetTransparent(BOOL bTrans){m_bTransparent = bTrans;}
    void GetNormalText(CString& mString){mString = m_strNormalText;}
protected:
	UINT m_uiDTFormat;
	BOOL m_bDrawMultiLineMode;
public:
	void SetDTFormat(UINT uiFormat){m_uiDTFormat = uiFormat;}
	void SetDrawMultiLineMode(BOOL bMultiline){m_bDrawMultiLineMode = bMultiline;}
public:
	void InvalidateRectParent();
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpStatic)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL DestroyWindow();   

	//}}AFX_VIRTUAL

// Implementation
public:
	// Operation of URL
	virtual ~CBmpStatic();

	// Generated message map functions
protected:	
	//{{AFX_MSG(CBmpStatic)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent) ;
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClicked();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);  
    afx_msg HBRUSH CtlColor(CDC *pDC,UINT nCtlColor);
    LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
protected:
		CFont m_fontText;
        
	//	BOOL m_bItalic;
	//	long m_lWeight;
public:
//	void SetFontItalic(BOOL bItalic){m_bItalic = bItalic;}
//	void SetFontWeight(long lWeight){m_lWeight = lWeight;}
	DECLARE_MESSAGE_MAP()

	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYHYPERLINK_H__699B2FB4_0C03_4B12_B117_210A97860E0D__INCLUDED_)
