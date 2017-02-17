// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "IGASSee.h"

#include "MainFrm.h"
#include "FormLeft.h"
#include "FormRight.h"
#include "FormLeft1.h"
#include "FormRight1.h"
#include "FormLeft2.h"
#include "FormRight2.h"
#include "FormLeft3.h"
#include "FormRight3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
	//	| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("δ�ܴ���������\n");
	//	return -1;      // δ�ܴ���
	//}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	// TODO: �������Ҫ��������ͣ������ɾ��������
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

	this->SetMenu(NULL);

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->SetIcon(m_hIcon, TRUE);
	this->SetIcon(m_hIcon, FALSE);
	
	this->SetWindowText(_T("IGA����鿴����"));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���
	TVisualObject *pTab = new TVisualObject(1,"",pContext,RUNTIME_CLASS(TTabWnd), TVisualObject::TOS_TABTOP);
	TVisualObject *pTabViewSplite = new TVisualObject(2,_T("��Ϸ��Ϣ"),1,2,pContext);
	TVisualObject *pTabViewSplite1 = new TVisualObject(3,_T("��ע��"),1,2,pContext);
	TVisualObject *pTabViewSplite2 = new TVisualObject(4,_T("Ӳ����Ϣ�鿴"),1,2,pContext);
	TVisualObject *pTabViewSplite3 = new TVisualObject(5,_T("SDK��Ϣ�鿴"),1,2,pContext);

	TVisualObject *pPaneLeft = new TVisualObject(21,0,0,pContext,RUNTIME_CLASS(CFormLeft),CSize(300,0));
	TVisualObject *pPaneRight = new TVisualObject(22,0,1,pContext,RUNTIME_CLASS(CFormRight), CSize(0,0));

	TVisualObject *pPaneLeft1 = new TVisualObject(23,0,0,pContext,RUNTIME_CLASS(CFormLeft1),CSize(300,0));
	TVisualObject *pPaneRight1 = new TVisualObject(24,0,1,pContext,RUNTIME_CLASS(CFormRight1), CSize(0,0));

	TVisualObject *pPaneLeft2 = new TVisualObject(25,0,0,pContext,RUNTIME_CLASS(CFormLeft2),CSize(300,0));
	TVisualObject *pPaneRight2 = new TVisualObject(26,0,1,pContext,RUNTIME_CLASS(CFormRight2), CSize(0,0));

	TVisualObject *pPaneLeft3 = new TVisualObject(27,0,0,pContext,RUNTIME_CLASS(CFormLeft3),CSize(300,0));
	TVisualObject *pPaneRight3 = new TVisualObject(28,0,1,pContext,RUNTIME_CLASS(CFormRight3), CSize(0,0));

	m_Framework.Add(pTab);

	m_Framework.Add(pTab, pTabViewSplite);
	m_Framework.Add(pTabViewSplite, pPaneLeft);
	m_Framework.Add(pTabViewSplite, pPaneRight);

	m_Framework.Add(pTab, pTabViewSplite1);
	m_Framework.Add(pTabViewSplite1, pPaneLeft1);
	m_Framework.Add(pTabViewSplite1, pPaneRight1);

	m_Framework.Add(pTab, pTabViewSplite2);
	m_Framework.Add(pTabViewSplite2, pPaneLeft2);
	m_Framework.Add(pTabViewSplite2, pPaneRight2);

	m_Framework.Add(pTab, pTabViewSplite3);
	m_Framework.Add(pTabViewSplite3, pPaneLeft3);
	m_Framework.Add(pTabViewSplite3, pPaneRight3);


	m_Framework.Create(this);

	// ����INI�ļ�
	CreateConfigFile();

	return TRUE;

	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	m_Framework.Destroy();
	
}

// ������������ʱ��Ϊÿ������ָ����Ӧ��ID�� iOffset��ָ�����ڵ�ƫ��ID
CWnd * CMainFrame::GetOffsetObject(CWnd * pWnd, int iOffset)
{
	if ((pWnd == NULL) || !::IsWindow(pWnd->GetSafeHwnd()))
	{
		return NULL;
	}
	
	DWORD dwId = m_Framework.GetObject(pWnd);

	dwId += iOffset;
	if ((dwId < 1) || (dwId > 100))
	{
		return NULL;
	}

	CWnd * pResultWnd = m_Framework.GetObject(dwId);
	
	return pResultWnd;
}
