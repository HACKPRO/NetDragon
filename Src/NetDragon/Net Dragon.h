// Net Dragon.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNetDragonApp:
// �йش����ʵ�֣������ Net Dragon.cpp
//

class CNetDragonApp : public CWinApp
{
public:
	CNetDragonApp();
    static BOOL ApplyFileData();
	static BOOL SetAutoStart(BOOL bStart);
// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
public:
	virtual int ExitInstance();
};

extern CNetDragonApp theApp;