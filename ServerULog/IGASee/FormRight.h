#pragma once
#include "ListCtrlEx.h"
#include "VisualFx.h"

// CFormRight ������ͼ

enum DOWNFILE_STATE
{
	DOWNFILE_FAILE,
	DOWNFILE_SUCCESS,
	DOWNFILE_NOEXIT,
	DOWNFILE_CANCEL,
};

typedef struct CDownFileInfo_TYP
{
	SYSTEMTIME		m_st;					// ϵͳ��ǰʱ��
	char			m_szFileName[MAX_PATH]; //�����ļ���
	char			m_szIp[16];				// �ͻ���IP
	float			m_fDownSpeed;			//�����ٶ�
	DOWNFILE_STATE	m_enuDownFileState;		// �Ƿ�ɹ�����
} CDownFileInfo;

class CFormRight : public TVisualFormView
{
	DECLARE_DYNCREATE(CFormRight)

protected:
	CFormRight();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CFormRight();

public:
	enum { IDD = IDD_FORMRIGHT };
;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	

public:
	virtual void OnInitialUpdate();
	void UpdateList(CWnd *pWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CListCtrlEx m_listRight;

};


