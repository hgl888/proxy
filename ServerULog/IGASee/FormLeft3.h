#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "Config.h"



// CFormLeft3 ������ͼ

class CFormLeft3 : public CFormView
{
	DECLARE_DYNCREATE(CFormLeft3)

protected:
	CFormLeft3();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CFormLeft3();

public:
	enum { IDD = IDD_FORMLEFT3 };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	CComboBox m_combPosition3;
	CDateTimeCtrl m_DTCtrlStart3;
	CDateTimeCtrl m_DTCtrlEnd3;
	CINIConfig m_config;

public:
	CString m_strPath;
	CTime m_tmStart;
	CTime m_tmEnd;

	afx_msg void OnBnClickedBtnBrowse3();
	afx_msg void OnBnClickedBtnSee3();
	virtual void OnInitialUpdate();
	void SavePath();
	void UpdatePath();
};


