#pragma  once

#include <iostream>
#include <sstream>
#include <fstream>
#include <io.h>
#include <list>
#include "windows.h"
using namespace std;

//����Ŀ¼�����ṹ��
struct STDataDescribe
{
	char   szName[32];
	DWORD  dwSize;
	DWORD  dwStartAddress;
	DWORD  dwEndAddress;
};

//----------------------------------------------------------------------------
//���ݰ���
//�������ݰ��ļ����������ݰ��е���Դ
//Ϊʹ�����ṩ�Ӵ��������ݰ�����ȡ��Ҫ�������ļ�
//���¹������ݰ��е�����
//ɾ�����ݰ�����Ч������
class CPack
{
public:
	CPack();	//ֻ��ʼ��Ҫ������������
	~CPack();

//---------------------------------------------------
//�����ͼ�������Ŀ¼
public:	
	HRESULT Init( char *name, char *path );

	//��ʼ������Ŀ¼���ڴ�,����Ŀ¼�ļ����ڴ�
	void LoadDataRootFromFile();
	
	//�ͷ�Ŀ¼��Ϣ
	void ReleaseRoot();

//---------------------------------------------------
//�������ݰ�����ɺ�Ҫ������Ŀ¼���������¼�����Ŀ¼
public:
	//���ڴ�Ŀ¼��Ϣ����Ŀ¼�ļ�
	void CreateFileRootFromMemory();

	//�������ݰ��ļ����õ����ļ����ݣ�
	void UpDataPackFromMemory( char *name, char *memory, DWORD size );		//���ڴ洴�����ݸ��µ����ݰ�
	void UpDataPackFromFile( char *name, char *path );						//ָ��·���µ��ļ����ݰ��и���

	//��ȡ�����ļ�
	void GetDataFromPack( char *name, char* &outData, DWORD *size );

protected:
	list<STDataDescribe*>    m_listDataRoot;		//����Ŀ¼
	

protected:
	
	ifstream				m_ifstreamDataIn;	//�ļ����ݵĶ˿�
	ofstream				m_ifstreamDataOut;
	char					m_szDataName[256];
	
	ifstream				m_ifstreamRootIn;	//�ļ�Ŀ¼��Ϣ�Ķ˿�
	ofstream				m_ifstreamRootOut;
	char					m_szDataRoot[256];

	ofstream				m_ofstreamDateTxt;	//������־
	char					m_szDate[32];
};

class CPackTool : public CPack
{
public:
	CPackTool();
	~CPackTool();

public:
	//��ָ�����ļ��д����ļ����ݰ�
	void CreateDataPackFromFilePath( char *path );	

	//��ӡ�ļ�Ŀ¼
	void OutputRootToFile();

	//������ݵ��ļ���
	void OutputDataToFile( char* path );
};

