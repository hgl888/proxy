// IGASSee.h : IGASSee Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CIGASSeeApp:
// �йش����ʵ�֣������ IGASSee.cpp
//

class CIGASSeeApp : public CWinApp
{
public:
	CIGASSeeApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIGASSeeApp theApp;
