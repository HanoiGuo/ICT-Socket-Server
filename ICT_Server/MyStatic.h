#pragma once
#ifndef MYSTATIC_H
#define MYSTATIC_H

#include "afxwin.h"
class MyStatic :public CStatic
{
public:
	MyStatic(void);
	~MyStatic(void);
private:
	bool m_bMode;//true 带框  false:不带框

public:

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	/********************************************************************
	函数名：SetToolTipText
	参数： 
	CString& sText				[in]: 信息内容
	bool bActivate				[in]: true激活，false不激活
	返回值：
	bool							   	: 总是true
	作用：设置鼠标提示信息
	*********************************************************************/
	bool SetToolTipText(const CString& sText, bool bActivate = true);

	/********************************************************************
	函数名：ActivateToolTip
	参数： 
	bool bEnable					[in]: true激活，false不激活
	返回值：
	bool							   	: 总是true
	作用：设置鼠标提示信息是否激活
	*********************************************************************/
	bool ActivateToolTip(bool bEnable = true);

	/********************************************************************
	函数名：GetTransparent
	参数： 
	none						
	返回值：
	bool							: true透明，false不透明
	作用：获取控件透明状态
	*********************************************************************/
	bool GetTransparent();

	/********************************************************************
	函数名：SetTransparent
	参数： 
	bool bTransparent				[in]: true透明，false不透明
	返回值：
	bool								: 总是true
	作用：设置控件透明
	*********************************************************************/
	bool SetTransparent( bool bTransparent = true);

	/********************************************************************
	函数名：SetFontRotation
	参数： 
	int nAngle					[in]: 
	返回值：
	bool								: 总是true
	作用：set the rotation of fonts ,works only with TT fonts
	*********************************************************************/
	bool SetFontRotation( int nAngle );

	/********************************************************************
	函数名：SetFontBold
	参数： 
	bool bBold					[in]: true加粗，false不加粗
	返回值：
	bool								: 总是true
	作用：设置字体粗细
	*********************************************************************/
	bool SetFontBold(bool bBold = true);

	/********************************************************************
	函数名：SetFontItalic
	参数： 
	bool bItalic					[in]: true倾斜，false不倾斜
	返回值：
	bool								: 总是true
	作用：设置字体倾斜
	*********************************************************************/
	bool SetFontItalic( bool bItalic = true);

		/********************************************************************
	函数名：SetFontHeight
	参数： 
	int nHeight					[in]: 字体大小
	返回值：
	bool								: 总是true
	作用：设置字体大小
	*********************************************************************/
	bool SetFontHeight( int nHeight );

	/********************************************************************
	函数名：SetFontFaceName
	参数： 
	CString sFaceName				[in]: 字体名
	返回值：
	bool								: 总是true
	作用：设置字体
	*********************************************************************/
	bool SetFontFaceName( const CString& sFaceName );

	/********************************************************************
	函数名：GetTextFont
	参数： 
	LOGFONT* plgfnt			[in/out]: 字体
	返回值：
	bool								: 总是true
	作用：获取字体
	*********************************************************************/
	bool GetTextFont( LOGFONT* plgfnt);

	/********************************************************************
	函数名：SetTextFont
	参数： 
	LONG nHeight					[in]: 字体大小
	bool bBold					[in]: true加粗，false不加粗
	bool bItalic					[in]: true倾斜，false不倾斜
	CString sFaceName				[in]: 字体名
	返回值：
	bool								: 总是true
	作用：set the font for the control
	*********************************************************************/
	bool SetTextFont( LONG nHeight, bool bBold, bool bItalic, const CString& sFaceName );

	/********************************************************************
	函数名：SetTextFont
	参数： 
	LOGFONT lgfnt					[in]: 字体
	返回值：
	bool								: 总是true
	作用：设置字体
	*********************************************************************/
	bool SetTextFont( const LOGFONT& lgfnt );

	/********************************************************************
	函数名：GetTextClr
	参数： 
	LOGFONT lgfnt				[in/out]: 字体
	返回值：
	bool								: 总是true
	作用：获取字体
	*********************************************************************/
	bool GetTextClr(COLORREF &clr);

	/********************************************************************
	函数名：SetTextClr
	参数： 
	LOGFONT lgfnt					[in]: 字体
	返回值：
	bool								: 总是true
	作用：设置字体
	*********************************************************************/
	bool SetTextClr( COLORREF clr );

	/********************************************************************
	函数名：GetBkClr
	参数： 
	LOGFONT lgfnt				[in/out]: 颜色
	返回值：
	bool								: 总是true
	作用：获取颜色
	*********************************************************************/
	bool GetBkClr(COLORREF &clr);

	/********************************************************************
	函数名：SetBkClr
	参数： 
	LOGFONT lgfnt					[in]: 颜色
	返回值：
	bool								: 总是true
	作用：设置背景颜色
	*********************************************************************/
	bool SetBkClr( COLORREF clr );

	/********************************************************************
	函数名：SetCommonMode
	参数： 
	int mode						[in]: 0普通样式
	返回值：
	none								
	作用：设置控件为普通样式
	*********************************************************************/
	void SetCommonMode(int mode = 0);//

	/********************************************************************
	函数名：SetLineInf
	参数： 
	COLORREF colLineColor				[in]: 边线颜色
	int	nLineWidth					[in]: 边线粗细
	返回值：
	none									
	作用：设置控件边线颜色粗细
	*********************************************************************/
	void SetLineInf(COLORREF colLineColor, int	nLineWidth);

	protected:
		bool InitToolTip();
		CToolTipCtrl m_tt;
		CFont m_fntText;
		COLORREF m_clrTextClr;
		COLORREF m_clrBkClr;
		CBrush m_brsBkGnd;
		CBrush m_brsHollow;
		bool m_bTransparent;

private:
	COLORREF m_colLineColor;
	int	m_nLineWidth;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};
#endif

