// 3D_TOOL.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CD_TOOLApp:
// �йش����ʵ�֣������ 3D_TOOL.cpp
//

class CD_TOOLApp : public CWinApp
{
public:
	CD_TOOLApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CD_TOOLApp theApp;
