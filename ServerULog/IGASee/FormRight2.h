#pragma once
#include "ListCtrlEx.h"



// CFormRight2 ������ͼ

class CFormRight2 : public CFormView
{
	DECLARE_DYNCREATE(CFormRight2)

protected:
	CFormRight2();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CFormRight2();

public:
	enum { IDD = IDD_FORMRIGHT2 };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	CListCtrlEx m_listRight2;
	void UpdateList(CWnd * pWnd);
};


