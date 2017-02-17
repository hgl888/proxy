// FormRight.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IGASSee.h"
#include "FormRight.h"
#include "MainFrm.h"
#include "FormLeft.h"
#include ".\formright.h"


// CFormRight

IMPLEMENT_DYNCREATE(CFormRight, TVisualFormView)

CFormRight::CFormRight()
	: TVisualFormView(CFormRight::IDD)
{
}

CFormRight::~CFormRight()
{
}

void CFormRight::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RIGHT, m_listRight);
}

BEGIN_MESSAGE_MAP(CFormRight, TVisualFormView)
	ON_WM_SIZE()
	
END_MESSAGE_MAP()


// CFormRight ���

#ifdef _DEBUG
void CFormRight::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormRight::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CFormRight ��Ϣ�������

void CFormRight::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���

	m_listRight.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP| LVS_EX_GRIDLINES );
	int nColCount = 0;

	m_listRight.InsertColumn(nColCount++, _T("˳���"), LVCFMT_LEFT, 60);
	m_listRight.InsertColumn(nColCount++, _T("���"), LVCFMT_LEFT, 60);
	m_listRight.InsertColumn(nColCount++, _T("ʱ��"), LVCFMT_LEFT, 100);
	m_listRight.InsertColumn(nColCount++, _T("��������"), LVCFMT_LEFT, 100);
	m_listRight.InsertColumn(nColCount++, _T("�ͻ���IP"), LVCFMT_LEFT, 160);
	m_listRight.InsertColumn(nColCount++, _T("�Ƿ�ɹ�����"), LVCFMT_LEFT, 50);
	m_listRight.InsertColumn(nColCount++, _T("�����ļ�"), LVCFMT_LEFT, 200);
	//m_listRight.InsertColumn(nColCount++, _T("MAC��ַ"), LVCFMT_LEFT, 100);
	//m_listRight.InsertColumn(nColCount++, _T("IP��ַ"), LVCFMT_LEFT, 100);
	//m_listRight.InsertColumn(nColCount++, _T("����"), LVCFMT_LEFT, 100);

	CRect clientRect;
	this->GetClientRect(&clientRect);
	if (::IsWindow(m_listRight.GetSafeHwnd()))
	{
		m_listRight.MoveWindow(&clientRect);
	}

}

void CFormRight::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: �ڴ˴������Ϣ����������
	if (::IsWindow(m_listRight.GetSafeHwnd()))
	{
		m_listRight.MoveWindow(0, 0, cx, cy);
	}
}

void CFormRight::UpdateList(CWnd *pWnd)
{
	ASSERT(pWnd != NULL);
	if (pWnd == NULL) return;

	m_listRight.DeleteAllItems();

	CString strPath = _T("");
	CTime timeStart, timeEnd;

	CFormLeft * pFormLeft = (CFormLeft *)pWnd;
	if ((pFormLeft != NULL) && ::IsWindow(pFormLeft->GetSafeHwnd()))
	{
		 strPath = pFormLeft->m_strPath;
		 timeStart = pFormLeft->m_tmStart;
		 timeEnd = pFormLeft->m_tmEnd;
	}

	AUTOAPPENDBRK(strPath);
	CTimeSpan ts(1, 0, 0, 0);
	int nRow = 0;

	do 
	{
		CDownFileInfo	downInfo;
		int len = sizeof(CDownFileInfo);
		memset(&downInfo, 0, len);
		FILE *pFile = NULL;
		CString strFilePath = _T("");
		CString strFileName = timeStart.Format(_T("%Y-%m-%d"));
		strFilePath.Format(_T("%sserverU%s%s"), strPath, strFileName, DAT_EXT);

		CString strTemp = _T("");
		int nCol = 1;
		if ((pFile = fopen((LPCTSTR)strFilePath,  "rb")) != NULL)
		{
			while (fread(&downInfo, 1, len, pFile) == len)
			{
				strTemp.Format(_T("%d"), nRow + 1);
				int nItemInserted = m_listRight.InsertItem(LVIF_TEXT, nRow, strTemp, 0, 0, 0, 0);

				m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);

				//m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);
				CTime timeEnd(downInfo.m_st);
				//CString strTemp = timeEnd.Format(_T("%Y-%m-%d %H:%M:%S"));
				m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")), 0, 0, 0, 0);
				//m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")), 0, 0, 0, 0);
				//m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, ts.Format(_T("%D %H:%M:%S")), 0, 0, 0, 0);
				
				strTemp.Format(_T("%f"), downInfo.m_fDownSpeed);
				m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);
				//strTemp.Format()
				m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, downInfo.m_szIp, 0, 0, 0, 0);
				strTemp.Format(_T("%d"), downInfo.m_enuDownFileState);
				m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);
				strTemp.Format(_T("%s"), downInfo.m_szFileName);
				m_listRight.SetItem(nItemInserted, nCol++, LVIF_TEXT, strTemp, 0, 0, 0, 0);

				nRow++;
				nCol = 1;
				memset(&downInfo, 0, len);
			}

			fclose( pFile );
		}
		timeStart += ts;
		
	}while (timeStart < timeEnd);

	m_listRight.SetOneselfItemData();

}

