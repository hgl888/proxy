// FormLeft.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IGASSee.h"
#include "FormLeft.h"
#include "FormRight.h"
#include "MainFrm.h"
#include "PathDialog.h"
#include ".\formleft.h"


// CFormLeft

IMPLEMENT_DYNCREATE(CFormLeft, TVisualFormView)

CFormLeft::CFormLeft()
	: TVisualFormView(CFormLeft::IDD)
{
}

CFormLeft::~CFormLeft()
{
}

void CFormLeft::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_POSITION, m_combPosition);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START, m_DTCtrlStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END, m_DTCtrlEnd);
}

BEGIN_MESSAGE_MAP(CFormLeft, TVisualFormView)
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_SEE, OnBnClickedBtnSee)
END_MESSAGE_MAP()


// CFormLeft ���

#ifdef _DEBUG
void CFormLeft::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormLeft::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CFormLeft ��Ϣ�������

void CFormLeft::OnBnClickedBtnBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strPath;
	CString strCaption(_T("�ļ�λ��"));
	CString strTitle(_T("·��:"));

	m_combPosition.GetWindowText(strPath);

	CPathDialog dlg(strCaption, strTitle, strPath);
	if(dlg.DoModal()==IDOK)
	{
		m_combPosition.SetWindowText(dlg.GetPathName());
	}
	
}

void CFormLeft::OnBnClickedBtnSee()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_combPosition.GetWindowText(m_strPath);
	m_DTCtrlStart.GetTime(m_tmStart);
	m_DTCtrlEnd.GetTime(m_tmEnd);
   // zzhenfei��� �ж�ʱ��
	if ( m_tmEnd < m_tmStart )
	{
		AfxMessageBox("��ȷ���Ƿ���д��ȷ�Ĳ�ѯʱ�䣡");
		return ;
	}
	if (m_strPath.IsEmpty()) return;

	int nIndex = 0;
	BOOL bIsInserted = FALSE;
	if ((nIndex= m_combPosition.FindString(nIndex, m_strPath)) != CB_ERR)
	{
		m_combPosition.InsertString(0, m_strPath);
		m_combPosition.SetCurSel(0);
		bIsInserted = TRUE;
		m_combPosition.DeleteString( ++nIndex );
	}
	else
	{
		int nCount = m_combPosition.GetCount();
		if (nCount >= MAX_PATH_KEY)
		{
			m_combPosition.DeleteString(MAX_PATH_KEY - 1);

		}
	}

	if (!bIsInserted)
	{
		m_combPosition.InsertString(0, m_strPath);
	}

	SavePath();


	CMainFrame *pMainWnd = (CMainFrame *)(::AfxGetApp()->GetMainWnd());
	ASSERT(pMainWnd != NULL);
	if (pMainWnd == NULL) return;

	CFormRight * pFormRight = (CFormRight *)(pMainWnd->GetOffsetObject(this, 1));
	if ((pFormRight != NULL) && ::IsWindow(pFormRight->GetSafeHwnd()))
	{
		pFormRight->UpdateList(this);
	}

}

void CFormLeft::OnInitialUpdate()
{
	TVisualFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	m_config.SetFileName(GetConfigFileName());
	m_config.SetSection(_T("FORM_LEFT0"));
	UpdatePath();

}

void CFormLeft::SavePath()
{
	int nCount = m_combPosition.GetCount();

	CString strKey = _T("");
	CString strValue = _T("");
	for(int i=0; i<nCount; i++)
	{
		m_combPosition.GetLBText(i, strValue);
		strKey.Format(_T("path_%02d"), i);
		m_config.SetValue(strKey, strValue);
	}

}

void CFormLeft::UpdatePath()
{
	m_combPosition.ResetContent();


	CString strKey = _T("");
	CString strValue = _T("");
	for(int i=0; i<MAX_PATH_KEY; i++)
	{
		strKey.Format(_T("path_%02d"), i);
		m_config.GetValue(strKey, strValue);
		if (strValue.IsEmpty())
		{
			break;
		}

		m_combPosition.InsertString(i, strValue);
	}

	m_combPosition.SetCurSel(0);
}
