#include <iostream>
#include <sstream>
#include <fstream>
#include <io.h>
#include <list>
#include "windows.h"
using namespace std;

//����Ŀ¼�����ṹ��
struct DataDescribe
{
	char   name[32];
	DWORD  size;
	DWORD  start_addres;
	DWORD  end_address;
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
	list<DataDescribe*>    m_DataRoot;		//����Ŀ¼
	

protected:
	//�ļ����ݵĶ˿�
	ifstream				data_in;
	ofstream				data_out;
	char					data_name[256];

	//�ļ�Ŀ¼��Ϣ�Ķ˿�
	ifstream				root_in;
	ofstream				root_out;
	char					data_root[256];

	//������־
	ofstream				date_txt;	
	char					date[32];
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

