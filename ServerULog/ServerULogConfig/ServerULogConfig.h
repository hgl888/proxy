// ServerULogConfig.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CServerULogConfigApp:
// �йش����ʵ�֣������ ServerULogConfig.cpp
//

class CServerULogConfigApp : public CWinApp
{
public:
	CServerULogConfigApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CServerULogConfigApp theApp;