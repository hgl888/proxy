#include "stdafx.h"

#include "tchar.h"
#include "z_FileIni.h"
#include "pack.h"

//-------------------------------------------
CPack::CPack()
{
	m_listDataRoot.clear();
}

CPack::~CPack()
{
	ReleaseRoot();
}

HRESULT CPack::Init( char *name, char *path )
{
	//���������ļ�
	char szPathNamePd[256];
	strcpy( szPathNamePd, path );
	strcat( szPathNamePd, name );
	strcat( szPathNamePd, ".pd");//���ݰ��ļ�����չ����.pd
	strcpy( m_szDataName, szPathNamePd );

	//����Ŀ¼�ļ�
	char szPathNamePr[256];
	strcpy( szPathNamePr, path );
	strcat( szPathNamePr, name );
	strcat( szPathNamePr, ".pr");//���ݰ��ļ�����չ����.pr
	strcpy( m_szDataRoot, szPathNamePr );

	//��־�ļ�����
	strcpy( m_szDate, "pack_Error.txt" );

	return S_OK;
}

void CPack::ReleaseRoot()
{
	//�ͷ�Ŀ¼������
	for( list<STDataDescribe*>::iterator ii = m_listDataRoot.begin(); ii != m_listDataRoot.end(); ii++ )
	{
		delete ((STDataDescribe*)(*ii));
	}
	m_listDataRoot.clear();
}
void CPack::LoadDataRootFromFile()
{
	//Ѱ��Ŀ¼�ļ�
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( m_szDataRoot, &file ) )==-1l)//����ļ�·��
	{
		//cout<<"data_root�ļ�û���ҵ�!\n";
		m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
		m_ofstreamDateTxt<<"LoadDataRootFromFile: û���ҵ�Ŀ¼�ļ�(*.pr)��"<<endl<<endl;
		m_ofstreamDateTxt.close();
		return;
	}
	_findclose(lf);

	//����ҵ��˾ͼ��ص�m_DataRoot��
	m_ifstreamRootIn.open( m_szDataRoot, ios::binary, 1 );
	
	//��ȡ�ļ�Ŀ¼������
	int ifileNum = 0x0;
	m_ifstreamRootIn.seekg( 0x10 );//���õ���ʾ�ļ�������λ�� 
	int iTemp = 0;
	char chTmp = 0;
	for(int ii = 0 ; ii < 4 && m_ifstreamRootIn.get(chTmp) ; ii++)
	{
		iTemp = (int)((unsigned char)chTmp);
		iTemp<<= ii*8 ;
		ifileNum |=iTemp;
		iTemp = 0;
	}

	if( m_listDataRoot.size() > 0 )
	{
		ReleaseRoot();
	}

	for( int i = 0; i < ifileNum; i++ )
	{
		//��ȡ�ļ����Ƶĳ���
		int iNameSize = 0x0;
		for(int ii = 0 ; ii < 1 && m_ifstreamRootIn.get(chTmp) ; iTemp = (int)((unsigned char)chTmp) ,iTemp<<= ii*8 , iNameSize |=iTemp ,iTemp = 0,ii++);

		//��ȡ�ļ�����
		char *pszName = new char[iNameSize];
		m_ifstreamRootIn.read( pszName, iNameSize );

		//��ȡ�ļ����ݵĴ�С
		int iFileSize = 0x0;
		for(int ii = 0 ; ii < 4 && m_ifstreamRootIn.get(chTmp) ; iTemp = (int)((unsigned char)chTmp) ,iTemp<<= ii*8 , iFileSize |=iTemp ,iTemp = 0,ii++);

		//��ȡ�ļ����ݵĿ�ʼ��ַ
		int iStart = 0x0;
		for(int ii = 0 ; ii < 4 && m_ifstreamRootIn.get(chTmp) ; iTemp = (int)((unsigned char)chTmp) ,iTemp<<= ii*8 , iStart |=iTemp ,iTemp = 0,ii++);

		//��ȡ�ļ����ݵĽ�����ַ
		int iEnd = 0x0;
		for(int ii = 0 ; ii < 4 && m_ifstreamRootIn.get(chTmp) ; iTemp = (int)((unsigned char)chTmp) ,iTemp<<= ii*8 , iEnd |=iTemp ,iTemp = 0,ii++);
	
		//����һ���ļ�����
		STDataDescribe *pDataDescribe	= new STDataDescribe();
		strcpy( pDataDescribe->szName, pszName);
		delete pszName;
		pDataDescribe->dwSize			= iFileSize;
		pDataDescribe->dwStartAddress	= iStart;
		pDataDescribe->dwEndAddress	= iEnd;

		//��ӵ�m_DataRoot��
		m_listDataRoot.push_back( pDataDescribe );
	}
	m_ifstreamRootIn.close();
}

void CPack::CreateFileRootFromMemory()
{
	int iFileNum = (int)m_listDataRoot.size();
	if( iFileNum > 0 )
	{
		//����Ŀ¼�ļ�
		m_ifstreamRootOut.open( m_szDataRoot, ios::out|ios::binary, 0 );

		//��䱣��λ
		char szChr[14] = {};
		for( int i = 4; i < 14; i++ )
		{
			szChr[i] = rand()%0xff;
		}
		m_ifstreamRootOut<<"pr";
		m_ifstreamRootOut.write( szChr, 14 );

		//д���ļ�����
		m_ifstreamRootOut.write((char*)&iFileNum, 4 );

		for( list<STDataDescribe*>::iterator ii = m_listDataRoot.begin(); ii != m_listDataRoot.end(); ii++ )
		{
			//�ļ����Ƶ���Ч����
			int name_size = (int)strlen(((STDataDescribe*)(*ii))->szName) + 1;
			m_ifstreamRootOut.write((char*)&name_size, 1 );

			//д���ļ�����
			m_ifstreamRootOut.write(((STDataDescribe*)(*ii))->szName, name_size );

			//д���ļ�����
			DWORD file_size = ((STDataDescribe*)(*ii))->dwSize;
			m_ifstreamRootOut.write((char*)&file_size, 4 );

			//�ļ�����ʼ��ַ
			DWORD start = ((STDataDescribe*)(*ii))->dwStartAddress;
			m_ifstreamRootOut.write((char*)&start, 4 );

			//�ļ��Ľ�����ַ
			DWORD end = ((STDataDescribe*)(*ii))->dwEndAddress;
			m_ifstreamRootOut.write((char*)&end, 4 );
		}
		m_ifstreamRootOut.close();
	}
	else
	{
		m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
		m_ofstreamDateTxt<<"CreatFileRootFromMemory: "<<endl;
		m_ofstreamDateTxt<<"�����˿յ�Ŀ¼�ļ���������Ŀ¼����û��Ŀ¼��Ϣ"<<endl<<endl;
		m_ofstreamDateTxt.close();
		
		//����Ŀ¼�ļ�
		m_ifstreamRootOut.open( m_szDataRoot, ios::out|ios::binary, 0 );

		//��䱣��λ
		char chr[14] = {};
		for( int i = 4; i < 14; i++ )
		{
			chr[i] = rand()%0xff;
		}
		m_ifstreamRootOut<<"pr";
		m_ifstreamRootOut.write( chr, 14 );

		int tmp = 0x0;
		m_ifstreamRootOut.write( (char*)&tmp, 4 );
		m_ifstreamRootOut.close();
	}
}

void CPack::UpDataPackFromMemory( char *name, char *memory, DWORD size )
{	
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( m_szDataName, &file ) )==-1l)//���û���ҵ��ļ��Ļ��ʹ����ļ�
	{
		//д���ļ�ͷ
		m_ifstreamDataOut.open( m_szDataName, ios::binary, 0 );
		m_ifstreamDataOut<<"pd";

		//��䱣��λ
		char chr[14] = {};
		srand( GetCurrentTime());
		for( int i = 4; i < 14; i++ )
		{
			chr[i] = rand()%0xff;
		}
		m_ifstreamDataOut.write( chr, 14 );
		m_ifstreamDataOut.close();
	}
	_findclose(lf);

	
	//-----------------------------------------------------------------

	//�����ڴ�������Ŀ¼
	DWORD dwstart = 0x10;			//������ݰ������һ���ļ��Ľ�����ַ
	if( m_listDataRoot.size() > 0 )
	{
		for( list<STDataDescribe*>::iterator ii = m_listDataRoot.begin(); ii != m_listDataRoot.end(); ii++ )
		{
			//�ж��Ƿ�������
			if( strcmp( ((STDataDescribe*)(*ii))->szName, name) == 0 )
			{
				m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
				m_ofstreamDateTxt<<"UpDataPackFromMemory�����ļ������ظ�!"<<endl;
				m_ofstreamDateTxt<<"�ظ����ļ������ǣ�"<<name<<endl;
				m_ofstreamDateTxt<<"û�н��и������ļ��ĸ��£�"<<endl<<endl;
				m_ofstreamDateTxt.close();
				return;
			}
			//�ļ��Ľ�����ַ
			DWORD tmp_end = ((STDataDescribe*)(*ii))->dwEndAddress;
			if( dwstart < tmp_end )
			{
				dwstart = tmp_end;
			}
		}
	}

	DWORD dwEnd	= dwstart + size;
	
	//����һ���ļ�����
	STDataDescribe *pDataDescribe	= new STDataDescribe();
	if( pDataDescribe != NULL && name != NULL )
	{
		strcpy( pDataDescribe->szName, name);
		pDataDescribe->dwSize			= size;
		pDataDescribe->dwStartAddress	= dwstart;
		pDataDescribe->dwEndAddress	= dwEnd;
	}

	//------------------------------------------------------------------
	//�����ݰ��ļ���׷�������ļ�
	m_ifstreamDataOut.open( m_szDataName, ios::out|ios::binary|ios::app, 0 );
	if( memory != NULL && size > 0 )
	{
		m_ifstreamDataOut.seekp( dwstart );
		m_ifstreamDataOut.write( memory, size );
	}
	else
	{
		m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
		m_ofstreamDateTxt<<"UpDataPackFromMemory��д�����ݰ��ļ�ʧ��!"<<endl;
		m_ofstreamDateTxt<<"��������Ч���ڴ��������Ч�Ĵ�Сֵ��"<<endl;
		m_ofstreamDateTxt<<"û�н��и������ļ��ĸ��£�"<<endl<<endl;
		m_ofstreamDateTxt.close();
		delete pDataDescribe;
		return;
	}
	//��ӵ�Ŀ¼��
	m_listDataRoot.push_back( pDataDescribe );
	m_ifstreamDataOut.close();
}

void CPack::UpDataPackFromFile( char *name, char *path )
{
	LoadDataRootFromFile();
	list<STDataDescribe*>    listDataRoot; 
	
	char szPathNamePr[256];
	strcpy( szPathNamePr, path );
	strcat( szPathNamePr, name );
	strcat( szPathNamePr, ".pr");//���ݰ��ļ�����չ����.pr
	
	//Ѱ��Ŀ¼�ļ�
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( szPathNamePr, &file ) )==-1l)//����ļ�·��
	{
		m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
		m_ofstreamDateTxt<<"UpDataPackFromFile: û���ҵ����µ�Ŀ¼�ļ�(*.pr)��"<<endl;
		m_ofstreamDateTxt<<"    �ļ����ƣ�"<<name<<endl;
		m_ofstreamDateTxt<<"	   UpDataPackFromFile��û�н����κθ��£�"<<endl<<endl;
		m_ofstreamDateTxt.close();
		return;
	}
	_findclose(lf);
	
	//����ҵ��˾ͼ��ص�tmp_DataRoot��
	ifstream ifstreamIn;
	ifstreamIn.open( szPathNamePr, ios::binary, 1 );

	//��ȡ�ļ�Ŀ¼������
	int iFileNum = 0x0;
	ifstreamIn.seekg( 0x10 );//���õ���ʾ�ļ�������λ�� 
	int tmp1 = 0;
	char tmp = 0;
	for(int ii = 0 ; ii < 4 && ifstreamIn.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , iFileNum |=tmp1 ,tmp1 = 0,ii++);

	//��ʼ��ȡ�ļ�Ŀ¼��Ϣ
	for( int i = 0; i < iFileNum; i++ )
	{
		//��ȡ�ļ����Ƶĳ���
		int f_name_size = 0x0;
		for(int ii = 0 ; ii < 1 && ifstreamIn.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , f_name_size |=tmp1 ,tmp1 = 0,ii++);

		//��ȡ�ļ�����
		char *f_name = new char[f_name_size];
		ifstreamIn.read( f_name, f_name_size );

		//��ȡ�ļ����ݵĴ�С
		int file_size = 0x0;
		for(int ii = 0 ; ii < 4 && ifstreamIn.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , file_size |=tmp1 ,tmp1 = 0,ii++);

		//��ȡ�ļ����ݵĿ�ʼ��ַ
		int start = 0x0;
		for(int ii = 0 ; ii < 4 && ifstreamIn.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , start |=tmp1 ,tmp1 = 0,ii++);

		//��ȡ�ļ����ݵĽ�����ַ
		int end = 0x0;
		for(int ii = 0 ; ii < 4 && ifstreamIn.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , end |=tmp1 ,tmp1 = 0,ii++);
	
		//����һ���ļ�����
		STDataDescribe *tmp_DataDescribe	= new STDataDescribe();
		strcpy( tmp_DataDescribe->szName, f_name);
		delete f_name;
		tmp_DataDescribe->dwSize			= file_size;
		tmp_DataDescribe->dwStartAddress	= start;
		tmp_DataDescribe->dwEndAddress	= end;

		//��ӵ�m_DataRoot��
		listDataRoot.push_back( tmp_DataDescribe );
	}
	ifstreamIn.close();
	
	//----------------------------------------------------
	//�жϸ��µ�Ŀ¼�Ƿ���Ч
	if( listDataRoot.size() > 0 )
	{
		//Ѱ�����ݸ��°�
		char path_name_pd[256];
		strcpy( path_name_pd, path );
		strcat( path_name_pd, name );
		strcat( path_name_pd, ".pd");//���ݰ��ļ�����չ����.pr
		_finddata_t file;
		long lf;
		if( ( lf = (long)_findfirst( path_name_pd, &file ) )==-1l)//����ļ�·��
		{
			m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
			m_ofstreamDateTxt<<"UpDataPackFromFile��û���ҵ����������ļ�(*.pd)��"<<endl;
			m_ofstreamDateTxt<<"�������ļ������ڣ�"<<endl<<endl;
			m_ofstreamDateTxt.close();
			
			//�ͷ���ʱ��Ŀ¼
			for( list<STDataDescribe*>::iterator ii = listDataRoot.begin(); ii != listDataRoot.end(); ii++ )
			{
				delete ((STDataDescribe*)(*ii));
			}
			listDataRoot.clear();
			return;
		}
		_findclose(lf);

		//�򿪸��°������ļ�
		ifstreamIn.open( path_name_pd, ios::binary, 1 ); //ֻ����ʽ��

		//�ҵ��������ݰ��Ļ��Ϳ�ʼ��ȡ����
		for( list<STDataDescribe*>::iterator ii = listDataRoot.begin(); ii != listDataRoot.end(); ii++ )
		{
			//��ȡ�ļ���Ϣ
			DWORD	f_size	= ((STDataDescribe*)(*ii))->dwSize;
			int		start	= ((STDataDescribe*)(*ii))->dwStartAddress;
			char    *f_name = ((STDataDescribe*)(*ii))->szName;
			
			//���ڴ�������һ����������
			char *tmp_data = new char[f_size];
			if( tmp_data != NULL )
			{
				ifstreamIn.seekg( start );
				ifstreamIn.read( tmp_data, f_size );
				
				//��ӵ�ԭ���ݰ���
				UpDataPackFromMemory( f_name, tmp_data, f_size );

				delete tmp_data;
				tmp_data = NULL;
			}
		}

		//�رո��°��ļ�
		ifstreamIn.close();

		//�ͷ���ʱ��Ŀ¼
		for( list<STDataDescribe*>::iterator ii = listDataRoot.begin(); ii != listDataRoot.end(); ii++ )
		{
			delete ((STDataDescribe*)(*ii));
		}
		listDataRoot.clear();

		//������ɺ����´���Ŀ¼�ļ�
		CreateFileRootFromMemory();
	}
	else
	{
		//�������κθ���
		m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
		m_ofstreamDateTxt<<"UpDataPackFromFile: ���µ�Ŀ¼�ļ�(*.pr)�����û���ļ�������Ϣ��"<<endl;
		m_ofstreamDateTxt<<"	   UpDataPackFromFile��û�н����κθ��£�"<<endl<<endl;
		m_ofstreamDateTxt.close();
	}

	ReleaseRoot();

}

void CPack::GetDataFromPack( char *name, char* &outDate, DWORD *pdwSize )
{
	//������ݰ��ļ��Ƿ����
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( m_szDataName, &file ) )==-1l)//����ļ�·��
	{
		//cout<<"GetDataFromPack: �ļ�û���ҵ�!\n";
		m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
		m_ofstreamDateTxt<<"GetDataFromPack��û���ҵ������ļ�(*.pd)��"<<endl;
		m_ofstreamDateTxt<<"�������ļ������ڣ�"<<endl<<endl;
		m_ofstreamDateTxt.close();
		*pdwSize	= 0x0;
		outDate = NULL;
		return;
	}
	_findclose(lf);

	//���������ļ�����Ѱ�������ļ�
	for( list<STDataDescribe*>::iterator ii = m_listDataRoot.begin(); ii != m_listDataRoot.end(); ii++ )
	{
		//������Ѱ���ļ�
		if( strcmp( ((STDataDescribe*)(*ii))->szName, name) == 0 )
		{
			*pdwSize		=((STDataDescribe*)(*ii))->dwSize;
			int start	= ((STDataDescribe*)(*ii))->dwStartAddress;

			//��ȡ����
			m_ifstreamDataIn.open( m_szDataName, ios::binary, 1 );
			outDate = new char[*pdwSize];
			m_ifstreamDataIn.seekg(start);
			m_ifstreamDataIn.read( outDate, *pdwSize );
			m_ifstreamDataIn.close();
			return;
		}
	}
	*pdwSize	= 0x0;
	outDate = NULL;

	return;
}

//------------------------------------------------------------------------
//���ݰ�����
CPackTool::CPackTool():CPack()
{

}

CPackTool::~CPackTool()
{

}

void CPackTool::CreateDataPackFromFilePath( char *pszPath )
{
	if( m_listDataRoot.size() > 0 )
	{
		ReleaseRoot();
	}

	cout<<"���������ļ������Ժ�......"<<endl;
	//Ѱ���ļ�
	char szPathName[256];
	strcpy( szPathName, pszPath );
	strcat( szPathName,"*.*");//�����޷��ж�
	_finddata_t file;
	long lf = (long)_findfirst( szPathName, &file );
	if( lf == -1l )//����ļ�·��
	{
		cout<<"UpDataPackFromFilePath: �ļ�û���ҵ�!\n";
		return;
	}
	else
	{	
		DWORD dwStart = 0x10;	
		m_ifstreamDataOut.open( m_szDataName, ios::binary, 0 );	//���½��������ļ�
		m_ifstreamDataOut<<"pd";

		//��䱣��λ
		char chr[14] = {};
		srand( GetCurrentTime());
		for( int i = 4; i < 14; i++ )
		{
			chr[i] = rand()%0xff;
		}
		m_ifstreamDataOut.write( chr, 14 );

		//����ļ�����
		do
		{	
			if(file.attrib == _A_SUBDIR)
			{
				continue;
			}

			DWORD dwSize = file.size;
			DWORD end	= dwStart + dwSize;

			//����һ���ļ�����
			STDataDescribe *tmp_DataDescribe	= new STDataDescribe();
			strcpy( tmp_DataDescribe->szName, file.name );
			tmp_DataDescribe->dwSize			= dwSize;
			tmp_DataDescribe->dwStartAddress	= dwStart;
			tmp_DataDescribe->dwEndAddress	= end;

			//��ӵ�Ŀ¼��
			m_listDataRoot.push_back( tmp_DataDescribe );
			dwStart = end;

			//�������ļ�װ�ص��ڴ�
			char path_file[256];
			strcpy( path_file, pszPath );
			strcat( path_file, file.name );
			ifstream in( path_file, ios::binary, 0 );
			char *tmp = new char[dwSize];
			if( tmp != NULL )
			{
				in.read( tmp, dwSize );

				//��ӵ����ݰ���
				m_ifstreamDataOut.write( tmp, dwSize );
				delete tmp;
			}
			in.close();
		}
		while( _findnext( lf, &file ) == 0 );
		m_ifstreamDataOut.close();
	}
	_findclose(lf);

	//����Ŀ¼�ļ�
	CreateFileRootFromMemory();

	cout<<"���ݰ�������ɣ�"<<endl;
}
void CPackTool::OutputRootToFile()
{
	ofstream outTxt( "DataRoot.txt" );
	if( m_listDataRoot.size() > 0 )
	{
		for( list<STDataDescribe*>::iterator ii = m_listDataRoot.begin(); ii != m_listDataRoot.end(); ii++ )
		{
			outTxt<<((STDataDescribe*)(*ii))->szName<<endl;
		}
	}
	else
	{
		outTxt<<"Ŀ¼��û���κ�������Ϣ" <<endl;
	}
	outTxt.close();
}

void CPackTool::OutputDataToFile( char *path )
{
	for( list<STDataDescribe*>::iterator ii = m_listDataRoot.begin(); ii != m_listDataRoot.end(); ii++ )
	{
		char path_name[256];
		strcpy( path_name, path );
		strcat( path_name,((STDataDescribe*)(*ii))->szName);
		char *name = ((STDataDescribe*)(*ii))->szName;
		char *memory = NULL;
		DWORD size = ((STDataDescribe*)(*ii))->dwSize;

		GetDataFromPack( name, memory, &size );

		if( memory != NULL )
		{
			ofstream out( path_name, ios::binary, 0 );
			out.write( memory, size );
			out.close();
			delete memory;
		}
	}
}