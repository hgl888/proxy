#pragma  once 


#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include <shellapi.h>

//////////////////////////////////////////////////////////////////////////

//extern DWORD	g_dwGameID;   //��Ӧ��ϷID��
typedef LRESULT(CALLBACK *LPWNDPROC)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
typedef int (WINAPI *LPFN_SEARCHSTATCALLBACK)(DWORD dwGameID, DWORD dwAdID, WPARAM wParam, LPARAM lParam);
typedef VOID(WINAPI *LPCallBack3DFocus)(DWORD,LPVOID);


HRESULT AllocateName( LPCSTR Name, LPSTR *pNewName );
DWORD WINAPI OpenIEThreadProc(void *p);

int WINAPI SearchStatCallback(DWORD dwGameID,DWORD dwAdID, WPARAM wParam, LPARAM lParam);

//�ж��ļ��Ƿ����
bool FileExist(const char* path);			

void GetModulePath( char * szfullPathName );

BOOL MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize);
BOOL WCharToMByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize);


//�쳣���˺���
long ExceptFilter( LPEXCEPTION_POINTERS pep );

//�쳣������
void ExceptDispose( LPCTSTR pFileName, int iline );

//�쳣��־��¼����
BOOL ExceptLog(LPCTSTR pchLogFileName, LPCTSTR lpszFormat, ...);



