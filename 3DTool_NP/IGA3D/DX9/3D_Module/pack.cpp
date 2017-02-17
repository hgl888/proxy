#include "stdafx.h"

//-------------------------------------------
CPack::CPack()
{
	m_DataRoot.clear();
}

CPack::~CPack()
{
	ReleaseRoot();
}

HRESULT CPack::Init( char *name, char *path )
{
	//���������ļ�
	char path_name_pd[256];
	strcpy( path_name_pd, path );
	strcat( path_name_pd, name );
	strcat( path_name_pd, ".pd");//���ݰ��ļ�����չ����.pd
	strcpy( data_name, path_name_pd );

	//����Ŀ¼�ļ�
	char path_name_pr[256];
	strcpy( path_name_pr, path );
	strcat( path_name_pr, name );
	strcat( path_name_pr, ".pr");//���ݰ��ļ�����չ����.pr
	strcpy( data_root, path_name_pr );

	//��־�ļ�����
	strcpy( date, "pack_Error.txt" );

	return S_OK;
}

void CPack::ReleaseRoot()
{
	//�ͷ�Ŀ¼������
	for( list<DataDescribe*>::iterator ii = m_DataRoot.begin(); ii != m_DataRoot.end(); ii++ )
	{
		delete ((DataDescribe*)(*ii));
	}
	m_DataRoot.clear();
}
void CPack::LoadDataRootFromFile()
{
	//Ѱ��Ŀ¼�ļ�
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( data_root, &file ) )==-1l)//����ļ�·��
	{
		//cout<<"data_root�ļ�û���ҵ�!\n";
		date_txt.open( date, ios::app, 0 );
		date_txt<<"LoadDataRootFromFile: û���ҵ�Ŀ¼�ļ�(*.pr)��"<<endl<<endl;
		date_txt.close();
		return;
	}
	_findclose(lf);

	//����ҵ��˾ͼ��ص�m_DataRoot��
	root_in.open( data_root, ios::binary, 1 );
	
	//��ȡ�ļ�Ŀ¼������
	int file_num = 0x0;
	root_in.seekg( 0x10 );//���õ���ʾ�ļ�������λ�� 
	int tmp1 = 0;
	char tmp = 0;
	for(int ii = 0 ; ii < 4 && root_in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , file_num |=tmp1 ,tmp1 = 0,ii++);

	//�ͷ�ԭ��Ŀ¼��Ϣ
	if( m_DataRoot.size() > 0 )
	{
		ReleaseRoot();
	}

	//��ʼ��ȡ�ļ�Ŀ¼��Ϣ
	for( int i = 0; i < file_num; i++ )
	{
		//��ȡ�ļ����Ƶĳ���
		int name_size = 0x0;
		for(int ii = 0 ; ii < 1 && root_in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , name_size |=tmp1 ,tmp1 = 0,ii++);

		//��ȡ�ļ�����
		char *name = new char[name_size];
		root_in.read( name, name_size );

		//��ȡ�ļ����ݵĴ�С
		int file_size = 0x0;
		for(int ii = 0 ; ii < 4 && root_in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , file_size |=tmp1 ,tmp1 = 0,ii++);

		//��ȡ�ļ����ݵĿ�ʼ��ַ
		int start = 0x0;
		for(int ii = 0 ; ii < 4 && root_in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , start |=tmp1 ,tmp1 = 0,ii++);

		//��ȡ�ļ����ݵĽ�����ַ
		int end = 0x0;
		for(int ii = 0 ; ii < 4 && root_in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , end |=tmp1 ,tmp1 = 0,ii++);
	
		//����һ���ļ�����
		DataDescribe *tmp_DataDescribe	= new DataDescribe();
		strcpy( tmp_DataDescribe->name, name);
		delete name;
		tmp_DataDescribe->size			= file_size;
		tmp_DataDescribe->start_addres	= start;
		tmp_DataDescribe->end_address	= end;

		//��ӵ�m_DataRoot��
		m_DataRoot.push_back( tmp_DataDescribe );
	}
	root_in.close();
}

void CPack::CreateFileRootFromMemory()
{
	int file_num = (int)m_DataRoot.size();
	if( file_num > 0 )
	{
		//����Ŀ¼�ļ�
		root_out.open( data_root, ios::out|ios::binary, 0 );

		//��䱣��λ
		char chr[14] = {};
		for( int i = 4; i < 14; i++ )
		{
			chr[i] = rand()%0xff;
		}
		root_out<<"pr";
		root_out.write( chr, 14 );

		//д���ļ�����
		root_out.write((char*)&file_num, 4 );

		for( list<DataDescribe*>::iterator ii = m_DataRoot.begin(); ii != m_DataRoot.end(); ii++ )
		{
			//�ļ����Ƶ���Ч����
			int name_size = (int)strlen(((DataDescribe*)(*ii))->name) + 1;
			root_out.write((char*)&name_size, 1 );

			//д���ļ�����
			root_out.write(((DataDescribe*)(*ii))->name, name_size );

			//д���ļ�����
			DWORD file_size = ((DataDescribe*)(*ii))->size;
			root_out.write((char*)&file_size, 4 );

			//�ļ�����ʼ��ַ
			DWORD start = ((DataDescribe*)(*ii))->start_addres;
			root_out.write((char*)&start, 4 );

			//�ļ��Ľ�����ַ
			DWORD end = ((DataDescribe*)(*ii))->end_address;
			root_out.write((char*)&end, 4 );
		}
		root_out.close();
	}
	else
	{
		date_txt.open( date, ios::app, 0 );
		date_txt<<"CreatFileRootFromMemory: "<<endl;
		date_txt<<"�����˿յ�Ŀ¼�ļ���������Ŀ¼����û��Ŀ¼��Ϣ"<<endl<<endl;
		date_txt.close();
		
		//����Ŀ¼�ļ�
		root_out.open( data_root, ios::out|ios::binary, 0 );

		//��䱣��λ
		char chr[14] = {};
		for( int i = 4; i < 14; i++ )
		{
			chr[i] = rand()%0xff;
		}
		root_out<<"pr";
		root_out.write( chr, 14 );

		int tmp = 0x0;
		root_out.write( (char*)&tmp, 4 );
		root_out.close();
	}
}

void CPack::UpDataPackFromMemory( char *name, char *memory, DWORD size )
{	
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( data_name, &file ) )==-1l)//���û���ҵ��ļ��Ļ��ʹ����ļ�
	{
		//д���ļ�ͷ
		data_out.open( data_name, ios::binary, 0 );
		data_out<<"pd";

		//��䱣��λ
		char chr[14] = {};
		srand( GetCurrentTime());
		for( int i = 4; i < 14; i++ )
		{
			chr[i] = rand()%0xff;
		}
		data_out.write( chr, 14 );
		data_out.close();
	}
	_findclose(lf);

	
	//-----------------------------------------------------------------

	//�����ڴ�������Ŀ¼
	DWORD start = 0x10;			//������ݰ������һ���ļ��Ľ�����ַ
	if( m_DataRoot.size() > 0 )
	{
		for( list<DataDescribe*>::iterator ii = m_DataRoot.begin(); ii != m_DataRoot.end(); ii++ )
		{
			//�ж��Ƿ�������
			if( strcmp( ((DataDescribe*)(*ii))->name, name) == 0 )
			{
				date_txt.open( date, ios::app, 0 );
				date_txt<<"UpDataPackFromMemory�����ļ������ظ�!"<<endl;
				date_txt<<"�ظ����ļ������ǣ�"<<name<<endl;
				date_txt<<"û�н��и������ļ��ĸ��£�"<<endl<<endl;
				date_txt.close();
				return;
			}
			//�ļ��Ľ�����ַ
			DWORD tmp_end = ((DataDescribe*)(*ii))->end_address;
			if( start < tmp_end )
			{
				start = tmp_end;
			}
		}
	}
	else
	{

	}

	DWORD end	= start + size;
	
	//����һ���ļ�����
	DataDescribe *tmp_DataDescribe	= new DataDescribe();
	if( tmp_DataDescribe != NULL && name != NULL )
	{
		strcpy( tmp_DataDescribe->name, name);
		tmp_DataDescribe->size			= size;
		tmp_DataDescribe->start_addres	= start;
		tmp_DataDescribe->end_address	= end;
	}

	//------------------------------------------------------------------
	//�����ݰ��ļ���׷�������ļ�
	data_out.open( data_name, ios::out|ios::binary|ios::app, 0 );
	if( memory != NULL && size > 0 )
	{
		data_out.seekp( start );
		data_out.write( memory, size );
	}
	else
	{
		date_txt.open( date, ios::app, 0 );
		date_txt<<"UpDataPackFromMemory��д�����ݰ��ļ�ʧ��!"<<endl;
		date_txt<<"��������Ч���ڴ��������Ч�Ĵ�Сֵ��"<<endl;
		date_txt<<"û�н��и������ļ��ĸ��£�"<<endl<<endl;
		date_txt.close();
		delete tmp_DataDescribe;
		return;
	}
	//��ӵ�Ŀ¼��
	m_DataRoot.push_back( tmp_DataDescribe );
	data_out.close();
}

void CPack::UpDataPackFromFile( char *name, char *path )
{
	//��ȡ���������ļ���Ϣ����ʼ���ļ�Ŀ¼
	LoadDataRootFromFile();

	//----------------------------------------------------
	//�ȼ��ظ��µİ���Ŀ¼
	list<DataDescribe*>    tmp_DataRoot; //��ʱ�����ݰ�Ŀ¼
	
	//��������Ŀ¼�ļ���
	char path_name_pr[256];
	strcpy( path_name_pr, path );
	strcat( path_name_pr, name );
	strcat( path_name_pr, ".pr");//���ݰ��ļ�����չ����.pr
	
	//Ѱ��Ŀ¼�ļ�
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( path_name_pr, &file ) )==-1l)//����ļ�·��
	{
		date_txt.open( date, ios::app, 0 );
		date_txt<<"UpDataPackFromFile: û���ҵ����µ�Ŀ¼�ļ�(*.pr)��"<<endl;
		date_txt<<"    �ļ����ƣ�"<<name<<endl;
		date_txt<<"	   UpDataPackFromFile��û�н����κθ��£�"<<endl<<endl;
		date_txt.close();
		return;
	}
	_findclose(lf);
	
	//����ҵ��˾ͼ��ص�tmp_DataRoot��
	ifstream in;
	in.open( path_name_pr, ios::binary, 1 );

	//��ȡ�ļ�Ŀ¼������
	int file_num = 0x0;
	in.seekg( 0x10 );//���õ���ʾ�ļ�������λ�� 
	int tmp1 = 0;
	char tmp = 0;
	for(int ii = 0 ; ii < 4 && in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , file_num |=tmp1 ,tmp1 = 0,ii++);

	//��ʼ��ȡ�ļ�Ŀ¼��Ϣ
	for( int i = 0; i < file_num; i++ )
	{
		//��ȡ�ļ����Ƶĳ���
		int f_name_size = 0x0;
		for(int ii = 0 ; ii < 1 && in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , f_name_size |=tmp1 ,tmp1 = 0,ii++);

		//��ȡ�ļ�����
		char *f_name = new char[f_name_size];
		in.read( f_name, f_name_size );

		//��ȡ�ļ����ݵĴ�С
		int file_size = 0x0;
		for(int ii = 0 ; ii < 4 && in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , file_size |=tmp1 ,tmp1 = 0,ii++);

		//��ȡ�ļ����ݵĿ�ʼ��ַ
		int start = 0x0;
		for(int ii = 0 ; ii < 4 && in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , start |=tmp1 ,tmp1 = 0,ii++);

		//��ȡ�ļ����ݵĽ�����ַ
		int end = 0x0;
		for(int ii = 0 ; ii < 4 && in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , end |=tmp1 ,tmp1 = 0,ii++);
	
		//����һ���ļ�����
		DataDescribe *tmp_DataDescribe	= new DataDescribe();
		strcpy( tmp_DataDescribe->name, f_name);
		delete f_name;
		tmp_DataDescribe->size			= file_size;
		tmp_DataDescribe->start_addres	= start;
		tmp_DataDescribe->end_address	= end;

		//��ӵ�m_DataRoot��
		tmp_DataRoot.push_back( tmp_DataDescribe );
	}
	in.close();
	
	//----------------------------------------------------
	//�жϸ��µ�Ŀ¼�Ƿ���Ч
	if( tmp_DataRoot.size() > 0 )
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
			date_txt.open( date, ios::app, 0 );
			date_txt<<"UpDataPackFromFile��û���ҵ����������ļ�(*.pd)��"<<endl;
			date_txt<<"�������ļ������ڣ�"<<endl<<endl;
			date_txt.close();
			
			//�ͷ���ʱ��Ŀ¼
			for( list<DataDescribe*>::iterator ii = tmp_DataRoot.begin(); ii != tmp_DataRoot.end(); ii++ )
			{
				delete ((DataDescribe*)(*ii));
			}
			tmp_DataRoot.clear();
			return;
		}
		_findclose(lf);

		//�򿪸��°������ļ�
		in.open( path_name_pd, ios::binary, 1 ); //ֻ����ʽ��

		//�ҵ��������ݰ��Ļ��Ϳ�ʼ��ȡ����
		for( list<DataDescribe*>::iterator ii = tmp_DataRoot.begin(); ii != tmp_DataRoot.end(); ii++ )
		{
			//��ȡ�ļ���Ϣ
			DWORD	f_size	= ((DataDescribe*)(*ii))->size;
			int		start	= ((DataDescribe*)(*ii))->start_addres;
			char    *f_name = ((DataDescribe*)(*ii))->name;
			
			//���ڴ�������һ����������
			char *tmp_data = new char[f_size];
			if( tmp_data != NULL )
			{
				in.seekg( start );
				in.read( tmp_data, f_size );
				
				//��ӵ�ԭ���ݰ���
				UpDataPackFromMemory( f_name, tmp_data, f_size );

				delete tmp_data;
				tmp_data = NULL;
			}
		}

		//�رո��°��ļ�
		in.close();

		//�ͷ���ʱ��Ŀ¼
		for( list<DataDescribe*>::iterator ii = tmp_DataRoot.begin(); ii != tmp_DataRoot.end(); ii++ )
		{
			delete ((DataDescribe*)(*ii));
		}
		tmp_DataRoot.clear();

		//������ɺ����´���Ŀ¼�ļ�
		CreateFileRootFromMemory();
	}
	else
	{
		//�������κθ���
		date_txt.open( date, ios::app, 0 );
		date_txt<<"UpDataPackFromFile: ���µ�Ŀ¼�ļ�(*.pr)�����û���ļ�������Ϣ��"<<endl;
		date_txt<<"	   UpDataPackFromFile��û�н����κθ��£�"<<endl<<endl;
		date_txt.close();
	}

	ReleaseRoot();

}

void CPack::GetDataFromPack( char *name, char* &outDate, DWORD *size )
{
	//������ݰ��ļ��Ƿ����
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( data_name, &file ) )==-1l)//����ļ�·��
	{
		//cout<<"GetDataFromPack: �ļ�û���ҵ�!\n";
		date_txt.open( date, ios::app, 0 );
		date_txt<<"GetDataFromPack��û���ҵ������ļ�(*.pd)��"<<endl;
		date_txt<<"�������ļ������ڣ�"<<endl<<endl;
		date_txt.close();
		*size	= 0x0;
		outDate = NULL;
		return;
	}
	_findclose(lf);

	//���������ļ�����Ѱ�������ļ�
	for( list<DataDescribe*>::iterator ii = m_DataRoot.begin(); ii != m_DataRoot.end(); ii++ )
	{
		//������Ѱ���ļ�
		if( strcmp( ((DataDescribe*)(*ii))->name, name) == 0 )
		{
			*size		=((DataDescribe*)(*ii))->size;
			int start	= ((DataDescribe*)(*ii))->start_addres;

			//��ȡ����
			data_in.open( data_name, ios::binary, 1 );
			outDate = new char[*size];
			data_in.seekg(start);
			data_in.read( outDate, *size );
			data_in.close();
			return;
		}
	}
	*size	= 0x0;
	outDate = NULL;
	//date_txt.open( date, ios::app, 0 );
	//date_txt<<"GetDataFromPack����������ļ����Ʋ����� "<<endl;
	//date_txt<<"Ҫ��ȡ���ļ������ǣ�"<<name<<endl;
	//date_txt<<"��ȡ�ļ�����ʧ�ܣ�"<<endl<<endl;
	//date_txt.close();
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

void CPackTool::CreateDataPackFromFilePath( char *path )
{
	//���Ŀ¼��
	if( m_DataRoot.size() > 0 )
	{
		ReleaseRoot();
	}

	cout<<"���������ļ������Ժ�......"<<endl;
	//Ѱ���ļ�
	char path_name[256];
	strcpy( path_name, path );
	strcat( path_name,"*.*");//�����޷��ж�
	_finddata_t file;
	long lf = (long)_findfirst( path_name, &file );
	if( lf == -1l )//����ļ�·��
	{
		cout<<"UpDataPackFromFilePath: �ļ�û���ҵ�!\n";
		return;
	}
	else
	{	
		DWORD start = 0x10;	
		data_out.open( data_name, ios::binary, 0 );	//���½��������ļ�
		data_out<<"pd";

		//��䱣��λ
		char chr[14] = {};
		srand( GetCurrentTime());
		for( int i = 4; i < 14; i++ )
		{
			chr[i] = rand()%0xff;
		}
		data_out.write( chr, 14 );

		//����ļ�����
		do
		{	
			if(file.attrib == _A_SUBDIR)
			{
				continue;
			}

			DWORD size = file.size;
			DWORD end	= start + size;

			//����һ���ļ�����
			DataDescribe *tmp_DataDescribe	= new DataDescribe();
			strcpy( tmp_DataDescribe->name, file.name );
			tmp_DataDescribe->size			= size;
			tmp_DataDescribe->start_addres	= start;
			tmp_DataDescribe->end_address	= end;

			//��ӵ�Ŀ¼��
			m_DataRoot.push_back( tmp_DataDescribe );
			start = end;

			//�������ļ�װ�ص��ڴ�
			char path_file[256];
			strcpy( path_file, path );
			strcat( path_file, file.name );
			ifstream in( path_file, ios::binary, 0 );
			char *tmp = new char[size];
			if( tmp != NULL )
			{
				in.read( tmp, size );

				//��ӵ����ݰ���
				data_out.write( tmp, size );
				delete tmp;
			}
			in.close();
		}
		while( _findnext( lf, &file ) == 0 );
		data_out.close();
	}
	_findclose(lf);

	//����Ŀ¼�ļ�
	CreateFileRootFromMemory();

	cout<<"���ݰ�������ɣ�"<<endl;
}
void CPackTool::OutputRootToFile()
{
	ofstream outTxt( "DataRoot.txt" );
	if( m_DataRoot.size() > 0 )
	{
		for( list<DataDescribe*>::iterator ii = m_DataRoot.begin(); ii != m_DataRoot.end(); ii++ )
		{
			outTxt<<((DataDescribe*)(*ii))->name<<endl;
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
	for( list<DataDescribe*>::iterator ii = m_DataRoot.begin(); ii != m_DataRoot.end(); ii++ )
	{
		char path_name[256];
		strcpy( path_name, path );
		strcat( path_name,((DataDescribe*)(*ii))->name);
		char *name = ((DataDescribe*)(*ii))->name;
		char *memory = NULL;
		DWORD size = ((DataDescribe*)(*ii))->size;

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