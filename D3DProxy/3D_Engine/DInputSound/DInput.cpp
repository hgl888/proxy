///////////////////////////////////////
//DInput.cpp
///////////////////////////////////////

#include "DInput.h"



CDInput * g_pDInput;


//////////////////////////////////////////////////////////////////////////////
//��ʼ���������


bool CDInput::Init(HINSTANCE hHinstance)
{
	if (FAILED(DirectInput8Create(hHinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pDInput,NULL)))
		return false;

	return true;
}

///////////////////////////////////////////////////////////
//�ͷ��������


void CDInput::Release()
{
	if (m_pDInput)
		m_pDInput->Release();
}


///////////////////////////////////////////////////////////
//��ʼ�����������豸


bool CDInput::InitKeyboard(HWND hWND)
{
	if (m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIKey, NULL) != DI_OK)
		return false;

	if (m_pDIKey->SetCooperativeLevel(hWND,DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK)
		return false;

	if (m_pDIKey->SetDataFormat(&c_dfDIKeyboard) != DI_OK)
		return false;

	if (m_pDIKey->Acquire() != DI_OK)
		return false;

	return true;
}


///////////////////////////////////////////////////////////
//��ȡ��������


bool CDInput::ReadKeyboard()
{

	if (m_pDIKey)
	{
		if (m_pDIKey->GetDeviceState(256, (void *)m_bKeyboard) != DI_OK)
			return false;
	}
	else
	{
		memset(m_bKeyboard,0,sizeof(m_bKeyboard));
		return false;
	} 

	return false;
}


///////////////////////////////////////////////////////////

void CDInput::ReleaseKeyboard()
{
	if (m_pDIKey)
	{
		m_pDIKey->Unacquire();
		m_pDIKey->Release();
	} 
}
