#pragma once
#include "ListCtrlEx.h"



// CFormRight3 ������ͼ

class CFormRight3 : public CFormView
{
	DECLARE_DYNCREATE(CFormRight3)

protected:
	CFormRight3();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CFormRight3();

public:
	enum { IDD = IDD_FORMRIGHT3 };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlEx m_listRight3;
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void UpdateList(CWnd * pWnd);
};


