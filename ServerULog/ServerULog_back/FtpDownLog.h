#pragma once

#include "ServerULog.h"
#include <vector>
#include "atlstr.h"

struct RFTPEventStr;

enum DOWNFILE_STATE
{
	DOWNFILE_FAILE,
	DOWNFILE_SUCCESS,
	DOWNFILE_NOEXIT,
	DOWNFILE_CANCEL,
};

void LogToFile(CString szLog);

typedef struct CDownFileInfo_TYP
{
	SYSTEMTIME		m_st;					// ϵͳ��ǰʱ��
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

class CFtpDownLog //: public CDownFileInfo
{
public:
	CFtpDownLog(void);
	~CFtpDownLog(void);
public:
	bool StartDownFile(RFTPEventStr* pBeforEvent);
	bool EndDownFile(RFTPEventStr* pAfterStruc);
	bool CloseDownd(RFTPEventStr* pCloseStruc);
	bool WriteToFile();
	bool FilterFile(RFTPEventStr* pStruc);

	void FtpEnterCriticalSection( ){ EnterCriticalSection( &m_cs ); };
	void FtpLeaveCriticalSection() { LeaveCriticalSection( &m_cs); }
private:
	CDownFileInfo m_downFileInfo;
	std::vector<DownFileInfoAndID > m_DownList;
	std::vector<DownFileInfoAndID>::iterator m_iterDownlist;
	RTL_CRITICAL_SECTION m_cs;

};

extern CFtpDownLog g_FtpDownLog;