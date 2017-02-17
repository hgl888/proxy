#include "python.h"

class CScript
{
public:
	CScript();
	~CScript();

public:
	bool Init();
	int Run();
	void Destory();
    
private:
	bool		m_bPyInit;		//�ű�

	PyObject*	m_pModule;
	PyObject*	m_pFunc;

};

CScript* GetScript();