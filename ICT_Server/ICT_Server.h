
// ICT_Server.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CICT_ServerApp:
// �йش����ʵ�֣������ ICT_Server.cpp
//

class CICT_ServerApp : public CWinApp
{
public:
	CICT_ServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CICT_ServerApp theApp;