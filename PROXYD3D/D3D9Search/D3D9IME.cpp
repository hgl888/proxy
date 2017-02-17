
#include "dxstdafx.h"
#include "D3D9IME.h"
#include <strsafe.h>

CD3D9IME::CD3D9IME(void)
{
	m_lpCandList = NULL;
//	DisableIme();
	m_bIme = true;
	m_wImeName[ 0 ] = 0;
	m_wCompStr[ 0 ] = 0;
	m_wCandList[ 0 ] = 0;
	m_wImeStr[0] = 0;
}

CD3D9IME::~CD3D9IME(void)
{
	DisableIme();
	if( m_lpCandList )
	{
		GlobalFree( (HANDLE)m_lpCandList );
		m_lpCandList = NULL;
	}
}

bool CD3D9IME::GetImeState()
{
	return m_bIme;
}

void CD3D9IME::DisableIme()
{
	while( ImmIsIME( GetKeyboardLayout( 0 )))
		ActivateKeyboardLayout(( HKL )HKL_NEXT, 0 );//���ime��ͨ��ѭ���л�����һ���ر�
	m_bIme = false;
	m_wImeName[ 0 ] = 0;
	m_wCompStr[ 0 ] = 0;
	m_wCandList[ 0 ] = 0;
	m_wImeStr[0] = 0;
}

void CD3D9IME::EnableIme()
{
	m_bIme = true;
}

void CD3D9IME::NextIme()
{
	if( m_bIme )
	{
		ActivateKeyboardLayout(( HKL )HKL_NEXT, 0 );
	}
}

void CD3D9IME::SharpIme( HWND hWnd )
{
	ImmSimulateHotKey( hWnd, IME_CHOTKEY_SHAPE_TOGGLE );
}

void CD3D9IME::SymbolIme( HWND hWnd )
{
	ImmSimulateHotKey( hWnd, IME_CHOTKEY_SYMBOL_TOGGLE );
}

//ת��CandidateList��һ�������ո�ָ�ÿһ��
void CD3D9IME::ConvertCandList()
{
	CANDIDATELIST *pCandList = m_lpCandList;
	WCHAR *pszCandList = m_wCandList;

	if( pCandList->dwCount < pCandList->dwSelection )
	{
		pszCandList[ 0 ] = 0;
		return;
	}
		//��ѡ����ų���������΢��ƴ���ڶ��ε�ѡ�ֱ����һҳ���ٰ�PageDown�����������������һ��˳�ѡ��״̬����ʼһ���µ�����
		//��΢��ƴ���Լ���ime���ڿ��Խ��������⣬����΢��ƴ��ʵ���˸���Ľӿڣ�����ʹ�������ֲ�̫��׼������
		//�������޷����������⣬����ʵ��ʹ����Ҳ�������������£�����������׼���뷨������������bug
		//�Ǳ�׼���뷨����ʵ�ֵĽӿڱȽ��٣�����Ӧ��Ҳ������������bug
	unsigned int i;
	char * a;
	WCHAR * b;
	for( i = 0; ( i < pCandList->dwCount - pCandList->dwSelection )&&( i < pCandList->dwPageSize ); i++ )
	{
		*pszCandList++ = ( i % 10 != 9 )? i % 10 + L'1' : L'0';//ÿ���Ӧ�����ּ�
		*pszCandList++ = L'.';//��'.'�ָ�
		//  ����ƫ���ҵ��ַ����ĵ�ַ
		a = (char*)pCandList + pCandList->dwOffset[ pCandList->dwSelection + i ];
		//����ָ��ת���ɿ���
		b = (WCHAR*)a;
		wcscpy( pszCandList, b );//ÿ��ʵ�ʵ�����
		pszCandList += wcslen( pszCandList );
		*pszCandList++ = L' ';//��֮����' '�ָ�
	}
	*( pszCandList - 1 )= 0;//��β�����������һ��' '
}

//ʹ��Ctrl+Space��Ctrl+Shift�л����뷨ʱ���������Ϣ��
//������ǽ�ֹ��ime���򷵻�false����ʱ���ں���Ӧ����0����ֹDefWindowProc�����뷨
bool CD3D9IME::OnWM_INPUTLANGCHANGEREQUEST()
{
	return !m_bIme;
}

bool CD3D9IME::OnWM_INPUTLANGCHANGE( HWND hWnd )
{
	//ime�ı�
	HKL hKL = GetKeyboardLayout( 0 );
	if( ImmIsIME( hKL )){
		HIMC hIMC = ImmGetContext( hWnd );
		ImmEscape( hKL, hIMC, IME_ESC_IME_NAME, m_wImeName );//ȡ�������뷨����
		DWORD dwConversion, dwSentence;
		ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence );
		m_bImeSharp = ( dwConversion & IME_CMODE_FULLSHAPE )? true : false;//ȡ��ȫ�Ǳ�־
		m_bImeSymbol = ( dwConversion & IME_CMODE_SYMBOL )? true : false;//ȡ�����ı���־
		ImmReleaseContext( hWnd, hIMC );
	}
	else//Ӣ������
		wcscpy( m_wImeName, L"Ӣ��" );

	ConvertImeStr();
	return false;//���Ƿ���false����Ϊ��Ҫ���ں�������DefWindowProc��������
}

bool CD3D9IME::OnWM_IME_NOTIFY( HWND hWnd, WPARAM wParam )
{
	HIMC hIMC;
	DWORD dwSize;
	DWORD dwConversion, dwSentence;
	switch( wParam ){
		case IMN_SETCONVERSIONMODE://ȫ��/��ǣ���/Ӣ�ı��ı�
			hIMC = ImmGetContext( hWnd );
			ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence );
			m_bImeSharp = ( dwConversion & IME_CMODE_FULLSHAPE )? true : false;
			m_bImeSymbol = ( dwConversion & IME_CMODE_SYMBOL )? true : false;
			ImmReleaseContext( hWnd, hIMC );
			break;
		case IMN_OPENCANDIDATE://����ѡ��״̬
		case IMN_CHANGECANDIDATE://ѡ�ֱ�ҳ
			hIMC = ImmGetContext( hWnd );
			if( m_lpCandList )
			{
				//�ͷ���ǰ��ѡ�ֱ�
				GlobalFree( (HANDLE)m_lpCandList );
				m_lpCandList = NULL;
			}
			if( dwSize = ImmGetCandidateList( hIMC, 0, NULL, 0 ))//�õ��µ�ѡ�ֱ�
			{
				m_lpCandList = (LPCANDIDATELIST)GlobalAlloc( GPTR, dwSize );
				if( m_lpCandList )
					ImmGetCandidateList( hIMC, 0, m_lpCandList, dwSize );
			}
			ImmReleaseContext( hWnd, hIMC );
			if( m_lpCandList )ConvertCandList();//ѡ�ֱ�����ɴ�
			break;
		case IMN_CLOSECANDIDATE://�ر�ѡ�ֱ�
			if( m_lpCandList )//�ͷ�
			{
				GlobalFree( (HANDLE)m_lpCandList );
				m_lpCandList = NULL;
			}
			m_wCandList[ 0 ] = 0;
			break;
		default:
			return true;
	}
	ConvertImeStr();
	return true;//���Ƿ���true����ֹime���ڴ�
}

//����ı�
bool CD3D9IME::OnWM_IME_COMPOSITION( HWND hWnd, LPARAM lParam, TCHAR * pStr )
{
	HIMC hIMC;
	DWORD dwSize;
	hIMC = ImmGetContext( hWnd );
	// ȡ���û�����ı���
	if( lParam & GCS_COMPSTR )
	{
		dwSize = ImmGetCompositionString( hIMC, GCS_COMPSTR, (void*)m_wCompStr, sizeof( m_wCompStr ));
		// �ú������صĳ���Ϊ�ֽ��������ֻ���Ҫ����2
		m_wCompStr[ dwSize / 2 ] = 0;
	}
	// �������������ִ�
	if( lParam & GCS_RESULTSTR ){
		WCHAR str[ MAX_PATH ];
		dwSize = ImmGetCompositionString( hIMC, GCS_RESULTSTR, (void*)str, sizeof( str ));//ȡ�ú������봮
		str[ dwSize / 2 ] = 0;
		// ����������ּ��뵽�ִ���
		if( pStr )
		{
			StringCchPrintf( pStr, MAX_PATH, L"%s%s", pStr, str );
		}
	}
	ImmReleaseContext( hWnd, hIMC );
	ConvertImeStr();
	return true;//���Ƿ���true����ֹime���ڴ�
}

bool CD3D9IME::IfImeSharp()
{//�Ƿ�ȫ��
	return m_bImeSharp;
}

bool CD3D9IME::IfImeSymbol()
{//�Ƿ����ı��
	return m_bImeSymbol;
}

void CD3D9IME::ConvertImeStr()
{
	StringCchPrintf( m_wImeStr, MAX_PATH, L"[%s - %s] %s", m_wImeName, m_wCompStr, m_wCandList );
}

WCHAR * CD3D9IME::GetImeStr()
{
	return m_wImeStr;
}

//����΢��ƴ��ʵ���˺ܶ��Լ��Ķ�����CIme�����ļ�������Щ����
//1���ں���ConvertCandList����˵��ѡ�ֱ������
//2������GetImeInput���ص�szCompReadStr��Ȼ�����˼ӹ�����������ļ�������
//   ����ÿ������ϵ�����ռ�Կո����8byte�����µĲ�����ϵ������Ϊ0xa1
//   ���ǿ��������뷨���������Ӵ�"΢��ƴ��"ʱ��ֻ��ʾĩβ��һ��8byte�������0xa1��ʲô������ʾ��Ҳ����ֱ����TextOut��ʾ���е�
