/**
 * Copyright 2007 by smallfool,
 * Room 303, Building 41, No 51, Boshan Road, Pudong District, Shanghai, China
 * All rights reserved.
 *
 * This software is the confidential and proprietary information of smallfool. 
 * Use is subject to license terms.
 *
 * Thanks to Dandan Huang.
 */
// InfoZip.h: interface for the CInfoZip class.
//
//////////////////////////////////////////////////////////////////////

/************************************************************************/
/*
	http://blog.csdn.net/qianjintianguo/archive/2006/04/21/671895.aspx

   һ�� ZIP �ļ�������������ɣ�
         

        ѹ��Դ�ļ�������+ѹ��Դ�ļ�Ŀ¼��+ѹ��Դ�ļ�Ŀ¼������־

    1��ѹ��Դ�ļ�������

    �������������ÿһ��ѹ����Դ�ļ�/Ŀ¼����һ����¼����¼�ĸ�ʽ���£�
       

       [�ļ�ͷ+ �ļ����� + ����������]
 
       a���ļ�ͷ�ṹ

         ���    ��                ����
      �ļ�ͷ���                  4 bytes  (0x04034b50)
      ��ѹ�ļ����� pkware �汾    2 bytes
      ȫ�ַ�ʽλ���              2 bytes
  ����ѹ����ʽ                    2 bytes
  ��������޸��ļ�ʱ��             2 bytes
 ���� ����޸��ļ�����             2 bytes
 ���� CRC-32У��                  4 bytes
 �� ��ѹ����ߴ�                  4 bytes
 �� ��δѹ���ߴ�                  4 bytes
 ���� �ļ�������                  2 bytes

      ��չ��¼����                2 bytes
 ���� �ļ���                     ���������ȣ�
 ���� ��չ�ֶ�                   ���������ȣ�

		typedef struct tagZippedFileHeader
		{
			DWORD	dwMarker;
			WORD	wVersion;
			WORD	wGlobalFlags;
			WORD	wCompressedMode;
			WORD	wTimeLastModified;
			WORD	wDateLastModified;
			DWORD	dwCRC32;
			DWORD	dwSizeCompressed;
			DWORD	dwSizeUncompressed;
			WORD	wFileNameLength;
			CHAR	FileName[0];
		}ZIPPEDFILEHEADER, *LPZIPPEDFILEHEADER;


        b���ļ�����

 

        

        c������������

 ���������    ������
  ����CRC-32У��                  4 bytes
  ����ѹ����ߴ�                   4 bytes
 �� ��δѹ���ߴ�                   4 bytes

      �������������ֻ��ȫ�ַ�ʽλ��ǵĵڣ�λ��Ϊ��ʱ�Ŵ��ڣ�������⣩��������ѹ�����ݵ����һ���ֽں��������������ֻ���ڲ��ܶ������ ZIP �ļ����м���ʱʹ�á����磺��һ�����ܼ��������������磺�Ŵ����ϣ��ϵ� ZIP �ļ��С�����Ǵ����ϵ�ZIP�ļ�һ��û�����������������

 

 

     2��ѹ��Դ�ļ�Ŀ¼��

     �������������ÿһ����¼��Ӧ��ѹ��Դ�ļ��������е�һ������

 

   ���������               ��            ����

    ����Ŀ¼���ļ��ļ�ͷ���             4 bytes  (0x02014b50)

    ����ѹ��ʹ�õġ�pkware �汾          2 bytes

    ������ѹ�ļ����� pkware �汾         2 bytes

    ����ȫ�ַ�ʽλ���                   2 bytes

    ����ѹ����ʽ                        2 bytes

    ��������޸��ļ�ʱ��                 2 bytes

    ��������޸��ļ�����                 2 bytes

    �����ãңã�����У��                 4 bytes

    ����ѹ����ߴ�                      4 bytes

    ����δѹ���ߴ�                      4 bytes

    �����ļ�������                      2 bytes

    ������չ�ֶγ���                    2 bytes

    �����ļ�ע�ͳ���                    2 bytes

    �������̿�ʼ��                      2 bytes

    �����ڲ��ļ�����                    2 bytes

    �����ⲿ�ļ�����                    4 bytes

        �ֲ�ͷ��ƫ����                  4 bytes

    �����ļ���                       ���������ȣ�

    ������չ�ֶ�                     ���������ȣ�

        �ļ�ע��                     ���������ȣ�

 

 

    3��ѹ��Դ�ļ�Ŀ¼������־

 

    ���������               ��          ����

        Ŀ¼�������                    4 bytes  (0x02014b50)

        ��ǰ���̱��                    2 bytes

        Ŀ¼����ʼ���̱��              2 bytes

    �����������ϼ�¼����                 2 bytes

    ����Ŀ¼���м�¼����                 2 bytes

    ����Ŀ¼���ߴ��С                   4 bytes

    ����Ŀ¼���Ե�һ�Ŵ��̵�ƫ����        4 bytes

    ����ZIP �ļ�ע�ͳ���                 2 bytes

    ����ZIP �ļ�ע��                   ���������ȣ�

 


                                                                     */
/************************************************************************/

#if !defined(AFX_INFOZIP_H__A5C325B4_180B_11D3_ABD5_00A0C9B4D8AB__INCLUDED_)
#define AFX_INFOZIP_H__A5C325B4_180B_11D3_ABD5_00A0C9B4D8AB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ZipDll.h"
#include "UnzipDll.h"
#include "ZCallBck.h"
BEGIN_ZIP


#define ZERROR_NONE                    0
#define ZERROR_DLL_NOT_FOUND           1
#define ZERROR_DLL_FOUNCTION_NOT_FOUND 2
#define ZERROR_NOT_INITIALIZED         3

class InfoZip  
{
public:
	BOOL ExtractFiles(const char* pszArchive, const char* pszTargetFolder);
	void SetDefaultValues(UnzipParams *pParams);
	void ReleaseParams(UnzipParams *pParams);
	BOOL Execute(UnzipParams *pParams);
	BOOL FinalizeUnzip();
	BOOL FinalizeZip();
	BOOL InitializeUnzip();
	BOOL InitializeZip();
	BOOL GetInitializedUnzip();
	BOOL GetInitializedZip();
	void ReleaseParams(ZipParams *pParams);
	BOOL AddFiles(const char *pszArchive, char **paFiles, int iFileCount, BOOL bIncludeDirectoryName = TRUE);
	BOOL Execute(ZipParams *pParams);
	void SetDefaultValues(ZipParams *pZipParms);
	BOOL Finalize();
	BOOL GetInitialized();
	BOOL Initialize();
	UINT GetLastError();
	int GetZipDllVersion();
	int GetUnzipDllVersion();
	InfoZip();
	virtual ~InfoZip();

private:
	void SetLastError(UINT uiError);
	UINT				m_uiLastError;
	HINSTANCE			m_ZipDllHandle;
	HINSTANCE			m_UnzipDllHandle;
	CZipDllExec			m_ZipDllExec;
	CUnzipDllExec		m_UnzipDllExec;
	CGetZipDllVersion   m_GetZipDllVersion;
	CGetUnzipDllVersion m_GetUnzipDllVersion;
private:
	static BOOL __stdcall DefaultZipCallback(ZipCallbackData *pData);
};


END_ZIP

#endif // !defined(AFX_INFOZIP_H__A5C325B4_180B_11D3_ABD5_00A0C9B4D8AB__INCLUDED_)
