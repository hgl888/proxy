// Markup.h: interface for the CMarkup class.
//
// Markup Release 8.2
// Copyright (C) 1999-2006 First Objective Software, Inc. All rights reserved
// Go to www.firstobject.com for the latest CMarkup and EDOM documentation
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.

#if !defined(XMLMARKUP_H)
#define XMLMARKUP_H

#if _MSC_VER > 1000
#pragma once
#pragma warning(disable:4996) // suppress VS 2005 deprecated function warnings
#endif // _MSC_VER > 1000

#ifdef _DEBUG
#define _DS(i) (i?&((LPCTSTR)m_strDoc)[m_aPos[i].nStart]:0)
#define MARKUP_SETDEBUGSTATE m_pMainDS=_DS(m_iPos); m_pChildDS=_DS(m_iPosChild)
#else
#define MARKUP_SETDEBUGSTATE
#endif

class CXMLBase  
{
public:
	CXMLBase() { SetDoc( NULL ); InitDocFlags(); };
	CXMLBase( LPCTSTR szDoc ) { SetDoc( szDoc ); InitDocFlags(); };
	CXMLBase( int nFlags ) { SetDoc( NULL ); m_nFlags = nFlags; };
	CXMLBase( const CXMLBase& markup ) { *this = markup; };
	void operator=( const CXMLBase& markup );
	~CXMLBase() 
	{};

	// Navigate
	virtual bool Load( LPCTSTR szFileName );
	
	bool SetDoc( LPCTSTR szDoc );
	bool IsWellFormed();
	bool FindElem( LPCTSTR szName=NULL );
	bool FindChildElem( LPCTSTR szName=NULL );
	bool IntoElem();
	bool OutOfElem();
	void ResetChildPos() { x_SetPos(m_iPosParent,m_iPos,0); };
	void ResetMainPos() { x_SetPos(m_iPosParent,0,0); };
	void ResetPos() { x_SetPos(0,0,0); };
	CString GetTagName() const;
	CString GetChildTagName() const { return x_GetTagName(m_iPosChild); };
	CString GetData() const { return x_GetData(m_iPos); };
	CString GetChildData() const { return x_GetData(m_iPosChild); };
	CString GetElemContent() const { return x_GetElemContent(m_iPos); };
	CString GetAttrib( LPCTSTR szAttrib ) const { return x_GetAttrib(m_iPos,szAttrib); };
	CString GetChildAttrib( LPCTSTR szAttrib ) const { return x_GetAttrib(m_iPosChild,szAttrib); };
	CString GetAttribName( int n ) const;
	int FindNode( int nType=0 );
	int GetNodeType() { return m_nNodeType; };
	bool SavePos( LPCTSTR szPosName=_T("") );
	bool RestorePos( LPCTSTR szPosName=_T("") );
	const CString& GetError() const { return m_strError; };
	int GetDocFlags() const { return m_nFlags; };
	void SetDocFlags( int nFlags ) { m_nFlags = nFlags; };
	enum MarkupDocFlags
	{
		MDF_IGNORECASE = 8,
	};
	enum MarkupNodeFlags
	{
		MNF_WITHCDATA      = 0x01,
		MNF_WITHNOLINES    = 0x02,
		MNF_WITHXHTMLSPACE = 0x04,
		MNF_WITHREFS       = 0x08,
		MNF_WITHNOEND      = 0x10,
		MNF_ESCAPEQUOTES  = 0x100,
		MNF_NONENDED   = 0x100000,
		MNF_ILLDATA    = 0x200000,
	};
	enum MarkupNodeType
	{
		MNT_ELEMENT					= 1,  // 0x01
		MNT_TEXT					= 2,  // 0x02
		MNT_WHITESPACE				= 4,  // 0x04
		MNT_CDATA_SECTION			= 8,  // 0x08
		MNT_PROCESSING_INSTRUCTION	= 16, // 0x10
		MNT_COMMENT					= 32, // 0x20
		MNT_DOCUMENT_TYPE			= 64, // 0x40
		MNT_EXCLUDE_WHITESPACE		= 123,// 0x7b
		MNT_LONE_END_TAG			= 128,// 0x80
		MNT_NODE_ERROR              = 32768 // 0x8000
	};
	bool GetOffsets( int& nStart, int& nEnd ) const;

	// Create
	bool Save( LPCTSTR szFileName );
	const CString& GetDoc() const { return m_strDoc; };
	bool AddElem( LPCTSTR szName, LPCTSTR szData=NULL, int nFlags=0 ) { return x_AddElem(szName,szData,nFlags); };
	bool InsertElem( LPCTSTR szName, LPCTSTR szData=NULL, int nFlags=0 ) { return x_AddElem(szName,szData,nFlags|MNF_INSERT); };
	bool AddChildElem( LPCTSTR szName, LPCTSTR szData=NULL, int nFlags=0 ) { return x_AddElem(szName,szData,nFlags|MNF_CHILD); };
	bool InsertChildElem( LPCTSTR szName, LPCTSTR szData=NULL, int nFlags=0 ) { return x_AddElem(szName,szData,nFlags|MNF_INSERT|MNF_CHILD); };
	bool AddElem( LPCTSTR szName, int nValue, int nFlags=0 ) { return x_AddElem(szName,nValue,nFlags); };
	bool InsertElem( LPCTSTR szName, int nValue, int nFlags=0 ) { return x_AddElem(szName,nValue,nFlags|MNF_INSERT); };
	bool AddChildElem( LPCTSTR szName, int nValue, int nFlags=0 ) { return x_AddElem(szName,nValue,nFlags|MNF_CHILD); };
	bool InsertChildElem( LPCTSTR szName, int nValue, int nFlags=0 ) { return x_AddElem(szName,nValue,nFlags|MNF_INSERT|MNF_CHILD); };
	bool AddAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_iPos,szAttrib,szValue); };
	bool AddChildAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_iPosChild,szAttrib,szValue); };
	bool AddAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_iPos,szAttrib,nValue); };
	bool AddChildAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_iPosChild,szAttrib,nValue); };
	bool AddSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,0); };
	bool InsertSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,MNF_INSERT); };
	CString GetSubDoc() const { return x_GetSubDoc(m_iPos); };
	bool AddChildSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,MNF_CHILD); };
	bool InsertChildSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,MNF_CHILD|MNF_INSERT); };
	CString GetChildSubDoc() const { return x_GetSubDoc(m_iPosChild); };
	bool AddNode( int nType, LPCTSTR szText ) { return x_AddNode(nType,szText,0); };
	bool InsertNode( int nType, LPCTSTR szText ) { return x_AddNode(nType,szText,MNF_INSERT); };

	// Modify
	bool RemoveElem();
	bool RemoveChildElem();
	bool RemoveNode();
	bool SetAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_iPos,szAttrib,szValue); };
	bool SetChildAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_iPosChild,szAttrib,szValue); };
	bool SetAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_iPos,szAttrib,nValue); };
	bool SetChildAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_iPosChild,szAttrib,nValue); };
	bool SetData( LPCTSTR szData, int nFlags=0 ) { return x_SetData(m_iPos,szData,nFlags); };
	bool SetChildData( LPCTSTR szData, int nFlags=0 ) { return x_SetData(m_iPosChild,szData,nFlags); };
	bool SetData( int nValue ) { return x_SetData(m_iPos,nValue); };
	bool SetChildData( int nValue ) { return x_SetData(m_iPosChild,nValue); };
	bool SetElemContent( LPCTSTR szContent ) { return x_SetElemContent(szContent); };

	// Utility
	static bool ReadTextFile( LPCTSTR szFileName, CString& strDoc, CString* pstrError=NULL, int* pnFlags=NULL );
	
	static bool WriteTextFile( LPCTSTR szFileName, CString& strDoc, CString* pstrError=NULL, int* pnFlags=NULL );
	static CString EscapeText( LPCTSTR szText, int nFlags = 0 );
	static CString UnescapeText( LPCTSTR szText, int nTextLength = -1 );

protected:

#ifdef _DEBUG
	LPCTSTR m_pMainDS;
	LPCTSTR m_pChildDS;
#endif

	
	CString m_strDoc;
	CString m_strError;

	int m_iPosParent;
	int m_iPos;
	int m_iPosChild;
	int m_iPosFree;
	int m_iPosDeleted;
	int m_nNodeType;
	int m_nNodeOffset;
	int m_nNodeLength;
	int m_nFlags;

	struct ElemPos
	{
		ElemPos() {};
		ElemPos( const ElemPos& pos ) { *this = pos; };
		enum { EP_STBITS=22, EP_STMASK=0x2fffff, EP_LEVMASK=0xffff };
		int StartTagLen() const { return (nTagLengths & EP_STMASK); };
		void SetStartTagLen( int n ) { nTagLengths = (nTagLengths & ~EP_STMASK) + n; };
		void AdjustStartTagLen( int n ) { nTagLengths += n; };
		int EndTagLen() const { return (nTagLengths >> EP_STBITS); };
		void SetEndTagLen( int n ) { nTagLengths = (nTagLengths & EP_STMASK) + (n << EP_STBITS); };
		bool IsEmptyElement() { return (StartTagLen()==nLength)?true:false; };
		int StartContent() const { return nStart + StartTagLen(); };
		int ContentLen() const { return nLength - StartTagLen() - EndTagLen(); };
		int StartAfter() const { return nStart + nLength; };
		int Level() const { return nFlags & EP_LEVMASK; };
		void SetLevel( int nLev ) { nFlags = (nFlags & ~EP_LEVMASK) | nLev; };
		void ClearVirtualParent() { memset(this,0,sizeof(ElemPos)); };

		// Memory size: 8 32-bit integers == 32 bytes
		int nStart;
		int nLength;
		int nTagLengths; // 22 bits 4MB limit for start tag, 10 bits 1K limit for end tag
		int nFlags; // 16 bits flags, 16 bits level 65536 depth limit
		int iElemParent;
		int iElemChild; // first child
		int iElemNext;
		int iElemPrev; // if this is first child, iElemPrev points to last
	};

	enum MarkupNodeFlagsInternal
	{
		MNF_REPLACE    = 0x001000,
		MNF_INSERT     = 0x002000,
		MNF_CHILD      = 0x004000,
		MNF_QUOTED     = 0x008000,
		MNF_EMPTY      = 0x010000,
		MNF_DELETED    = 0x020000,
		MNF_FIRST      = 0x080000,
		MNF_PUBLIC     = 0x300000,
		MNF_ILLFORMED  = 0x800000,
		MNF_USER      = 0xf000000,
	};

	struct NodePos
	{
		NodePos() {};
		NodePos( int n ) { nFlags=n; nNodeType=0; nStart=0; nLength=0; };
		int nNodeType;
		int nStart;
		int nLength;
		int nFlags;
		CString strMeta;
	};

	struct TokenPos
	{
		TokenPos( LPCTSTR sz, int n ) { Clear(); szDoc=sz; nTokenFlags=n; };
		void Clear() { nL=0; nR=-1; nNext=0; };
		int Length() const { return nR - nL + 1; };
		bool Match( LPCTSTR szName )
		{
			int nLen = nR - nL + 1;
			if ( nTokenFlags & MDF_IGNORECASE )
				return ( (_tcsncicmp( &szDoc[nL], szName, nLen ) == 0)
					&& ( szName[nLen] == _T('\0') || _tcschr(_T(" =/[]"),szName[nLen]) ) );
			else
				return ( (_tcsnccmp( &szDoc[nL], szName, nLen ) == 0)
					&& ( szName[nLen] == _T('\0') || _tcschr(_T(" =/[]"),szName[nLen]) ) );
		};
		int nL;
		int nR;
		int nNext;
		LPCTSTR szDoc;
		int nTokenFlags;
		int nPreSpaceStart;
		int nPreSpaceLength;
	};

	struct SavedPos
	{
		SavedPos() { nSavedPosFlags=0; iPos=0; };
		CString strName;
		int iPos;
		int nSavedPosFlags;
	};

	struct SavedPosMap
	{
		SavedPosMap() { pTable = NULL; };
		~SavedPosMap() { RemoveAll(); };
		void RemoveAll() { if (pTable) Release(); pTable=NULL; };
		enum { SPM_SIZE = 7, SPM_MAIN = 1, SPM_CHILD = 2, SPM_USED = 4, SPM_LAST = 8 };
		void Release() { for (int n=0;n<SPM_SIZE;++n) if (pTable[n]) delete[] pTable[n]; delete[] pTable; };
		void AllocMapTable() { pTable = new SavedPos*[SPM_SIZE]; for (int n=0; n<SPM_SIZE; ++n) pTable[n]=NULL; };
		int Hash( LPCTSTR szName ) { int n=0; while (*szName) n += *szName++; return n % SPM_SIZE; };
		SavedPos** pTable;
	};
	SavedPosMap m_mapSavedPos;

	struct PosArray
	{
		PosArray() { Clear(); };
		~PosArray() { Release(); };
		enum { PA_SEGBITS = 16, PA_SEGMASK = 0xffff };
		void RemoveAll() { Release(); Clear(); };
		void Release() { for (int n=0;n<SegsUsed();++n) delete[] (char*)pSegs[n]; if (pSegs) delete[] (char*)pSegs; };
		void Clear() { nSegs=0; nSize=0; pSegs=NULL; };
		int GetSize() const { return nSize; };
		int SegsUsed() const { return ((nSize-1)>>PA_SEGBITS) + 1; };
		ElemPos& operator[](int n) const { return pSegs[n>>PA_SEGBITS][n&PA_SEGMASK]; };
		ElemPos** pSegs;
		int nSize;
		int nSegs;
	};
	PosArray m_aPos;

	struct NodeStack
	{
		NodeStack() { nTop=-1; nSize=0; pN=NULL; };
		~NodeStack() { if (pN) delete [] pN; };
		NodePos& Top() { return pN[nTop]; };
		NodePos& At( int n ) { return pN[n]; };
		void Add() { ++nTop; if (nTop==nSize) Alloc(nSize*2+6); };
		void Remove() { --nTop; };
		int TopIndex() { return nTop; };
	protected:
		void Alloc( int nNewSize ) { NodePos* pNNew = new NodePos[nNewSize]; Copy(pNNew); nSize=nNewSize; };
		void Copy( NodePos* pNNew ) { for(int n=0;n<nSize;++n) pNNew[n]=pN[n]; if (pN) delete [] pN; pN=pNNew; };
		NodePos* pN;
		int nSize;
		int nTop;
	};

	void x_SetPos( int iPosParent, int iPos, int iPosChild )
	{
		m_iPosParent = iPosParent;
		m_iPos = iPos;
		m_iPosChild = iPosChild;
		m_nNodeOffset = 0;
		m_nNodeLength = 0;
		m_nNodeType = iPos?MNT_ELEMENT:0;
		MARKUP_SETDEBUGSTATE;
	};
	int x_GetFreePos()
	{
		if ( m_iPosFree == m_aPos.GetSize() )
			x_AllocPosArray();
		return m_iPosFree++;
	};
	bool x_AllocPosArray( int nNewSize = 0 );

	void InitDocFlags()
	{
		// To always ignore case, define MARKUP_IGNORECASE
	#ifdef MARKUP_IGNORECASE
		m_nFlags = MDF_IGNORECASE;
	#else
		m_nFlags = 0;
	#endif
	};

	bool x_ParseDoc();
	int x_ParseElem( int iPos, TokenPos& token );
	static bool x_FindAny( LPCTSTR szDoc, int& nChar );
	static bool x_FindName( TokenPos& token );
	static CString x_GetToken( const TokenPos& token );
	int x_FindElem( int iPosParent, int iPos, LPCTSTR szPath ) const;
	CString x_GetPath( int iPos ) const;
	CString x_GetTagName( int iPos ) const;
	CString x_GetData( int iPos ) const;
	CString x_GetAttrib( int iPos, LPCTSTR szAttrib ) const;
	static CString x_EncodeCDATASection( LPCTSTR szData );
	bool x_AddElem( LPCTSTR szName, LPCTSTR szValue, int nFlags );
	bool x_AddElem( LPCTSTR szName, int nValue, int nFlags );
	CString x_GetSubDoc( int iPos ) const;
	bool x_AddSubDoc( LPCTSTR szSubDoc, int nFlags );
	static bool x_FindAttrib( TokenPos& token, LPCTSTR szAttrib, int n=0 );
	bool x_SetAttrib( int iPos, LPCTSTR szAttrib, LPCTSTR szValue );
	bool x_SetAttrib( int iPos, LPCTSTR szAttrib, int nValue );
	bool x_AddNode( int nNodeType, LPCTSTR szText, int nFlags );
	void x_RemoveNode( int iPosParent, int& iPos, int& nNodeType, int& nNodeOffset, int& nNodeLength );
	void x_AdjustForNode( int iPosParent, int iPos, int nShift );
	static bool x_CreateNode( CString& strNode, int nNodeType, LPCTSTR szText );
	int x_InsertNew( int iPosParent, int& iPosRel, NodePos& node );
	void x_LinkElem( int iPosParent, int iPosBefore, int iPos );
	int x_UnlinkElem( int iPos );
	int x_ReleaseSubDoc( int iPos );
	int x_ReleasePos( int iPos );
	void x_CheckSavedPos();
	static int x_ParseNode( TokenPos& token, NodePos& node );
	bool x_SetData( int iPos, LPCTSTR szData, int nFlags );
	bool x_SetData( int iPos, int nValue );
	int x_RemoveElem( int iPos );
	CString x_GetElemContent( int iPos ) const;
	bool x_SetElemContent( LPCTSTR szContent );
	void x_DocChange( int nLeft, int nReplace, const CString& strInsert );
	void x_Adjust( int iPos, int nShift, bool bAfterPos = false );
};

class CXMLMakeUp : public CXMLBase
{

public:
	CXMLMakeUp(): m_byDoc(NULL),m_ilength(0) {}
	~CXMLMakeUp()
	{
		if ( NULL != m_byDoc )
		{
			delete [] m_byDoc;
			m_byDoc = NULL;
		}
	}

	virtual bool Load( LPCTSTR szFileName );
	bool         Load(byte *byData, int ilength);

	bool ReadTextFile( LPCTSTR szFileName, CString* pstrError=NULL, int* pnFlags=NULL );
	bool DecryptionString(); //��m_byDoc���м���
	bool EncryptionString(); //��m_byDoc���н���


private:
	byte*   m_byDoc;
	int		m_ilength;

};


#endif // !defined(XMLMARKUP_H)


/*
CMarkup����
 
����һ���µ�XML�ĵ��������޸��ִ��XML�ĵ������е���Щ���п��Դ�һ������MFC��STL�ַ�����ʵ��
��һ������ķ����еõ�������CMarkup�����������Zip�ļ��ұߵ����ӣ������Ը�⣬�����ͨ����
Դ�룬�����Ӱ������뵽���Լ���Ӧ���С�
����
Cmarkup������EDOM�ģ����������XML�����Ĺؼ���EDOM�ǲ���XMLԪ�ء����Ժ����ݵļ�����������
���㲻��Ҫ���ӵ�XML����ʱ�����������������DOM��SAX�ĵ���
�����ϣ��ʹ��΢���MSXML��CMarkup������CmarkupMSXML��ȫ�����ʾ��ͨ��EDOM��ʹ��MSXML������
������C++ COM�﷨��ʼ��
���ذ���������zip�ļ������ӣ��а����˲��ԶԻ��򹤳̵�Դ�롢Cmarkup������в��Ժ�ʾ�����Լ���
��ѡ�����������ҵ��;�е���ϸ˵���뿴���µײ�����Licensing�е�ע�����        
�����г���CMarkup��������
����������Ҫ�κ��ⲿ��XML�����
С�����뵽��Ĺ���ֻ�Ǻ�С��һ���࣬���Ҷ����ĵ�ֻά����һ���ַ������Լ�һ���ܼ�С���ַ�����
���С���������顣
�죺���������൱��Ľ����������顣
�򵥣�EDOM����ʹ�������������޸ģأͣ��ޱȵļ򵥡�
MSXML��CMarkupMSXML����EDOM������װ��΢���XML����MSXML����ѡ��������ʾ����ࡣ
UNICODE��ͳһ�ַ������׼��������WindowsNT��WindowsCEƽ̨�ܹ��������UNICODE����WindowsNT 
UNICODE�£�XML�ĵ�������UTF-8�ļ��У������ڲ��ÿ��ַ���
UTF-8������UTF-8�ļ�������ܺͷ���UTF-8�ַ�����ȷ��_MBCSû�б����壩
MBCS������˫�ֽ����ݣ�����_MBCS����������UTF-8����.
STL��CMarkupSTL����ȫû��ʹ��MFC��������ʾҪʹ��STL����ѡ�
 
ÿ�����ݵ�XML
 ���Ǿ�����һ���ļ�����Ҫ����򴫵���Ϣ����Ӽ������ ����һ����Ϣ�鵽������¡�����������һ
��������֣���Ҫ����������Щ���ݵĸ�ʽ������XML֮ǰ�������Ҫ���ǡ�env�����ͣ�����
PATH=C:\WIN95������ini�����ͣ�һЩ���ֵ���ϣ����������ƻ�����һЩ���ơ������ǹ̶����ַ�����
���ȡ�����XML���Ǳ�ȷ������Щ����Ĵ𰸣�����ĳЩʱ�򣬳���Ա�����٣�������ҪһЩ���ڰ�����
���͸�ʽ������������Щ�Ľ��XML�ĸ����ԡ���������޶ȶ�XML��ǩƥ������ҽ����Ķ���
CcodeProjectվ��ġ�Beginning XML - Chapter 2: Well-Formed XML��
XML��������Ϊ����������Ȼ�ĵȼ����������㷺��Ӧ�á���ȻXML���޶���ʽ���˸�����ַ��������
Ҫ�Ļ����������õ�����ѹ����������Ҫ��չ���ĵ���Ϣ����ʱ��XML������Եõ������֣�����ҪҪ��
��Ϣ��ÿ����������д�߼����̡����ܹ����־ɵ���Ϣ��ʶ�������ƶ�����ͬ�ķ�ʽ���������Ժ�Ԫ�ء� 
 
ʹ��CMarkup
Cmarkup�ǻ��ڡ�ѹ�����ĵ�����ģ��(EDOM �� "Encapsulated" Document Object Model,)�����������
��XML�������Ĺؼ�������һ��XML�������Ͳ�����Ч����DOM(Document Object Model)��һ���ġ�����
����DOM�������Ķ������ͣ���EDOMֻ������һ������XML�ĵ���EDOM�ص���XMLԭʼ�����������򵥡�
���ڷ����������ĵ����뿴EDOM Specification.
CmarkUp��װ��XML�ĵ��ı����ṹ�͵�ǰλ�á����ṩ������Ԫ�ء������͵õ�Ԫ�����Ժ����ݵ����з�
���ԡ����ĵ��в�����ִ�еĵط�ȡ���ڵ�ǰλ�ú͵�ǰ����λ�á���ǰλ������������һ��Ԫ�ص��ĵ�
�е����λ�ã�������Ҫ��ȷָ�����Ӷ����ĵ�������ϣ���CMarkup����Ч�������������һֱ��
����һ�����������ĵ����ַ������������GetDoc.���Եõ�����  
�鿴free firstobject XML editor��Դ�룬������CMarkup�������������Լ���XML�ĵ���
����һ��XML�ĵ�
���ڴ���һ��XML�ĵ�����Ҫʵ����һ��CMarkup���󣬲�����AddElem������Ԫ�ء�.�����λ�ã������
���� AddElem("ORDER") ������ĵ���򵥵�װһ����ORDERԪ��<ORDER/>. Ȼ�����AddChildElem �ڸ�
Ԫ�ص����洴��Ԫ�� (���磺�����롱��Ԫ���ڲ�����α�ʾ).�����ʾ�����봴��һ��XML�ĵ�������
���������ݣ���һ���ַ����С�
CMarkup xml;
xml.AddElem( "ORDER" );
xml.AddChildElem( "ITEM" );
xml.IntoElem();
xml.AddChildElem( "SN", "132487A-J" );
xml.AddChildElem( "NAME", "crank casing" );
xml.AddChildElem( "QTY", "1" );
CString csXML = xml.GetDoc();
��Щ��������������XML������������ORDERԪ�أ�ע�����Ŀ�ʼ��ǩ<ORDER> �ڿ�ͷ��������ǩ
</ORDER>�ڽ�β����һ��Ԫ������һ�������棨����򱻰������������Ԫ�صĿ�ʼ��ǩҪ����֮ǰ����
����ǩҪ����֮��ORDERԪ�ذ���һ��ITEMԪ�أ���ITEMԪ�ذ�������������Ԫ�أ�SN��NAME��QTY��
<ORDER>
<ITEM>
<SN>132487A-J</SN>
<NAME>crank casing</NAME>
<QTY>1</QTY>
</ITEM>
</ORDER>
������������ʾ�ģ���Ҳ�ܹ���һ����Ԫ���´�����Ԫ�أ�����Ҫ����IntoElem �ƶ���ĵ�ǰ��λ�õ�
��ǰ��Ԫ��λ�ã�Ȼ����Ϳ���������������һ����Ԫ���ˡ�CMarkup�������б�����һ����ǰλ��ָ��
���Ա�֤���Դ����Ӷ̺͸��򵥣��������ļ�ʱ����ͬ���߼�λ��Ҳ�ᱻʹ�á�
����XML�ĵ�
�����������������XML�ַ�������SetDoc�������뵽CMarkup�������ܹ�����������Ҳ������������ȷ��
���뱻������ͬһ��CMarkup�����У������Ҫ���õ�ǰλ�õ��ĵ��Ŀ�ʼʱ����Ҫ����ResetPos.
������������У���csXML�ַ�������CMarkup���������ѭ��ORDERԪ���µ�����ITEMԪ�أ����õ�ÿ
����Ŀ����ź�������
CMarkup xml;
xml.SetDoc( csXML );
while ( xml.FindChildElem("ITEM") )
{
    xml.IntoElem();
    xml.FindChildElem( "SN" );
    CString csSN = xml.GetChildData();
    xml.FindChildElem( "QTY" );
    int nQty = atoi( xml.GetChildData() );
    xml.OutOfElem();
}
�������Ƿ��ֵ�ÿ��Ԫ�أ��ڲ�ѯ������Ԫ��֮ǰҪ����IntoElem����ѯ��֮���ٵ���OutOfElem ������
ϰ�������ֵ�������ʱ���㽫֪����������ѭ��ʱ��Ҫȷ��ÿ��IntoElem ���ö���һ����֮��Ӧ��
OutOfElem ���� ��
����Ԫ�غ�����
���洴���ĵ��������н�������һ��ITEMԪ�أ�������������Ǵ��������Ŀ����ǰһ�����ݼӲú���
��������Դ������SHIPMENT��ϢԪ������һ�����ԣ���δ���Ҳ��ʾ�����ܵ��õ���IntoElem��AddElem
������AddChildElem,�������á���Ȼ����ζ�Ÿ���ĵ��ã����������Ϊ������ֱ�ۡ�
CMarkup xml;
xml.AddElem( "ORDER" );
xml.IntoElem(); // inside ORDER
for ( int nItem=0; nItem<aItems.GetSize(); ++nItem )
{
    xml.AddElem( "ITEM" );
    xml.IntoElem(); // inside ITEM
    xml.AddElem( "SN", aItems[nItem].csSN );
    xml.AddElem( "NAME", aItems[nItem].csName );
    xml.AddElem( "QTY", aItems[nItem].nQty );
    xml.OutOfElem(); // back out to ITEM level
}
xml.AddElem( "SHIPMENT" );
xml.IntoElem(); // inside SHIPMENT
xml.AddElem( "POC" );
xml.SetAttrib( "type", csPOCType );
xml.IntoElem(); // inside POC
xml.AddElem( "NAME", csPOCName );
xml.AddElem( "TEL", csPOCTel );
��δ�������������XML����Ԫ��ORDER��������ITEMԪ�غ�һ��SHIPMENTԪ�أ�ITEMԪ��ȫ������SN��
NAME����QTYԪ�أ�SHIPMENTԪ�ذ���һ�������������͵�POCԪ�أ���NAME��TEL��Ԫ�ء�
<ORDER>

<ITEM>

<SN>132487A-J</SN>

<NAME>crank casing</NAME>

<QTY>1</QTY>

</ITEM>

<ITEM>

<SN>4238764-A</SN>

<NAME>bearing</NAME>

<QTY>15</QTY>

</ITEM>

<SHIPMENT>

<POC type="non-emergency">

<NAME>John Smith</NAME>

<TEL>555-1234</TEL>

</POC>

</SHIPMENT>

</ORDER>

����Ԫ��
FindElem �� FindChildElem�������ڵ���һ���ֵ�Ԫ�ء������ѡ�ı�ǩ����ָ������ô���ǽ�����һ
�����ǩ����ƥ���Ԫ�أ������ֵ�Ԫ���ǵ�ǰԪ�أ������´ε���Find���ᵽ��ǰλ�ú����һ���ֵ�
����һ��ƥ���ֵܡ�
�����޷��ж�Ԫ�ص�����ʱ���ڵ�������Find����֮�䣬һ��Ҫ��λ��ǰλ�á��������������ITEMԪ��
������Ǳ���˴�����XML�ļ����㲻��ȷ��SNԪ����QTYԪ��֮ǰ����ô�ڲ���QTYԪ��֮ǰ��Ҫ����
ResetChildPos();
������һ���ض������ȥ����Ԫ�أ�����Ҫ��ȫѭ��ITEMԪ�أ����Ƚ�SNԪ�ص����ݺ����������������
��������Ӳ�ͬ����ǰ���������ӣ�������IntoElem ���뵽ORDERԪ�أ�������FindElem("ITEM")�滻
FindChildElem("ITEM");��ʵ���ַ�ʽ��ͦ�á���Ҫע����ǣ���Find������ָ��ITEMԪ�صı�ǩ������
�ǻ���������������ֵ�Ԫ�أ�����SHIPMENTԪ�ء�
CMarkup xml;
xml.SetDoc( csXML );
xml.FindElem(); // ORDER element is root
xml.IntoElem(); // inside ORDER
while ( xml.FindElem("ITEM") )
{
    xml.FindChildElem( "SN" );
    if ( xml.GetChildData() == csFindSN )
        break; // found
}
����
ASCII�����������������������ַ���128���µ��ַ�������Ӣ���̡������ֻʹ��ASCII�룬�ܷ��㣬
UTF-8�������������ASCII����ͬ��
�������ʹ�õ��ַ�������Unicode���뼯(UTF-8��UTF-16��UCS-2)�У���ô���ڽ������Լ���IE�кܺ�
����ʾ���������Ҫ��XML�����н�����������ISO-8859-1(��ŷ)�ַ���ָ���ַ�ֵ��һ����������128��
255֮�䡣�Ա�ÿ���ַ���Ȼʹ��һ�����ء�Windows˫�ֽ��ַ�����GB2312��Shift_JIS��EUC-KR��ÿ��
�ַ�������һ���������ֽڣ�������ЩWindows�ַ����������Ԥ��������Ҫ����_MBCS ����Ҫȷ���û�
�Ĳ���ϵͳ���õ����ʵı���ҳ��
������һ��XML������XML�ĵ�ǰ׺����<?xml version="1.0" encoding="ISO-8859-1"?>����Ҫͨ����
SetDoc��Cmarkup�Ĺ��캯�������ݡ��ڽ�βҪ�����س�����������������ʾ����һ�С�
xml.SetDoc( "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\r\n" );
xml.AddElem( "island", "Cura?ao" );
���ԶԿ�
Markup.exe��һCmarkup�Ĳ��Գ�������һ��VC6�µ�MFC���̡�������ʼʱ��������OnTest����������
��ϳ�����ѡ�����ض��ı���ѡ���²���Cmarkup.,�����һ��һ���ĵ���OnTest�������ῴ��������
Cmarkup��һЩ���ӣ���Open��Parse��������һ���ļ���
������Ĳ�ͼ���ʾ�ı���汾�ǡ�CMarkup 7.0 Unicode��������ζ��Cmarkup��������_UNICODE����
�±���ġ��ɹ���������RunTest������������Charsets_err.xml�ļ��б��������ļ������أ��ļ�ԭ��
1500����ת����1033Unicode���ַ�(����2066�ֽ�)��������������һ��������ǩ����ʼ��ǩ����Ӧ�Ĵ�
�� ��

������ԶԻ��򱣴������һ���������ļ��ͶԻ������Ļλ����Ϣ����Щ����ע����е�
HKEY_CURRENT_USER/ Software/ First Objective Software/ Markup/ Settings.���¡�

*/

//////////////////////////
/**************����Ҳ����**********************
 vc�в���Xml--ʹ��CMarkup�� 
DotNet��ʹ��xml��ԱȽ����ɣ���vc�в���xml����Щ����������������ϵ�֪CMarkup�ȽϺ��ã�����
�����£����ã��ͰѲ�������д�������Ժ���ʱ��Ļ�����д�����
//----------UserInfo.xml--------------
<?xml version="1.0" encoding="UTF-8" ?> 
 <UserInfo>
  <UserID>luo</UserID> 
  <UserID>lin</UserID> 
  </UserInfo>
1������UserInfo.xml
CMarkup xml;
xml.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
xml.AddElem("UserInfo");
xml.IntoElem();
xml.AddElem("UserID","luo");
xml.AddElem("UserID","lin");
xml.OutOfElem();
xml.Save("UserInfo.xml");
2������ض�Ԫ��
//----�鿴����UserID----
CMarkup xml;
xml.Load("UserInfo.xml");
BOOL bFind = true;
xml.ResetMainPos();
while (xml.FindChildElem("UserID");)
{
       //��ʱ�ӵ㻹�Ǹ��ӵ�
         CString strTagName = _T("");
         CString strData = _T("");
         strTagName = xml.GetChildTagName();
         strData = xml.GetChildData();
         TRACE("\n---tagName:%s,Data:%s--\n",strTagName,strData);
}
       /********************����Ҳ����******************
       CMarkup xml;
       xml.Load("UserInfo.xml");
       BOOL bFind = true;
       xml.ResetMainPos();
       while (xml.FindChildElem("UserID");)
       {
                    xml.IntoElem();
                     CString strTagName = _T("");
                     CString strData = _T("");
                     strTagName = xml.GetTagName();
                     strData = xml.GetData();
                     TRACE("\n---tagName:%s,Data:%s--\n",strTagName,strData);
                     xml.OutOfElem();

       }
       
       CMarkup xml;
       xml.Load("UserInfo.xml");
       BOOL bFind = true;
       xml.ResetMainPos();
       xml.FindElem();  //UserInfo
       xml.IntoElem();
       while (xml.FindElem("UserID");)
       {
                     CString strTagName = _T("");
                     CString strData = _T("");
                     strTagName = xml.GetTagName();
                     strData = xml.GetData();
                     TRACE("\n---tagName:%s,Data:%s--\n",strTagName,strData);
       }
       
//-------------���----------
---tagName:UserID,Data:luo--
---tagName:UserID,Data:lin��
3���޸�
//--------��UserIDΪ��luo����Ϊ��flypigluo��-----------
BOOL bLoadXml = false;
CMarkup xml;
bLoadXml = xml.Load("UserInfo.xml");
       if (bLoadXml)
       {
             CString strUserID = _T("");
              xml.ResetMainPos();
              xml.FindElem();
              xml.IntoElem();
              while (xml.FindElem("UserID"))
              {
                     strUserID = xml.GetData();
                    if (strUserID=="luo")
                     {
                           xml.SetData(CString("flypig")+strUserID);
                           xml.Save("UserInfo.xml");
                            break;
                     }
              }
       }
4�����
4��1   ��������棨ʹ�õ���AddElem��
//���������
       BOOL bLoadXml = false;
       CMarkup xml;
       bLoadXml = xml.Load("UserInfo.xml");
       if (bLoadXml)
       {
              xml.ResetMainPos();
              xml.FindElem();
              xml.IntoElem();
              xml.AddElem("UserID","luoluo");
              xml.OutOfElem();
              xml.Save("UserInfo.xml");
       }
//--------ԭ��Ϊ------
<?xml version="1.0" encoding="UTF-8" ?> 
 <UserInfo>
  <UserID>luo</UserID> 
  <UserID>lin</UserID> 
  </UserInfo>
//---------��Ӻ�Ϊ-------------
<?xml version="1.0" encoding="UTF-8" ?> 
<UserInfo>
  <UserID>luo</UserID> 
  <UserID>lin</UserID> 
  <UserID>luoluo</UserID> 
  </UserInfo>
4��2  ������ǰ�棨ʹ��InsertElem��
BOOL bLoadXml = false;
CMarkup xml;
bLoadXml = xml.Load("UserInfo.xml");
if (bLoadXml)
{
       xml.ResetMainPos();
       xml.FindElem();
       xml.IntoElem();
       xml.InsertElem("UserID","AddUserIDHead");
       xml.OutOfElem();
       xml.Save("UserInfo.xml");
}
//----ԭ��Ϊ��
<?xml version="1.0" encoding="UTF-8" ?> 
 <UserInfo>
  <UserID>luo</UserID> 
  <UserID>lin</UserID> 
  </UserInfo>
//------�޸ĺ�-----
<?xml version="1.0" encoding="UTF-8" ?> 
<UserInfo>
  <UserID>AddUserIDHead</UserID> 
  <UserID>luo</UserID> 
  <UserID>lin</UserID> 
  </UserInfo>
 5��ɾ��
CMarkup xml;
xml.Load("UserInfo.xml");
BOOL bFind = true;
xml.ResetMainPos();       
while (bFind)
{
       bFind = xml.FindChildElem("UserID");
       if (bFind)
       {
              //��ʱ�ӵ㻹�Ǹ��ӵ�
              CString strData = _T("");
              strData = xml.GetChildData();
              if (strData=="luo")
              {
                     xml.RemoveChildElem();
                     xml.Save("UserInfo.xml");
                     break;
              }
       }
}
6������
���ڶ������������
�������������û��������Щ�����������һ�������������Ե�
//UserInfo1.xml
<?xml version="1.0" encoding="UTF-8" ?> 
<UserInfo>
 <User UserID="UserID00" UserPwd="UserPwd00" UserLevel="158" /> 
  <User UserID="UserID01" UserPwd="UserPwd01" UserLevel="162" /> 
  <User UserID="UserID02" UserPwd="UserPwd02" UserLevel="165" /> 
  <User UserID="UserID03" UserPwd="UserPwd03" UserLevel="168" /> 
  <User UserID="UserID04" UserPwd="UserPwd04" UserLevel="171" /> 
  <User UserID="UserID05" UserPwd="UserPwd05" UserLevel="175" /> 
  <User UserID="UserID06" UserPwd="UserPwd06" UserLevel="178" /> 
  <User UserID="UserID07" UserPwd="UserPwd07" UserLevel="181" /> 
  <User UserID="UserID08" UserPwd="UserPwd08" UserLevel="184" /> 
  <User UserID="UserID09" UserPwd="UserPwd09" UserLevel="188" /> 
  </UserInfo>
7������
CString strID = _T("");
       CString strPwd = _T("");
       CMarkup xml;
       xml.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
       xml.AddElem("UserInfo");
       xml.IntoElem();
       for (int i=0; i<10; i++)
       {
              //����һ�����������ΪȨ��
              srand(UINT(time(NULL)+i));
              int nLevel = rand()%256;
              strID.Format("UserID%02d",i);
              strPwd.Format("UserPwd%02d",i);
              xml.AddElem("User");
              xml.AddAttrib("UserID",strID);
              xml.AddAttrib("UserPwd",strPwd);
             xml.AddAttrib("UserLevel",nLevel);      
	}
       xml.OutOfElem();
       xml.Save("UserInfo1.xml");
8������
CMarkup xml;
BOOL bLoad = false;
BOOL bFind = false;
bLoad = xml.Load("UserInfo1.xml");
       if (bLoad)
       {
              CString strID;
              CString strPwd;
              xml.ResetMainPos();
              xml.FindElem();    //UserInfo
              while (xml.FindChildElem("User"))
              {
                     strID = xml.GetChildAttrib("UserID");
                     strPwd = xml.GetChildAttrib("UserPwd");
                     TRACE("\n----id:%s,pwd:%s-------\n",strID,strPwd);
                     if (strID=="UserID00"&&strPwd=="UserPwd00")
                     {
                           bFind = true;
                            break;
                     }
              }
       }
       if (bFind)
       {
              TRACE("\n---- find------\n");
       }
      else
      {
             TRACE("\n----no find------\n");
       }
9���޸�
CMarkup xml;
       BOOL bLoad = false;
       BOOL bFind = false;
       bLoad = xml.Load("UserInfo1.xml");
       if (bLoad)
       {
             CString strID;
              CString strPwd;
             xml.ResetMainPos();
              xml.FindElem();    //UserInfo
             while (xml.FindChildElem("User"))
              {
                     strID = xml.GetChildAttrib("UserID");
                     strPwd = xml.GetChildAttrib("UserPwd");
                     TRACE("\n----id:%s,pwd:%s-------\n",strID,strPwd);
                     if (strID=="UserID00"&&strPwd=="UserPwd00")
                     {
                            bFind = true;
                            xml.SetChildAttrib("UserID",strID+CString("Modify"));
                            xml.Save("UserInfo1.xml");
                            break;
                   }
              }
       }
       if (bFind)
       {
              TRACE("\n---- find------\n");
       }
       else
       {
              TRACE("\n----no find------\n");
       }
*/
