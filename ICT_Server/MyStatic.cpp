#include "StdAfx.h"
#include "MyStatic.h"
#include <string>


MyStatic::MyStatic(void)
{
	m_bTransparent = true;
	m_clrBkClr = ::GetSysColor(COLOR_WINDOW);
	m_clrTextClr = ::GetSysColor(COLOR_WINDOWTEXT);
	m_brsBkGnd.CreateSolidBrush(m_clrBkClr);
	m_brsHollow.CreateStockObject(HOLLOW_BRUSH);
	m_bMode = true;
	m_colLineColor = RGB(113,113,113);
	m_nLineWidth = 1;
}


MyStatic::~MyStatic(void)
{
	m_brsBkGnd.DeleteObject();
	m_brsHollow.DeleteObject();
	m_fntText.DeleteObject();
}


BOOL MyStatic::PreTranslateMessage(MSG* pMsg) 
{
	InitToolTip();
	m_tt.RelayEvent(pMsg);

	return CStatic::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(MyStatic, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


HBRUSH MyStatic::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	if( m_bTransparent )
	{
		pDC->SetBkMode( TRANSPARENT );
	}
	else 
	{
		pDC->SetBkMode( OPAQUE );
		pDC->SetBkColor(m_clrBkClr);
	}
	pDC->SetTextColor(m_clrTextClr);

	// TODO: Return a non-NULL brush if the parent's handler should not be called
	if( m_bTransparent )
		return (HBRUSH)m_brsHollow;
	else
		return (HBRUSH)m_brsBkGnd;
}


BOOL MyStatic::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	if( m_bTransparent )
		pDC->SetBkMode( TRANSPARENT );
	else
		pDC->SetBkColor(m_clrBkClr);

	return CStatic::OnEraseBkgnd(pDC);
	//return CStatic::OnEraseBkgnd(pDC);
}


void MyStatic::OnPaint()
{
	CStatic::OnPaint();
	if (m_bMode == true)
	{
		CRect rect;
		GetClientRect(&rect);
		rect.InflateRect(1,1,1,1);
		CPen pen(PS_SOLID, m_nLineWidth, m_colLineColor);
		CBrush* brush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		CClientDC dc(this);
		CPen * pOldPen = dc.SelectObject(&pen);
		CBrush* pOldBrush = dc.SelectObject(brush);
		dc.Rectangle(rect);
		if(pOldBrush != NULL)
			dc.SelectObject(pOldPen);
		if(pOldBrush != NULL)
			dc.SelectObject(pOldBrush);
	}
}

bool MyStatic::SetBkClr(COLORREF clr)
{
	m_clrBkClr = clr;
	m_brsBkGnd.DeleteObject();
	m_brsBkGnd.CreateSolidBrush(m_clrBkClr);
	Invalidate();

	return true;
}

void MyStatic::SetLineInf(COLORREF colLineColor, int	nLineWidth)
{
	m_colLineColor = colLineColor;
	m_nLineWidth = nLineWidth;
	//	m_pen.CreatePen(PS_SOLID, m_nLineWidth, m_colLineColor);
}

void MyStatic::SetCommonMode(int mode)
{
	m_bMode = false;
}

bool MyStatic::GetBkClr(COLORREF & clr)
{
	clr = m_clrBkClr;
	return true;
}

bool MyStatic::SetTextClr(COLORREF clr)
{
	m_clrTextClr = clr;
	Invalidate();

	return true;
}

bool MyStatic::GetTextClr(COLORREF & clr)
{
	clr = m_clrTextClr;
	return true;
}

bool MyStatic::SetTextFont( LONG nHeight, bool bBold, bool bItalic, const CString& sFaceName )
{
	LOGFONT lgfnt;

	lgfnt.lfHeight			= -MulDiv(nHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY), 72);
	lgfnt.lfWidth			= 0;
	lgfnt.lfEscapement		= 0;
	lgfnt.lfOrientation		= 0;
	lgfnt.lfWeight			= bBold?FW_HEAVY:FW_DONTCARE;
	lgfnt.lfItalic			= bItalic?TRUE:FALSE;
	lgfnt.lfUnderline		= FALSE;
	lgfnt.lfStrikeOut		= FALSE;
	lgfnt.lfCharSet			= DEFAULT_CHARSET;
	lgfnt.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	lgfnt.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	lgfnt.lfQuality			= DEFAULT_QUALITY;
	lgfnt.lfPitchAndFamily	= DEFAULT_PITCH | FF_DONTCARE;
	wcscpy(lgfnt.lfFaceName,(TCHAR*)(LPCTSTR)sFaceName);

	return SetTextFont( lgfnt );
}

bool MyStatic::SetTextFont( const LOGFONT& lgfnt )
{
	m_fntText.DeleteObject();
	m_fntText.CreateFontIndirect( &lgfnt );
	SetFont( &m_fntText, TRUE );

	return true;
}

bool MyStatic::GetTextFont( LOGFONT* plgfnt)
{
	GetFont()->GetObject( sizeof(LOGFONT), (void*)plgfnt);
	return true;
}

bool MyStatic::SetTransparent( bool bTransparent /* = true */)
{
	m_bTransparent = bTransparent;
	Invalidate();
	return true;
}

bool MyStatic::GetTransparent()
{
	return m_bTransparent;
}

bool MyStatic::SetFontBold( bool bBold )
{
	LOGFONT lgfnt;

	GetTextFont( &lgfnt );
	lgfnt.lfWeight	= bBold?FW_BOLD:FW_DONTCARE; 

	SetTextFont( lgfnt );

	return true;
}


bool MyStatic::SetFontItalic( bool bItalic )
{
	LOGFONT lgfnt;

	GetTextFont( &lgfnt );
	lgfnt.lfItalic	= bItalic ? TRUE : FALSE; 

	SetTextFont( lgfnt );
	return true;
}

bool MyStatic::SetFontHeight( int nHeight )
{
	LOGFONT lgfnt;

	GetTextFont( &lgfnt );
	lgfnt.lfHeight	= -MulDiv(nHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY), 72);    
	lgfnt.lfWidth	= 0; 

	SetTextFont( lgfnt );
	return true;
}


bool MyStatic::SetFontFaceName( const CString& sFaceName )
{
	LOGFONT lgfnt;

	GetTextFont( &lgfnt );
	wcscpy( lgfnt.lfFaceName, (TCHAR*)(LPCTSTR)sFaceName );


	SetTextFont( lgfnt );
	return true;
}

bool MyStatic::SetFontRotation( int nAngle )
{
	LOGFONT lgfnt;

	GetTextFont( &lgfnt );
	lgfnt.lfEscapement	= long(nAngle)*10;
	lgfnt.lfOrientation	= long(nAngle)*10;
	SetTextFont( lgfnt );

	return true;
}

bool MyStatic::InitToolTip()
{
	if (m_tt.m_hWnd == NULL) 
	{
		m_tt.Create(this);
		m_tt.Activate(true);
		m_tt.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
	}

	return true;
}

bool MyStatic::SetToolTipText(const CString& sText, bool bActivate)
{
	InitToolTip(); 

	// If there is no tooltip defined then add it
	if (m_tt.GetToolCount() == 0)
	{
		CRect rect; 
		GetClientRect(rect);
		m_tt.AddTool(this, sText, rect, 1);
	}

	m_tt.UpdateTipText(sText, this, 1);
	m_tt.Activate(bActivate);

	return true;
}

bool MyStatic::ActivateToolTip(bool bActivate)
{
	if (m_tt.GetToolCount() == 0)
		return false;

	// Activate tooltip
	m_tt.Activate(bActivate);

	return true;
}


 