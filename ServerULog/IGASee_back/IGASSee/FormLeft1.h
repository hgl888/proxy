#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "Config.h"



// CFormLeft1 ������ͼ

class CFormLeft1 : public CFormView
{
	DECLARE_DYNCREATE(CFormLeft1)

protected:
	CFormLeft1();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CFormLeft1();

public:
	enum { IDD = IDD_FORMLEFT1 };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	
protected:
	afx_msg void OnBnClickedBtnBrowse1();
	afx_msg void OnBnClickedBtnSee1();

	CComboBox m_combPosition1;
	CDateTimeCtrl m_DTCtrlStart1;
	CDateTimeCtrl m_DTCtrlEnd1;
	CConfig m_config;

public:
	CString m_strPath;
	CTime m_tmStart;
	CTime m_tmEnd;

	
	virtual void OnInitialUpdate();
	void SavePath();
	void UpdatePath();
};


