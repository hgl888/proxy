#pragma once
//���ڴ����࣬�ṩ���ڸ�ʽת�����õ��������ڣ�ǰ�����ڵȹ���

#include <Windows.h>

class CSystemTime
{
public:
	CSystemTime(void);
	~CSystemTime(void);
	
	// ��ȡ���������
	bool GetTodayDate(TCHAR * pTodayDate, int ilen);
	// ��ȡ���������
	bool GetYesterDayDate(TCHAR * pYesterdayDate, int ilen);
	
private:
	SYSTEMTIME m_st;
};
