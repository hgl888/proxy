// FtpDownLog.cpp: implementation of the CFtpDownLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerULog.h"
#include "FtpDownLog.h"
#include <shlwapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFtpDownLog::CFtpDownLog()
{

}

CFtpDownLog::~CFtpDownLog()
{

}

//--------------------------------------------------------------------------------------
// ��ʼ�������ļ����� ��־����·��
//--------------------------------------------------------------------------------------
BOOL CFtpDownLog::InitData(CString &strAppPath)
{
	//
	//��װ�����ļ�ȫ·������
	//
	m_strPathFileIni= strAppPath+SERVER_LOG_INI;

	//��װ��־���·��,Ĭ��ֵΪ��ģ��·��
	GetPrivateProfileString(INI_SECTION_LOG,
		INI_KEY_PATH,
		(LPCSTR)strAppPath,
		m_strPathLog.GetBuffer(MAX_PATH),
		MAX_PATH,(LPCSTR)m_strPathFileIni);
	m_strPathLog.ReleaseBuffer();

	if(m_strPathLog.Right(1)!='\\')
		m_strPathLog+='\\';

	//���·���Ƿ����
	if( ! PathFileExists(m_strPathLog))
	{
		DWORD dwCode = GetLastError();
		if(dwCode!=ERROR_ALREADY_EXISTS)
		{
			CString str;
			str.Format("Path does not exist: %X %s",dwCode,m_strPathLog);
			OutputDebugString((LPCSTR)str);

			LogText((LPCSTR)str);
			return FALSE;
		}
	}

	//ȡ�����ļ�
	int iKey=1;
	CString strKey;
	CString strTmp;
	//char szIndexKey[16];
	m_FileArray.RemoveAll();
	while(TRUE)
	{
		strKey.Format("%d", iKey );
		GetPrivateProfileString(INI_KEY_FILTER,
			strKey,
			"",
			strTmp.GetBuffer(MAX_PATH),	MAX_PATH,
			(LPCSTR)m_strPathFileIni);
		strTmp.ReleaseBuffer();
		if(strTmp.IsEmpty())
			break;
		m_FileArray.Add(strTmp);
		iKey++;
	} //end while

	//�����ļ�·��
	iKey = 1;
	m_strFilePathArray.RemoveAll();
	while( true )
	{
		strKey.Format("%d", iKey );
		GetPrivateProfileString("FilterPath",
			strKey,
			"",
			strTmp.GetBuffer(MAX_PATH),	MAX_PATH,
			(LPCSTR)m_strPathFileIni);
		strTmp.ReleaseBuffer();
		if(strTmp.IsEmpty())
			break;
		m_strFilePathArray.Add(strTmp);
		iKey++;
	}

	LogText("CFtpDownLog::InitData OK!");
	return TRUE;
}

//--------------------------------------------------------------------------------------
// ��ʼ�����¼�����
//--------------------------------------------------------------------------------------
BOOL CFtpDownLog::StartDownFile(RFTPEventStr* pBeforEvent)
{
	if (EVNT_StartDown != pBeforEvent->Event)
		return FALSE;

	//�����������Ҹ�SessionID
	BOOL bIsExist = FALSE;
	m_Section.Lock();
	try
	{
		POSITION pos = m_List.GetHeadPosition();
		while(pos!=NULL)
		{
			DownFileInfoAndID &Item = m_List.GetNext(pos);
			if(Item.m_SessionID==pBeforEvent->SessionID)
			{
				bIsExist = TRUE;
				break;
			}
		}
		//SessionID�Ѿ�����
		if(bIsExist)
		{
			m_Section.Unlock();
			return FALSE;
		}

		//�����SessionID��Ӧ����Ϣ
		DownFileInfoAndID downFileInfo;
		ZeroMemory(&downFileInfo,  sizeof(downFileInfo));
		GetLocalTime( &downFileInfo.m_DownFileInfo.m_st);
		lstrcpy(downFileInfo.m_DownFileInfo.m_szFileName, pBeforEvent->AuxOne);
		lstrcpy(downFileInfo.m_DownFileInfo.m_szIp, pBeforEvent->ClientIP);
		downFileInfo.m_DownFileInfo.m_fDownSpeed = 0;
		downFileInfo.m_DownFileInfo.m_enuDownFileState = DOWNFILE_FAILE;
		downFileInfo.m_SessionID = pBeforEvent->SessionID;
		
		//��ӵ�����
		m_List.AddTail(downFileInfo);
		m_Section.Unlock();

	}
	catch(...)
	{
		m_Section.Unlock();
		LogText("StartDownFile");
		return FALSE;
	}

	
	return TRUE;
}

//--------------------------------------------------------------------------------------
// �ɹ����ؽ����¼�����
//--------------------------------------------------------------------------------------
BOOL CFtpDownLog::EndDownFile(RFTPEventStr* pAfterStruc)
{
	if ( EVNT_EndDown != pAfterStruc->Event )
		return FALSE;

	BOOL bResult=FALSE;
	m_Section.Lock();

	try
	{
		POSITION pos = m_List.GetHeadPosition();
		while(pos!=NULL)
		{
			DownFileInfoAndID &Item = m_List.GetAt(pos);
			if(Item.m_SessionID!=pAfterStruc->SessionID)
			{
				m_List.GetNext(pos);
				continue;
			}
			CDownFileInfo downFileInfo;
			ZeroMemory(&downFileInfo, sizeof(CDownFileInfo));
			downFileInfo.m_st = Item.m_DownFileInfo.m_st;
			strcpy(downFileInfo.m_szFileName, Item.m_DownFileInfo.m_szFileName);
			strcpy(downFileInfo.m_szIp, Item.m_DownFileInfo.m_szIp);
			downFileInfo.m_fDownSpeed = (float)pAfterStruc->Size /(float) pAfterStruc->Duration;
			downFileInfo.m_enuDownFileState = DOWNFILE_SUCCESS;

			//��������
			if ( SaveData(downFileInfo))	
			{
				//ɾ���ýڵ�
				m_List.RemoveAt(pos);
				bResult = TRUE;
			}
			else
			{
				LogText("SaveData Failed");
			}
			break;
		} // end while
	}
	catch(...)
	{
		m_Section.Unlock();
		LogText("EndDownFile");
		return FALSE;
	}

	m_Section.Unlock();
	return bResult;
}

//--------------------------------------------------------------------------------------
// �ر������¼�����
//--------------------------------------------------------------------------------------
BOOL CFtpDownLog::AbortDownd(RFTPEventStr* pCloseStruc)
{
	if ( EVNT_AbortDown != pCloseStruc->Event )
		return FALSE;

	BOOL bResult=FALSE;

	m_Section.Lock();
	try
	{
		POSITION pos = m_List.GetHeadPosition();
		while(pos!=NULL)
		{
			DownFileInfoAndID &Item = m_List.GetAt(pos);
			if(Item.m_SessionID!=pCloseStruc->SessionID)
			{
				m_List.GetNext(pos);
				continue;
			}
			
			CDownFileInfo downFileInfo;
			ZeroMemory(&downFileInfo, sizeof(CDownFileInfo));
			downFileInfo.m_st = Item.m_DownFileInfo.m_st;
			strcpy(downFileInfo.m_szFileName, Item.m_DownFileInfo.m_szFileName);
			strcpy(downFileInfo.m_szIp, Item.m_DownFileInfo.m_szIp);
			downFileInfo.m_fDownSpeed = (float)pCloseStruc->Size /(float) pCloseStruc->Duration;
			downFileInfo.m_enuDownFileState = DOWNFILE_FAILE;

			//��������
			if ( SaveData(downFileInfo))	
			{
				m_List.RemoveAt(pos);
				bResult = TRUE;
			}
			break;
		} // end while
	}
	catch(...)
	{
		m_Section.Unlock();
		LogText("AboutDownd");
		return FALSE;
	}
	return bResult;
}

//--------------------------------------------------------------------------------------
// �����¼
//--------------------------------------------------------------------------------------
BOOL CFtpDownLog::SaveData(CDownFileInfo &Data)
{
	CString strLogFile;
	CTime time = CTime::GetCurrentTime();

	strLogFile.Format("%sserverU%s.dat",m_strPathLog,time.Format("%Y-%m-%d"));

	try
	{
		CFile file;
		if(!file.Open(strLogFile,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::shareDenyWrite))
			return FALSE;
		
		file.SeekToEnd();
		
		file.Write((void*)&Data,sizeof(CDownFileInfo));
		file.Close();
		return TRUE;

	}
	catch(...)
	{
		OutputDebugString(strLogFile+" Error!");
		LogText(strLogFile+" Error!");
		return FALSE;
	}
}

/*
*�����ļ��ж�
*/
BOOL CFtpDownLog::FilterFile(RFTPEventStr* pStruc)
{
	m_Section.Lock();
	bool bFilter = false;
	try
	{
		//�����ļ��ж�
		for(int i=0; i< m_FileArray.GetSize(); i++)
		{
			const CString &filter = m_FileArray.GetAt(i);
			if(filter.CompareNoCase(pStruc->AuxOne)==0)
			{
				bFilter = true;
				goto RET;
			}
		}

		//�����ļ�·��,

		for( int j = 0; j < m_strFilePathArray.GetSize(); j ++ )
		{
			const CString & strFilePath = m_strFilePathArray.GetAt( j );
			CString strTem = pStruc->AuxOne;
			CString strFilePathlow = strFilePath;
			strFilePathlow.MakeLower();
			int ilength = strFilePathlow.GetLength();
			strTem.MakeLower();
			if ( ilength > strTem.GetLength() )
			{
				bFilter = true;
				goto RET;
			}
			if (strTem.GetAt(ilength) =='\\')
			{
				strTem = strTem.Left(ilength);
				if (strTem == strFilePathlow )
				{
					bFilter =  false;
					goto RET;
				}
				else
				{
					bFilter = true;
				}
			}
			else
			{
				bFilter = true;
			}
		}
	}
	catch(...)
	{
		OutputDebugString(" CFtpDownLog::FilterFile ");
		LogText(" CFtpDownLog::FilterFile ");
	}

RET:
	m_Section.Unlock();
	return bFilter;
}

//--------------------------------------------------------------------------------------
// �������
//--------------------------------------------------------------------------------------
void CFtpDownLog::ClearData()
{
	m_strFilePathArray.RemoveAll();
	m_FileArray.RemoveAll();
	m_List.RemoveAll();
}

//--------------------------------------------------------------------------------------
// ���������Ϣ����־
//--------------------------------------------------------------------------------------
void CFtpDownLog::LogText(LPCSTR lpszText)
{
	DWORD dwErrorCode = GetLastError();
	CServerULogApp* pApp = (CServerULogApp*)AfxGetApp();
	CString strLogFile;
	strLogFile = pApp->m_strPath+APP_LOG_FILE;
	try{

		CFile file;
		if(!file.Open(strLogFile,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::shareDenyNone))
			return;

		file.SeekToEnd();

		CTime tm = CTime::GetCurrentTime();
		DWORD dwThreadID = ::GetCurrentThreadId();
		
		CString strData;
		strData.Format("%s [%X] [%X] %s\r\n",
			tm.Format("%Y-%m-%d %H:%M:%S"),
			dwThreadID,
			dwErrorCode,
			lpszText);
		file.Write((LPCSTR)strData,strData.GetLength());
		
		file.Close();
	}
	catch(...)
	{
		OutputDebugString("Error: LogText");
	}
}