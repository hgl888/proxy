#pragma once
#include "ListCtrlEx.h"



// CFormRight1 ������ͼ

class CFormRight1 : public CFormView
{
	DECLARE_DYNCREATE(CFormRight1)

protected:
	CFormRight1();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CFormRight1();

public:
	enum { IDD = IDD_FORMRIGHT1 };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrlEx m_listRight1;
	virtual void OnInitialUpdate();
	void UpdateList(CWnd * pWnd);
};


