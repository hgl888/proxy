/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : z_Memory.h
*	�����ڴ��װ��, add by hgl
*	��ǰ�汾��2, 0, 8, 1226
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1222
*	�޸��ˣ�
*	������ڣ�                                                      
/************************************************************************/

#pragma  once 

#include "stdio.h"

class   CShareRestrictedSD       
{   
public:   
	CShareRestrictedSD();   
	virtual   ~CShareRestrictedSD();   
	SECURITY_ATTRIBUTES* GetSA();  

	PVOID BuildRestrictedSD(PSECURITY_DESCRIPTOR   pSD); 
	VOID  FreeRestrictedSD(PVOID  ptr);  

protected:   
	PVOID     m_pVoid;   
	SECURITY_ATTRIBUTES   m_securityAttributes;   
	SECURITY_DESCRIPTOR   m_securityDescriptor;   
};   

class CShareMemoryBase  
{
public:
	enum{ READONLY,READWRITE};
private:
    CShareMemoryBase(const CShareMemoryBase&) ;
	CShareMemoryBase& operator=(const CShareMemoryBase&) ;

public:
	CShareMemoryBase ();	
	~CShareMemoryBase();

	void  Attach(LPCTSTR szname);
	void  Detach();
    
	void* CreateMem(DWORD dwSize,LPCTSTR szname = NULL);
	void  DeleteMem();

	BOOL  UpdateMem(void* p,DWORD dwSize);
private:
    void CreateFileMap();	
	void DeleteFileMap();
protected:
	HANDLE    m_hFileMap;
	void*     m_pMapViewOfFile;
	DWORD     m_dwSize;
    TCHAR*    m_pszName;

private:
	CShareRestrictedSD       m_sd;

};


/* �����ڴ��ģ���� 
   TType: �洢����������
   size : �洢�ĵ�ǰ������������
*/

template < class TType, size_t size = 1>
class CShareMemory : private CShareMemoryBase
{
public:
	typedef    TType    _type;
private:
    CShareMemory(const CShareMemory&) ;
	CShareMemory& operator=(const CShareMemory&) ;

public:
	CShareMemory ()
	{	
		m_hFileMap = NULL;
		DWORD  dwSize = size * sizeof(TType);
		try
		{	
			CreateMem(dwSize,NULL);
		}
		catch(...)
		{
			char szOutputDebug[256];
			sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
			OutputDebugString( szOutputDebug );
		}
		return;
	};	

	CShareMemory ( LPCTSTR szname)
	{	
		m_hFileMap = NULL;
		DWORD  dwSize = size * sizeof(TType);
		try
		{	
			CreateMem(dwSize,szname);
		}
		catch(...)
		{
			char szOutputDebug[256];
			sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
			OutputDebugString( szOutputDebug );
		}
		return;
	}


	~CShareMemory()
	{	
		DeleteMem();
	};

	_type& operator[](int i)
	{

		static _type t;
        if ( m_hFileMap == NULL || i > size)  
		{
			throw("operator[] faile");
			return t;
		}

        return  *((_type*)((char*)m_pMapViewOfFile + i * sizeof(_type)));
	}

	BOOL Update()
	{
		BOOL bUpdate = false;

		if ( m_pMapViewOfFile == NULL )
		{
			char szOutputDebug[256];
			sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
			OutputDebugString( szOutputDebug );
			//return false;
		}
        bUpdate = UpdateMem(m_pMapViewOfFile,m_dwSize);
		return bUpdate;
	}
};
