// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__7436F932_305D_4922_9AAD_1AB4B43C4634__INCLUDED_)
#define AFX_STDAFX_H__7436F932_305D_4922_9AAD_1AB4B43C4634__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "config.h"

#include <afxinet.h>

//#define NEED_OUTPUT_MSG

#define IDS_AD_SERVER			_T("EMMOA_IIS")
#define IGA_FTP_AGENT			_T("EMMOA_FTP_AGENT")
#define IDS_INI_FILENAME		_T("EMMOA.ini")

#define DEFAULT_FTP_PORT		10004
#define DEFAULT_FTP_IP			_T("ftp.ige.emmoa.com")
#define DEFAULT_FTP_ACCOUNT		_T("XYZ_IGA_123*&X")
#define DEFAULT_FTP_PASSWORD	_T("XYZ_IGA_123*&X")

# if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
#	define	PROTECT(s)		{	static	TCHAR *__FUNC_NAME__	= TEXT(#s);
#	define	UNPROTECT		}
# else
#	define	PROTECT(s)		{			\
	try			\
							{			\
static	TCHAR *__FUNC_NAME__= TEXT(#s);	

#	define	UNPROTECT		}			\
	catch(...)	\
							{			\
							}			\
						}
# endif

/************************************************************************/
/* ������Ϣ�ṹ                                                         */
/************************************************************************/
typedef struct tagDownloadStatus
{
	DWORD				dwTaskID;		//	��ǰ���������,��ϵͳ�Զ�����

	TCHAR				*pszIP;			//	Զ��������ַ
	UINT				nPort;			//	Զ�����������˿�
	TCHAR				*pszRemoteFile;	//	������Ŀ���ļ�����,��ԴӸ�·����ʼ������·������
	TCHAR				*pszLocalFile;	//	���ش�ŵ�Ŀ��·������

	DWORD				dwStatus;		//	����״̬��Ϣ
	
	DWORD				dwRemoteFileSize;	//	Զ���ļ���С
	DWORD				dwLocalFileSize;	//	�����ļ���С
	
	DWORD				dwBytesDownloaded;	//	�Ѿ����ص����ݳ���
	DWORD				dwBytesToDownload;	//	�����ص����ݳ���
	
	FLOAT				ftTimeElapsed;		//	�����ؿ�ʼ�ļ�ʱʱ��,��
	FLOAT				ftTimeToDownload;	//	���ݵ�ǰ�����ٶȼ�������Ļ���Ҫ������ʱ��,��
	
	
	FLOAT				ftMinSpeed;			//	������С�ٶ�	KB/s
	FLOAT				ftMaxSpeed;			//	��������ٶ�	KB/s
	
	FLOAT				ftAvrgSpeed;		//	����ƽ���ٶ�	KB/s
	FLOAT				ftCurSpeed;			//	��ǰ�����ٶ�	KB/s
	
	WPARAM				wParam;				//	
	LPARAM				lParam;				//	�Զ������
	
}DOWNLOADSTATUS, *LPDOWNLOADSTATUS;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7436F932_305D_4922_9AAD_1AB4B43C4634__INCLUDED_)
