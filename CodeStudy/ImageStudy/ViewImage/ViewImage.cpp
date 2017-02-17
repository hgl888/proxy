// ViewImage.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ViewImage.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "ViewImageDoc.h"
#include "ViewImageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewImageApp

BEGIN_MESSAGE_MAP(CViewImageApp, CWinApp)
	//{{AFX_MSG_MAP(CViewImageApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewImageApp construction

CViewImageApp::CViewImageApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CViewImageApp object

CViewImageApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CViewImageApp initialization

BOOL CViewImageApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_VIEWDITYPE,
		RUNTIME_CLASS(CViewImageDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewImageView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CViewImageApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CViewImageApp message handlers


void CViewImageApp::OnFileOpen() 
{
	CString strOpenFilter = "ͼ���ļ�|*.bmp; *.dib; *.gif; *.jpg; *.jpe; *.jpeg; *.pcx; *.tga; *.tif; *.tiff|λͼ�ļ� (*.bmp;*.dib)|*.bmp; *.dib|GIF�ļ� (*.gif)|*.gif|JPEG�ļ� (*.jpg;*.jpe;*.jpeg)|*.jpg; *.jpe; *.jpeg|PCX�ļ� (*.pcx)|*.pcx|TGA�ļ� (*.tga)|*.tga|TIFF�ļ� (*.tif;*.tiff)|*.tif; *.tiff|All Files (*.*)|*.*||";
	CFileDialog FileDlg(TRUE, "*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strOpenFilter);

	if (FileDlg.DoModal() == IDOK)
		OpenDocumentFile(FileDlg.m_ofn.lpstrFile);
}

void CViewImageApp::AddFile(HDIB hDIB, LPCTSTR lpszTitle) 
{
	OnFileNew();

	CMainFrame *pMainWnd = (CMainFrame *)m_pMainWnd;
	CChildFrame *pChildWnd = (CChildFrame *)pMainWnd->MDIGetActive();
	CViewImageView *pView = (CViewImageView *)pChildWnd->GetActiveView();
	CViewImageDoc *pDoc = (CViewImageDoc *)pChildWnd->GetActiveDocument();

	if (hDIB != NULL)
	{
		pDoc->m_pDib->Attach(hDIB);
		CSize sizeTotal(pDoc->m_pDib->GetWidth(), pDoc->m_pDib->GetHeight());
		pView->SetScrollSizes(MM_TEXT, sizeTotal);
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(lpszTitle);
		pDoc->UpdateAllViews(NULL);
	}
}
