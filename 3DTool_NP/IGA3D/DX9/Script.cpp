#include "stdafx.h"
CScript* GetScript()
{
	static CScript scr;
	return &scr;
}

//--------------------------------------------
CScript::CScript()
{
	m_bPyInit = false;
	m_pFunc = NULL;
	m_pModule = NULL;
}

CScript::~CScript()
{
	Destory();
}

bool CScript::Init()
{
	m_bPyInit = true;

	Py_Initialize();
	
	m_pModule = PyImport_ImportModule("t1");
	if( m_bPyInit && m_pModule == NULL )
	{
		m_bPyInit = false;
		OutputDebugString("Python: û���ҵ�ģ�� t1 ");
	}

	//�ҵ��ű�
	if( m_bPyInit )
	{
		//���ؽű�����
		m_pFunc = PyObject_GetAttrString( m_pModule, "fun1");
		if( m_pFunc == NULL )
		{
			m_bPyInit = false;
			OutputDebugString("Python: û����ģ�� t1 ���ҵ� fun1");
		}
	
	}
	return true;
}

int CScript::Run()
{
	if( m_bPyInit )
	{
		PyObject* pRetVal = NULL;
		pRetVal = PyEval_CallObject( m_pFunc, NULL );
		
		//��������ֵ
		int    iRetVal = 0;
		PyArg_Parse(pRetVal, "i", &iRetVal );

		return iRetVal;
		//char text[256];
		//sprintf(text, "Python: return %d", iRetVal );
		//OutputDebugString(text);
	}
}

void CScript::Destory()
{
	if( m_bPyInit )
	{
		Py_Finalize();
	}
}