// FtpDownLog.h: interface for the CFtpDownLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPDOWNLOG_H__C7593685_24FB_4D24_A7AD_4B37535B25CD__INCLUDED_)
#define AFX_FTPDOWNLOG_H__C7593685_24FB_4D24_A7AD_4B37535B25CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ServerU.h"
#include <afxtempl.h>
#include <afxmt.h>

//--------------------------------------------------------------------------------------
// �����궨��
//--------------------------------------------------------------------------------------
#define APP_LOG_FILE					"LogFile.txt"
#define SERVER_LOG_INI					"LogFile.ini"
#define INI_SECTION_LOG					"LogFile"
#define INI_KEY_PATH					"FilePath"
#define INI_KEY_FILTER					"FilterFile"

enum DOWNFILE_STATE
{
	DOWNFILE_FAILE,
	DOWNFILE_SUCCESS,
	DOWNFILE_NOEXIT,
	DOWNFILE_CANCEL,
};


typedef struct CDownFileInfo_TYP
{
	SYSTEMTIME		m_st;					//ϵͳ��ǰʱ��
	char			m_szFileName[MAX_PATH]; //�����ļ���
	char			m_szIp[16];				// �ͻ���IP
	float			m_fDownSpeed;			//�����ٶ�
	DOWNFILE_STATE	m_enuDownFileState;		// �Ƿ�ɹ�����
} CDownFileInfo;


struct DownFileInfoAndID 
{
	CDownFileInfo m_DownFileInfo;
	DWORD  m_SessionID;	// unique ID of the FTP session
};

class CFtpDownLogDecorator;
class CFtpDownLog;

class CFtpDownLogBase  
{
public:
	CFtpDownLogBase(){};
	virtual ~CFtpDownLogBase(){};

public:
	//--------------------------------------------------------------------------------------
	// �ӿ�
	//--------------------------------------------------------------------------------------
	virtual BOOL InitData(CString& strAppPath)=0;						
	virtual void ClearData()=0;										
	virtual void LogText(LPCSTR lpszText)=0;				

public:
	//--------------------------------------------------------------------------------------
	// FTP�¼��������
	//--------------------------------------------------------------------------------------
	virtual BOOL StartDownFile(RFTPEventStr* pBeforEvent)=0;			
	virtual BOOL EndDownFile(RFTPEventStr* pAfterStruc) =0;			
	virtual BOOL AbortDownd(RFTPEventStr* pCloseStruc) =0;				
	virtual BOOL SaveData(CDownFileInfo &Data) =0;					
	virtual BOOL FilterFile(RFTPEventStr* pStruc) =0;	

};


class CFtpDownLog  : public CFtpDownLogBase
{
public:
	CFtpDownLog();
	~CFtpDownLog();

public:
	//--------------------------------------------------------------------------------------
	// �ӿ�
	//--------------------------------------------------------------------------------------
	BOOL InitData(CString& strAppPath);						//��ʼ�������ļ�����
	void ClearData();										//�������
	void LogText(LPCSTR lpszText);							//д�������־�ļ�

public:
	//--------------------------------------------------------------------------------------
	// FTP�¼��������
	//--------------------------------------------------------------------------------------
	BOOL StartDownFile(RFTPEventStr* pBeforEvent);			// ��ʼ�����¼�����
	BOOL EndDownFile(RFTPEventStr* pAfterStruc);			//�ɹ����ؽ����¼�����
	BOOL AbortDownd(RFTPEventStr* pCloseStruc);				//���������¼�����
	BOOL SaveData(CDownFileInfo &Data);						//�����¼
	BOOL FilterFile(RFTPEventStr* pStruc);					//�����ļ��ж�

private:
	CList<DownFileInfoAndID,DownFileInfoAndID&> m_List;		//Session��������
	CStringArray		m_FileArray;						//�����ļ�����
	CStringArray		m_strFilePathArray;
	CString				m_strPathLog;						//��־���ȫ·��
	CString				m_strPathFileIni;					//�����ļ�ȫ·��
	CCriticalSection	m_Section;							//�߳�ͬ��
};


class CFtpDownLogDecorator : public CFtpDownLogBase 
{
	friend CFtpDownLog;
public:
	CFtpDownLogDecorator(CFtpDownLogBase * pFtpDownBase):m_pFtpDownBase(pFtpDownBase){};
	virtual ~CFtpDownLogDecorator(){};

public:
	//--------------------------------------------------------------------------------------
	// �ӿ�
	//--------------------------------------------------------------------------------------
	virtual BOOL InitData(CString& strAppPath){ return m_pFtpDownBase->InitData(strAppPath); };					
	virtual void ClearData() {  m_pFtpDownBase->ClearData(); };										
	virtual void LogText(LPCSTR lpszText){  m_pFtpDownBase->LogText( lpszText); };							

public:
	//--------------------------------------------------------------------------------------
	// FTP�¼��������
	//--------------------------------------------------------------------------------------
	virtual BOOL StartDownFile(RFTPEventStr* pBeforEvent){ return m_pFtpDownBase->StartDownFile(pBeforEvent); };			
	virtual BOOL EndDownFile(RFTPEventStr* pAfterStruc){ return m_pFtpDownBase->EndDownFile(pAfterStruc); };		
	virtual BOOL AbortDownd(RFTPEventStr* pCloseStruc) { return m_pFtpDownBase->AbortDownd(pCloseStruc); };			
	virtual BOOL SaveData(CDownFileInfo &Data){ return m_pFtpDownBase->SaveData(Data); };					
	virtual BOOL FilterFile(RFTPEventStr* pStruc){ return m_pFtpDownBase->FilterFile( pStruc ); };	

protected:
	CFtpDownLogBase * m_pFtpDownBase;
};


#endif // !defined(AFX_FTPDOWNLOG_H__C7593685_24FB_4D24_A7AD_4B37535B25CD__INCLUDED_)
