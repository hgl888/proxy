#pragma once

#pragma comment ( lib, "imm32.lib" )
#include <windows.h>
#include <imm.h>

class CD3D9IME
{
public:
	CD3D9IME(void);
public:
	~CD3D9IME(void);

	//���ƺ���
	void DisableIme();//�رղ���ֹ���뷨����ime�Ѿ�����رգ��˺���Ҳ������ȼ�����ime
	void EnableIme();//�������뷨���˺���ҿ������ȼ�����ime
	void NextIme();//�л�����һ�����뷨������EnableIme�����Ч
	void SharpIme( HWND hWnd );//�л�ȫ��/���
	void SymbolIme( HWND hWnd );//�л���/Ӣ�ı��

	//״̬����
	bool	GetImeState();
	WCHAR*	GetImeStr();	// �������������뷨������ѡ���б�
	bool	IfImeSharp();	//�Ƿ�ȫ��
	bool	IfImeSymbol();	//�Ƿ����ı��

	//��������Ϣ�е��õĺ��������������true���򴰿ں���Ӧֱ�ӷ���0������Ӧ���ݸ�DefWindowProc
	bool OnWM_INPUTLANGCHANGEREQUEST();
	bool OnWM_INPUTLANGCHANGE( HWND hWnd );
	bool OnWM_IME_NOTIFY( HWND hWnd, WPARAM wParam );
	bool OnWM_IME_COMPOSITION( HWND hWnd, LPARAM lParam, TCHAR * pStr );

private:

	bool	m_bIme;						//ime�����־
	WCHAR	m_wCompStr[ MAX_PATH ];		//�洢ת����Ĵ�
	WCHAR	m_wCandList[ MAX_PATH ];	//�洢������ַ���ѡ�ֱ�
	WCHAR	m_wImeStr[ MAX_PATH ];		// ���������뷨�ִ�
	int		m_nImeCursor;			//�洢ת����Ĵ��еĹ��λ��
	CANDIDATELIST *m_lpCandList;	//�洢��׼��ѡ�ֱ�
	WCHAR	m_wImeName[ 64 ];		//�洢���뷨������
	bool	m_bImeSharp;			//ȫ�Ǳ�־
	bool	m_bImeSymbol;			//���ı���־

	void	ConvertCandList();//��ѡ�ֱ�����ɴ�
	void	ConvertImeStr();		// �����������뷨�ִ�

};
