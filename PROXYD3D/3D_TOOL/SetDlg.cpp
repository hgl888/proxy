// SetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "3D_TOOL.h"
#include "SetDlg.h"
#include ".\setdlg.h"


// CSetDlg �Ի���

IMPLEMENT_DYNCREATE(CSetDlg, CDHtmlDialog)

CSetDlg::CSetDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CSetDlg::IDD, CSetDlg::IDH, pParent)
	, m_cstrResourcePath(_T(""))
	, m_cstrResourceOutputPath(_T(""))
{
}

CSetDlg::~CSetDlg()
{
}

void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SET_RES_PATH, m_cstrResourcePath);
	DDX_Text(pDX, IDC_EDIT_SET_RES_OUT_PATH, m_cstrResourceOutputPath);
	DDX_Control(pDX, IDC_EDIT_SET_RES_OUT_PATH, m_EditOutputPath);
}

BOOL CSetDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	//m_cstrResourcePath = "";
	//m_cstrResourceOutputPath = "";

	UpdateData(FALSE);



	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(CSetDlg, CDHtmlDialog)
	ON_BN_CLICKED(ID_SET_OK, OnBnClickedSetOk)
	ON_BN_CLICKED(ID_SET_CANCEL, OnBnClickedSetCancel)
	ON_BN_CLICKED(IDC_BUTTON_SET_BROWSE, OnBnClickedButtonSetBrowse)
	ON_BN_CLICKED(IDC_BUTTON_SET_RES_OUT_BROWSE, OnBnClickedButtonSetResOutBrowse)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CSetDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CSetDlg ��Ϣ�������

HRESULT CSetDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	CDialog::OnOK();
	return S_OK;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

HRESULT CSetDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	CDialog::OnCancel();
	return S_OK;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

CString CSetDlg::GetResourcePath(void)
{
	return m_cstrResourcePath;
}

CString CSetDlg::GetResourceOutputPath(void)
{
	return m_cstrResourceOutputPath;
}

void CSetDlg::OnBnClickedSetOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	OnOK();
}

void CSetDlg::OnBnClickedSetCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CSetDlg::SelectPath(char* path)
{
	BROWSEINFO bi; 
	ITEMIDLIST *pidl; 

	bi.hwndOwner = NULL; 
	bi.pidlRoot = NULL; 
	bi.pszDisplayName = path; 
	bi.lpszTitle = "Select a directory"; 
	bi.ulFlags = BIF_RETURNONLYFSDIRS; 
	bi.lpfn = NULL; 
	bi.lParam = 0; 
	bi.iImage = 0; 

	pidl = SHBrowseForFolder( &bi );     /* Display "Select Folder" dialog box, Get the  
                                           folder name and convert it into a ITEMLIST  
                                           data structure. */ 
	if( pidl == NULL )
	{
		path[0] = 0; 
		return;
	}

	if(!SHGetPathFromIDList( pidl, path ))       /* Retrieve folder name from ITEMLIST structure. */ 
	{
		path[0] = 0; 
		return;
	}
}

void CSetDlg::OnBnClickedButtonSetBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char Dir[MAX_PATH];
	SelectPath(Dir);
	if( Dir[0] == 0 )
	{
		return;
	}
	m_EditOutputPath.EnableWindow();
	m_cstrResourcePath = Dir;
	UpdateData(FALSE);
}

void CSetDlg::OnBnClickedButtonSetResOutBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char Dir[MAX_PATH];
	SelectPath(Dir);
	if( Dir[0] == 0 )
	{
		return;
	}
	m_cstrResourceOutputPath = Dir;
	UpdateData(FALSE);

}

void CSetDlg::SetPath(CString ResourcePath, CString outputPath)
{
	m_cstrResourcePath = ResourcePath;
	m_cstrResourceOutputPath = outputPath;
}
