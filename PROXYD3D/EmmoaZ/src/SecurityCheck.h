/********************************************************************************************************************
 *  2008-09-02  ��ȫ����� ��ɨ����̱�������
 *********************************************************************************************************************/
#if !defined(AFX_SECURITYCHECK_H__FD88C52C_AC2D_48BF_8922_BD1B2E8EE9FC__INCLUDED_)
#define AFX_SECURITYCHECK_H__FD88C52C_AC2D_48BF_8922_BD1B2E8EE9FC__INCLUDED_


class SecurityCheck  
{
public:
	SecurityCheck();
	~SecurityCheck();

public:	
	BOOL    Start ();
	void    Stop  ();

	BOOL    IsSecurity()  ;   //��ǰ�Ƿ�ȫ��
public:	
    static SecurityCheck* CreateInstance();
private:
    static DWORD WINAPI ThreadProc(void *p);
	
protected:
	HANDLE    m_hWorkingThread;       //�����߳̾��
	DWORD     m_dwWorkingThreadID;    //�����߳�id    
	HANDLE    m_hEventStop;           //ֹͣ�߳��¼�

	DWORD     m_dwTimeOut;
};

#endif // !defined(AFX_SECURITYCHECK_H__FD88C52C_AC2D_48BF_8922_BD1B2E8EE9FC__INCLUDED_)
