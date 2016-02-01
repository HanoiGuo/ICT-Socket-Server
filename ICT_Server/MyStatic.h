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
	bool m_bMode;//true ����  false:������

public:

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	/********************************************************************
	��������SetToolTipText
	������ 
	CString& sText				[in]: ��Ϣ����
	bool bActivate				[in]: true���false������
	����ֵ��
	bool							   	: ����true
	���ã����������ʾ��Ϣ
	*********************************************************************/
	bool SetToolTipText(const CString& sText, bool bActivate = true);

	/********************************************************************
	��������ActivateToolTip
	������ 
	bool bEnable					[in]: true���false������
	����ֵ��
	bool							   	: ����true
	���ã����������ʾ��Ϣ�Ƿ񼤻�
	*********************************************************************/
	bool ActivateToolTip(bool bEnable = true);

	/********************************************************************
	��������GetTransparent
	������ 
	none						
	����ֵ��
	bool							: true͸����false��͸��
	���ã���ȡ�ؼ�͸��״̬
	*********************************************************************/
	bool GetTransparent();

	/********************************************************************
	��������SetTransparent
	������ 
	bool bTransparent				[in]: true͸����false��͸��
	����ֵ��
	bool								: ����true
	���ã����ÿؼ�͸��
	*********************************************************************/
	bool SetTransparent( bool bTransparent = true);

	/********************************************************************
	��������SetFontRotation
	������ 
	int nAngle					[in]: 
	����ֵ��
	bool								: ����true
	���ã�set the rotation of fonts ,works only with TT fonts
	*********************************************************************/
	bool SetFontRotation( int nAngle );

	/********************************************************************
	��������SetFontBold
	������ 
	bool bBold					[in]: true�Ӵ֣�false���Ӵ�
	����ֵ��
	bool								: ����true
	���ã����������ϸ
	*********************************************************************/
	bool SetFontBold(bool bBold = true);

	/********************************************************************
	��������SetFontItalic
	������ 
	bool bItalic					[in]: true��б��false����б
	����ֵ��
	bool								: ����true
	���ã�����������б
	*********************************************************************/
	bool SetFontItalic( bool bItalic = true);

		/********************************************************************
	��������SetFontHeight
	������ 
	int nHeight					[in]: �����С
	����ֵ��
	bool								: ����true
	���ã����������С
	*********************************************************************/
	bool SetFontHeight( int nHeight );

	/********************************************************************
	��������SetFontFaceName
	������ 
	CString sFaceName				[in]: ������
	����ֵ��
	bool								: ����true
	���ã���������
	*********************************************************************/
	bool SetFontFaceName( const CString& sFaceName );

	/********************************************************************
	��������GetTextFont
	������ 
	LOGFONT* plgfnt			[in/out]: ����
	����ֵ��
	bool								: ����true
	���ã���ȡ����
	*********************************************************************/
	bool GetTextFont( LOGFONT* plgfnt);

	/********************************************************************
	��������SetTextFont
	������ 
	LONG nHeight					[in]: �����С
	bool bBold					[in]: true�Ӵ֣�false���Ӵ�
	bool bItalic					[in]: true��б��false����б
	CString sFaceName				[in]: ������
	����ֵ��
	bool								: ����true
	���ã�set the font for the control
	*********************************************************************/
	bool SetTextFont( LONG nHeight, bool bBold, bool bItalic, const CString& sFaceName );

	/********************************************************************
	��������SetTextFont
	������ 
	LOGFONT lgfnt					[in]: ����
	����ֵ��
	bool								: ����true
	���ã���������
	*********************************************************************/
	bool SetTextFont( const LOGFONT& lgfnt );

	/********************************************************************
	��������GetTextClr
	������ 
	LOGFONT lgfnt				[in/out]: ����
	����ֵ��
	bool								: ����true
	���ã���ȡ����
	*********************************************************************/
	bool GetTextClr(COLORREF &clr);

	/********************************************************************
	��������SetTextClr
	������ 
	LOGFONT lgfnt					[in]: ����
	����ֵ��
	bool								: ����true
	���ã���������
	*********************************************************************/
	bool SetTextClr( COLORREF clr );

	/********************************************************************
	��������GetBkClr
	������ 
	LOGFONT lgfnt				[in/out]: ��ɫ
	����ֵ��
	bool								: ����true
	���ã���ȡ��ɫ
	*********************************************************************/
	bool GetBkClr(COLORREF &clr);

	/********************************************************************
	��������SetBkClr
	������ 
	LOGFONT lgfnt					[in]: ��ɫ
	����ֵ��
	bool								: ����true
	���ã����ñ�����ɫ
	*********************************************************************/
	bool SetBkClr( COLORREF clr );

	/********************************************************************
	��������SetCommonMode
	������ 
	int mode						[in]: 0��ͨ��ʽ
	����ֵ��
	none								
	���ã����ÿؼ�Ϊ��ͨ��ʽ
	*********************************************************************/
	void SetCommonMode(int mode = 0);//

	/********************************************************************
	��������SetLineInf
	������ 
	COLORREF colLineColor				[in]: ������ɫ
	int	nLineWidth					[in]: ���ߴ�ϸ
	����ֵ��
	none									
	���ã����ÿؼ�������ɫ��ϸ
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

