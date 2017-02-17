// ImageProcess.h : main header file for the IMAGEPROCESS application
//

#if !defined(AFX_IMAGEPROCESS_H__7C70D8E4_EC43_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_IMAGEPROCESS_H__7C70D8E4_EC43_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CImageProcessApp:
// See ImageProcess.cpp for the implementation of this class
//

class CImageProcessApp : public CWinApp
{
public:
	CImageProcessApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProcessApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CImageProcessApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESS_H__7C70D8E4_EC43_11D2_9481_000021003EA5__INCLUDED_)
