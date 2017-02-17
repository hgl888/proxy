// 3D_TOOLDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "3D_TOOL.h"
#include "3D_TOOLDlg.h"
#include ".\3d_tooldlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//ȫ�ֵ����Ի����ָ��
CD_TOOLDlg* g_pMainDlg = NULL;

extern IGA_DATA_MEM g_IGA_Date;

/*
	���¿ͻ���״̬
*/
void UpdateClient()
{
	g_IGA_Date[0].bIsReload = true;
	g_IGA_Date.Update();
}

void GetAbsPath( char fullPathName[MAX_PATH] )
{
	//OutputDebugString(fullPathName);
	char  TmpPath[MAX_PATH];
	GetModuleFileName(NULL,TmpPath,MAX_PATH);
	char *p;
	p = strrchr(TmpPath,'\\');
	if(!p)
		return ;
	*(p+1) = 0;

    char path[MAX_PATH];	
	sprintf( path,"%s%s", TmpPath, fullPathName );
	strcpy(fullPathName,path);
	
}

//�����༶Ŀ¼
BOOL mkdirEx(CString Path)
{
    CString pathname = Path;
    if(pathname.Right(1) != "\\")
	{
		pathname += "\\" ;
	}
	
    int end = pathname.ReverseFind('\\');
    int pt = pathname.Find('\\');
    if ( pt-1 >= 0 && pathname[pt-1] == ':')
	{
		pt = pathname.Find('\\', pt+1);
	}

    CString path;
    while(pt != -1 && pt<=end)
    {
        path = pathname.Left(pt+1);
        if(_access(path, 0) == -1)
		{
			if( -1 == _mkdir(path))
			{
				return false;
			}
		}
        pt = pathname.Find('\\', pt+1);
    }
    return true;
}



int SFileCopy::nCount = 500;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CD_TOOLDlg �Ի���



CD_TOOLDlg::CD_TOOLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CD_TOOLDlg::IDD, pParent)
	, m_GameName(_T(""))
	, m_ResourcePath(_T(""))
	, m_nCurPickID(0)
	, m_bShowPickMesh(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CD_TOOLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_GAME_NAME, m_GameName);
	DDX_Text(pDX, IDC_STATIC_RES_PATH, m_ResourcePath);
	DDX_Control(pDX, IDC_LIST_3DINFO, m_3DInfoList);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_ButtonAdd);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_ButtonDelete);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_ButtonClear);
	DDX_Control(pDX, IDC_BUTTON_OUTPUT, m_ButtonOutput);
	DDX_Control(pDX, IDC_BUTTON_SET, m_ButtonSet);
	DDX_Control(pDX, IDC_BUTTON_QUIT, m_ButtonQuit);
	DDX_Text(pDX, IDC_STATIC_TEXT_VLUE, m_nCurPickID);
	DDX_Check(pDX, IDC_CHECK1, m_bShowPickMesh);
}

BEGIN_MESSAGE_MAP(CD_TOOLDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SET, OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, OnBnClickedButtonQuit)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_3DINFO, OnNMRclickList3dinfo)
	ON_NOTIFY(NM_CLICK, IDC_LIST_3DINFO, OnNMClickList3dinfo)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, OnBnClickedButtonOutput)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CD_TOOLDlg ��Ϣ�������

BOOL CD_TOOLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	bool	_bInit = true;


	UpdateData();
	m_GameName	= "δ֪����Ϸ";
	m_ResourcePath = "";
	m_ResourceOutputPath = "";

	CRegKey		reg;
	LPCTSTR lp="Software\\3D_TOOL";
	reg.Create( HKEY_CURRENT_USER, lp,"");

	if( reg.Open(HKEY_CURRENT_USER,lp) == ERROR_SUCCESS )
	{
		//��ȡֵ
		DWORD nCount = 1024;
		char*  buf = new char[1024];
		if( reg.QueryValue( buf,"ResourceWorkPath", &nCount ) == ERROR_SUCCESS )
		{
			m_ResourcePath = buf;
		}
		else
		{
			reg.SetValue((LPCSTR)m_ResourcePath,"ResourceWorkPath");
		}

		if( reg.QueryValue( buf,"ResourceOutputPath", &nCount ) == ERROR_SUCCESS )
		{
			m_ResourceOutputPath = buf;
		}
		else
		{
			reg.SetValue((LPCSTR)m_ResourceOutputPath,"ResourceOutputPath");
		}


		delete buf;
	
	}
	else
	{
		AfxMessageBox("ע����ʧ�ܣ�");
	}
	reg.Close();
	/*
		�����б���
	*/
	m_3DInfoList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_3DInfoList.InsertColumn(0,_T("����ID"),LVCFMT_IMAGE|LVCFMT_LEFT);
	m_3DInfoList.InsertColumn(1,_T("ģ������"));
	m_3DInfoList.InsertColumn(2,_T("URL"));
	m_3DInfoList.SetColumnWidth(0 ,50);
	m_3DInfoList.SetColumnWidth(1 ,100);
	m_3DInfoList.SetColumnWidth(2 ,300);

	m_nDataIndex = 0;

	m_CurDataNode = m_DataMap.end();
	m_nSelectIndex = -1;

    /*
		���ر�����Ϣ��emmoa.ini��data.ini��Adinfo.xml
	*/
	m_MeshCopyMap.clear();

	/*
		����emmoa.ini,���û���ҵ�����ļ�������߽���������
	*/
	char NameINI[MAX_PATH];
	strcpy( NameINI, "emmoa.ini" );
	GetAbsPath(NameINI);
	GetIni()->Attach(NameINI);
	int  game_id = 0;
	GetIni()->GetInt("common","GameID",game_id,0 );
	if( game_id == IGA_SHENQI )
	{
		m_GameName = "����";
	}
	else if( game_id == IGA_WUJIEQU )	
	{
		m_GameName = "�������������";
	}
	else if( game_id == IGA_WUJIEQU_MLXY )
	{
		m_GameName = "��������������ǣ�";	
	}
	else if( game_id == IGA_WUJIEQU_XJP )
	{
		m_GameName = "��������¼��£�";	
	}
	else if( game_id == IGA_WUJIEQU_FLB )
	{
		m_GameName = "����������ɱ���";			
	}
	else if( game_id == IGA_WLOL )
	{
		m_GameName = "����Ⱥ����";			
	}
	else if( game_id == IGA_GV )
	{
		m_GameName = "�󺽺�ʱ��";			
	}
	else if( game_id == IGA_SUN )
	{
		m_GameName = "�漣����";			
	}
	else if( game_id == IGA_RELIPAIQIU )
	{
		m_GameName = "��������";			
	}
	else if( game_id == IGA_PAOPAOKADINGCHE )
	{
		m_GameName = "���ܿ�����";			
	}
	else if( game_id == IGA_TIANZUN )
	{
		m_GameName = "����";			
	}
	else if( game_id == IGA_CHAOJIWUZHE )
	{
		m_GameName = "��������";
	}
	else if(game_id == IGA_FENGYICHUANSHUO)
	{
		m_GameName = "��ӡ��˵";
	}
	else if ( game_id == IGA_SHUSHAN )
	{
		m_GameName = "��ɽ";
	}
	else if ( game_id == IGA_TEST )
	{
		m_GameName = "������Ϸ";
	}
	else
	{
		m_ButtonSet.EnableWindow(0);
		m_3DInfoList.EnableWindow(0);
		_bInit = false;
	}


	//...
	if( _bInit && !m_ResourcePath.IsEmpty() && !m_ResourceOutputPath.IsEmpty() )
	{
		SetEnable(true);
		/*
			Ѱ����Դ������䵽��ӶԻ�����
		*/
		int num = m_AddModifictionDlg.FindResource(m_ResourcePath);
	    
		/*
			������Դ���Ŀ¼,��·���ᱻ����
		*/
		BOOL bCreatePath = mkdirEx(m_ResourceOutputPath);
		if( bCreatePath == TRUE )
		{
			UpdateData(FALSE);
			SetEnable(true);
		}
	}
	else
	{
		SetEnable(false);
	}






	UpdateData(FALSE);


	SetTimer(500,500,NULL);

	g_pMainDlg = this;
	
	return FALSE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CD_TOOLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CD_TOOLDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CD_TOOLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CD_TOOLDlg::OnBnClickedButtonSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_SetDlg.SetPath(m_ResourcePath,m_ResourceOutputPath);
	INT_PTR Key = m_SetDlg.DoModal();
	
	m_ResourcePath = m_SetDlg.GetResourcePath();
	m_ResourceOutputPath = m_SetDlg.GetResourceOutputPath();
	if( m_ResourcePath.IsEmpty() || m_ResourceOutputPath.IsEmpty() )
	{
		AfxMessageBox("����·��δ������ɣ�");
		return;
	}

	if( Key == IDOK )
	{
		//��ӵ�ע���
		CRegKey		reg;
		LPCTSTR lp="Software\\3D_TOOL";
		if( reg.Open(HKEY_CURRENT_USER,lp) == ERROR_SUCCESS )
		{
			//��ȡֵ
			reg.SetValue((LPCSTR)m_ResourcePath,"ResourceWorkPath");
			reg.SetValue((LPCSTR)m_ResourceOutputPath,"ResourceOutputPath");
		}
		else
		{
			AfxMessageBox("ע����ʧ�ܣ�");
		}
		reg.Close();

		/*
			Ѱ����Դ������䵽��ӶԻ�����
		*/
		int num = m_AddModifictionDlg.FindResource(m_ResourcePath);
	    
		/*
			������Դ���Ŀ¼,��·���ᱻ����
		*/
		BOOL bCreatePath = mkdirEx(m_ResourceOutputPath);
		if( bCreatePath == TRUE )
		{
			UpdateData(FALSE);
			SetEnable(true);
		}
	}

}

void CD_TOOLDlg::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_AddModifictionDlg.SetCurData(S3DDATA());

	INT_PTR Key = m_AddModifictionDlg.DoModal();
	
	if( Key == IDOK )
	{
		S3DDATA	data = m_AddModifictionDlg.Get3DData();
		//����ǲ����Ѿ���ӹ�������
		int key = data.obj_id;
		LPNODE p = m_DataMap.find(key);
		if( p == m_DataMap.end() )
		{
			data.obj_id  = ++m_nDataIndex;
			int nCount = m_3DInfoList.GetItemCount();
			CString str(_T(""));
			str.Format( "%d", data.obj_id );
			m_3DInfoList.InsertItem( nCount,"" );
			m_3DInfoList.SetItemText( nCount, 0, str );
			str.Format( "%s", data.mesh_name );
			m_3DInfoList.SetItemText( nCount, 1, str);
			str.Format( "%s", data.url );
			m_3DInfoList.SetItemText( nCount, 2, str);

			/*
				��ӵ������б���		
			*/
			m_DataMap.insert( DATA_NODE(data.obj_id,data) );
			m_nSelectIndex = nCount;

		}
		else
		{
			CString str(_T(""));
			str.Format( "%d", m_CurDataNode->second.obj_id );
			m_3DInfoList.SetItemText( m_nSelectIndex, 0, str );
			str.Format( "%s", m_CurDataNode->second.mesh_name );
			m_3DInfoList.SetItemText( m_nSelectIndex, 1, str);
			str.Format( "%s", m_CurDataNode->second.url );
			m_3DInfoList.SetItemText( m_nSelectIndex, 2, str);
		}

		UpdateData(FALSE);

		/*
			����Ҫ�Ļ������ӵ���Դ�ļ����Ƶ�InterAdtive\EMMOA\3d��
			���������������ļ�
		*/
		UpdateDataINI();
		UpdataAdInfoXML();

		//֪ͨ�ͻ��˸���
      	UpdateClient();	
	}

}

void CD_TOOLDlg::SetEnable(bool bIsEnable )
{
	if( bIsEnable )
	{
		m_ButtonAdd.EnableWindow();
		m_ButtonDelete.EnableWindow();
		m_ButtonClear.EnableWindow();
		m_ButtonOutput.EnableWindow();
	}
	else
	{
		m_ButtonAdd.EnableWindow(0);
		m_ButtonDelete.EnableWindow(0);
		m_ButtonClear.EnableWindow(0);
		m_ButtonOutput.EnableWindow(0);
	}
}

void CD_TOOLDlg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(500);
	Delete3DRes();
	CDialog::OnOK();
}

void CD_TOOLDlg::OnNMRclickList3dinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*
		Ѱ�ҵ���ѡ������ݽڵ㣬���� ���\�޸ĶԻ�������޸�
	*/
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	m_nSelectIndex = pNMLV->iItem;
	if( m_nSelectIndex > -1 )
	{  
		CString mesh_name = m_3DInfoList.GetItemText( m_nSelectIndex, 0 );
		int _Index = _ttoi(mesh_name);
		m_CurDataNode = m_DataMap.find(_Index);

		m_AddModifictionDlg.SetCurData(m_CurDataNode->second);

		INT_PTR key = m_AddModifictionDlg.DoModal();
		if( key == IDOK )
		{
			m_CurDataNode->second = m_AddModifictionDlg.Get3DData();

			CString str(_T(""));
			str.Format( "%d", m_CurDataNode->second.obj_id );
			m_3DInfoList.SetItemText( m_nSelectIndex, 0, str );
			str.Format( "%s", m_CurDataNode->second.mesh_name );
			m_3DInfoList.SetItemText( m_nSelectIndex, 1, str);
			str.Format( "%s", m_CurDataNode->second.url );
			m_3DInfoList.SetItemText( m_nSelectIndex, 2, str);
		}
	}
	UpdateData(FALSE);

 	/*
		����Ҫ�Ļ������ӵ���Դ�ļ����Ƶ�InterAdtive\EMMOA\3d��
		���������������ļ�
	*/
	UpdateDataINI();
	UpdataAdInfoXML();

	//֪ͨ�ͻ��˸���
    UpdateClient();	

	*pResult = 0;
}

void CD_TOOLDlg::OnNMClickList3dinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*
		Ѱ�ҵ���ѡ������ݽڵ�
	*/
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	m_nSelectIndex = pNMLV->iItem;
	if( m_nSelectIndex > -1 )
	{  
		CString mesh_name = m_3DInfoList.GetItemText( m_nSelectIndex, 0 );
		int _Index = _ttoi(mesh_name);
		m_CurDataNode = m_DataMap.find(_Index);

	}

	*pResult = 0;
}

void CD_TOOLDlg::OnBnClickedButtonDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( m_nSelectIndex == -1 )
	{
		AfxMessageBox("û�����ݱ�ѡ��");
		return;
	}

	INT_PTR key = AfxMessageBox("ȷʵҪ�Ƴ�����������?", MB_OKCANCEL ); 
	if( key == IDOK )
	{
		m_3DInfoList.DeleteItem(m_nSelectIndex);
		if( m_CurDataNode != m_DataMap.end() )
		{
			m_DataMap.erase(m_CurDataNode);
		}
		m_CurDataNode = m_DataMap.end();
		m_nSelectIndex = -1;
	}

	/*
		����Ҫ�Ļ������ӵ���Դ�ļ����Ƶ�InterAdtive\EMMOA\3d��
		���������������ļ�
	*/
	UpdateDataINI();
	UpdataAdInfoXML();

	//֪ͨ�ͻ��˸���
    UpdateClient();	

}

void CD_TOOLDlg::OnBnClickedButtonClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( m_DataMap.empty() )
	{
		AfxMessageBox("�б���û����Ϣ��");
		return;
	}

	INT_PTR key = AfxMessageBox("ȷʵҪɾ��������Ϣ��", MB_OKCANCEL );
	if( key == IDCANCEL )
	{
		m_CurDataNode = m_DataMap.end();
		m_nSelectIndex = -1;
		return;
	}

	m_DataMap.clear();
	m_CurDataNode = m_DataMap.end();

	m_3DInfoList.DeleteAllItems();
	m_nSelectIndex = -1;

	m_AddModifictionDlg.SetCurData(S3DDATA());

	/*
		����Ҫ�Ļ������ӵ���Դ�ļ����Ƶ�InterAdtive\EMMOA\3d��
		���������������ļ�
	*/
	UpdateDataINI();
	UpdataAdInfoXML();

	//֪ͨ�ͻ��˸���
    UpdateClient();	
}

void CD_TOOLDlg::OnBnClickedButtonOutput()
{
	//��ʹ�õ���Դ���Ƶ�ָ��Ŀ¼��
	for( LPCOPYNODE pNode = m_MeshCopyMap.begin(); pNode != m_MeshCopyMap.end(); pNode++ )
	{
		SFileCopy&  fileCopy = pNode->second;
		/*
			�������Ŀ¼����ģ���ļ���������ÿ��ģ�͵�������Դ�ŵ�����ļ����
		*/
		CString distPath = m_ResourceOutputPath + CString("\\");
		CString SourceMesnName = pNode->first;
		if( !(SourceMesnName.Right(2) == ".x" || SourceMesnName.Right(2) == ".X"))
		{
			continue;
		}

		char _name[MAX_PATH];
		int num = SourceMesnName.GetLength()-2;
		char* _p = SourceMesnName.GetBuffer();
		memcpy( _name, _p, num );
		_name[num] = '\0';
		distPath += _name;

		BOOL bIsCreatPath = mkdirEx(distPath);
		if( bIsCreatPath == FALSE )
		{
			continue;
		}

		distPath += "\\";
		
		//����ģ���ļ�����
		CString SoutcePath = m_AddModifictionDlg.GetMeshPath(SourceMesnName);
		CString nameSource = SoutcePath + SourceMesnName;
		CString nameDist = distPath + SourceMesnName;
		BOOL succeed = CopyFile( nameSource, nameDist, true );
		if( succeed == TRUE )
		{
			//����ʹ�õ�����
			CStringList& TexList = fileCopy.NameList;
			POSITION   pos = TexList.GetHeadPosition();
			while( pos != NULL )
			{
				CString str =  TexList.GetNext(pos);
				CString texSource = SoutcePath + str; 
				CString texDist   = distPath + str;
				CopyFile( texSource, texDist, true );
			}
		}
	}

	//--------------------------------------
	/*
		�����õ���Ϣ���浽�ı��ļ�	
	*/
	//CFileDialog fileDlg( 0, "*.txt", m_GameName + ".txt", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "*.txt" );

	//INT_PTR key = fileDlg.DoModal();
	//if( key == IDCANCEL )
	//{
	//	return;
	//}

	//CString name = fileDlg.GetPathName();

	//char* pName = name.GetBuffer();

	char text[MAX_PATH];
	strcpy( text, m_ResourceOutputPath.GetBuffer() );
	m_ResourceOutputPath.ReleaseBuffer();
	
	CString pathName = m_ResourceOutputPath + "\\" + m_GameName + ".txt";
	char* pName = pathName.GetBuffer();
	ofstream out;
	out.open(pName);
	if( out.is_open() )
	{
		int num = (int)m_DataMap.size();
		int i = 1;
		out<<"һ�������"<<num<<"������,�������£�"<<endl;
		for( LPNODE p = m_DataMap.begin(); p != m_DataMap.end(); ++p )
		{
			S3DDATA& data = p->second;
			
			out<<"------------ "<<i<<" -----------"<<endl;
			out<<"ģ�����ƣ�"<<data.mesh_name<<endl;
			
			out<<"���ӣ�"<<endl;
			out<<"	url: "<<data.url<<endl;

			out<<"λ�ã�"<<endl;
			out<<"	x: "<<data.x<<endl;
			out<<"	y: "<<data.y<<endl;
			out<<"	z: "<<data.z<<endl;

			out<<"�ȱ����ţ�"<<endl;
			out<<"	s_xyz: "<<data.scale_xyz<<endl;

			out<<"��ת��"<<endl;
			out<<"	Rot_x: "<<data.rot_x/3.14*180<<endl;
			out<<"	Rot_y: "<<data.rot_y/3.14*180<<endl;
			out<<"	Rot_z: "<<data.rot_z/3.14*180<<endl;
			
			i++;
		}
		out.close();		
	}
	pathName.ReleaseBuffer();
	
	INT_PTR Key = AfxMessageBox("�Ѿ���3D��Ϣ��ʹ�õ���Դ���浽���Ŀ¼��\n\n��Ҫ�����Ŀ¼��", MB_OKCANCEL );
	if( Key == IDOK )
	{
		ShellExecute( this->GetSafeHwnd(), "open", m_ResourceOutputPath, NULL, NULL, SW_SHOW );
	}
}

void CD_TOOLDlg::UpdataCurList(S3DDATA& data)
{
	//����ǲ����Ѿ���ӹ�������
	int key = data.obj_id;
	LPNODE p = m_DataMap.find(key);
	if( p == m_DataMap.end() )
	{
		INT_PTR _key = AfxMessageBox("��Ҫ�������������", MB_OKCANCEL );
		if( _key == IDCANCEL )
		{
			return;
		}
		else if( _key == IDOK )
		{
			data.obj_id  = ++m_nDataIndex;
			int nCount = m_3DInfoList.GetItemCount();
			CString str(_T(""));
			str.Format( "%d", data.obj_id );
			m_3DInfoList.InsertItem( nCount,"" );
			m_3DInfoList.SetItemText( nCount, 0, str );
			str.Format( "%s", data.mesh_name );
			m_3DInfoList.SetItemText( nCount, 1, str);
			str.Format( "%s", data.url );
			m_3DInfoList.SetItemText( nCount, 2, str);

			/*
				��ӵ������б���		
			*/
			m_DataMap.insert( DATA_NODE(data.obj_id,data) );

			m_CurDataNode = m_DataMap.find(data.obj_id);
			m_nSelectIndex = nCount;
		}
	}
	else	//��Ҫ�޸ĵ�����
	{
		m_CurDataNode->second = data;
	}
    
	/*
		����Ҫ�Ļ������ӵ���Դ�ļ����Ƶ�InterAdtive\EMMOA\3d��
		����Ӧ����һ�������ƣ������ظ��ļ��Ļ���
	*/
	//�������ݵ��ļ�
	UpdateDataINI();
	UpdataAdInfoXML();



	/*
		�����б��
	*/
	if( m_CurDataNode != m_DataMap.end() )
	{
		CString str(_T(""));
		str.Format( "%d", m_CurDataNode->second.obj_id );
		m_3DInfoList.SetItemText( m_nSelectIndex, 0, str );
		str.Format( "%s", m_CurDataNode->second.mesh_name );
		m_3DInfoList.SetItemText( m_nSelectIndex, 1, str);
		str.Format( "%s", m_CurDataNode->second.url );
		m_3DInfoList.SetItemText( m_nSelectIndex, 2, str);
	}

	UpdateData(FALSE);

	//g_IGA_Date[0].bIsReload = true;
	//g_IGA_Date.Update();


	UpdateClient();
	//g_IGA_Date[0].bIsReload = true;

}

void CD_TOOLDlg::UpdateDataINI()
{
	char WorkFullPath[MAX_PATH] = {0};
	GetAbsPath(WorkFullPath);
	CString WorkResPath = WorkFullPath;
	WorkResPath  += "InterAdtive\\EMMOA\\3d\\";

	//�������·�������û�еĻ�
	BOOL bIsCreate = mkdirEx(WorkResPath);
	if( bIsCreate == FALSE )
	{
		return;
	}

	/*
		��ģ���ļ����Ƶ�InterAdtive\EMMOA\3d�£������ݶ���ID����(�����ظ�ʹ�õ�ģ�ͻḴ�ƶ��)
	*/
	//����ǰ�б���ʹ�õ���Դ���ƽ���ͳ��
	for( LPNODE p = m_DataMap.begin(); p != m_DataMap.end(); p++ )
	{
		CString name = p->second.mesh_name;

		LPCOPYNODE pNode = m_MeshCopyMap.find(name);
		if( pNode == m_MeshCopyMap.end() )
		{
			SFileCopy  fileCopy;
			fileCopy.mesh_id = SFileCopy::nCount;
			p->second.mesh_id = SFileCopy::nCount;
			SFileCopy::nCount++;
			fileCopy.MeshName.Format("m_%d.x",fileCopy.mesh_id); 
			GetMeshResNameList( name, &fileCopy.NameList );
			m_MeshCopyMap.insert(COPY_NODE(name,fileCopy));
		}
		else
		{
			p->second.mesh_id = pNode->second.mesh_id;
		}
	}

	//-------------------------------------------------------
	//��ʹ�õ���Դ���Ƶ�ָ��Ŀ¼��
	for( LPCOPYNODE pNode = m_MeshCopyMap.begin(); pNode != m_MeshCopyMap.end(); pNode++ )
	{
		SFileCopy&  fileCopy = pNode->second;
		if( !fileCopy.bIsCopy )
		{
			//����ģ���ļ�����
			CString SourceMesnName = pNode->first;
			CString SoutcePath = m_AddModifictionDlg.GetMeshPath(SourceMesnName);
			CString nameSource = SoutcePath + SourceMesnName;
			CString nameDist = WorkResPath + fileCopy.MeshName;
			BOOL succeed = CopyFile( nameSource, nameDist, true );
			if( succeed == TRUE )
			{
				//����ʹ�õ�����
				CStringList& TexList = fileCopy.NameList;
				POSITION   pos = TexList.GetHeadPosition();
				while( pos != NULL )
				{
					CString str =  TexList.GetNext(pos);
					CString texSource = SoutcePath + str; 
					CString texDist   = WorkResPath + str;
					CopyFile( texSource, texDist, true );
				}
			}

			//���ƹ����ļ����ϱ��
			fileCopy.bIsCopy = true;
		}
	}

	//-----------------------------------------------------
	/*
		����ĵ�data.ini
	*/

	setlocale(LC_ALL,"Chinese-simplified");

	char NameINI[MAX_PATH];
	CString csrtINI = WorkResPath + "data.ini";
	strcpy( NameINI, csrtINI.GetBuffer() );
	csrtINI.ReleaseBuffer();

	ofstream out;
	out.open( NameINI );
	if( out.is_open() )
	{
		out<<"[comm]"<<endl;
		out<<"gameid = "<<4<<"	;"<<m_GameName<<endl;;
		out<<"list = ";

		for( LPNODE p = m_DataMap.begin(); p != m_DataMap.end(); p++ )
		{
			out<<p->second.obj_id<<",";
		}
		out<<endl<<endl;

		for( LPNODE p = m_DataMap.begin(); p != m_DataMap.end(); p++ )
		{
			S3DDATA& data = p->second; 
			out<<"["<<data.obj_id<<"]"<<endl;
			out<<"mesh_id = "<<data.mesh_id<<endl;
			out<<"x = "<<data.x<<endl;
			out<<"y = "<<data.y<<endl;
			out<<"z = "<<data.z<<endl;
			out<<"s_xyz = "<<data.scale_xyz<<endl;
			out<<"r_x = "<<data.rot_x<<endl;
			out<<"r_y = "<<data.rot_y<<endl;
			out<<"r_z = "<<data.rot_z<<endl;
			out<<"url = "<<data.url<<endl<<endl;	
		}

		out.close();	
	}

}

void CD_TOOLDlg::UpdataAdInfoXML()
{



}

void CD_TOOLDlg::GetMeshResNameList(CString name, CStringList* pCStringList)
{
	try
	{
		CString cstrResoutcePath = m_AddModifictionDlg.GetMeshPath(name);
		char path[MAX_PATH];
		strcpy( path, cstrResoutcePath.GetBuffer() );
		cstrResoutcePath.ReleaseBuffer();
		strcat( path, "*.*");
		_finddata_t file;
		long lf;
		if((lf = (long)_findfirst(path,&file))==-1l)//����ļ�·��
			/*cout<<"�ļ�û���ҵ�!\n"*/;
		else
		{
			do
			{
			// cout<<file.name;
				if(file.attrib == _A_ARCH)
				{
					/*cout<<"  ��ͨ�ļ�  "*/;
					CString _name = file.name;

					if( _name == name )
					{
						continue;
					}
					pCStringList->AddHead(_name);                
				}
			}
			while( _findnext( lf, &file ) == 0 );
		}
		_findclose(lf);
	}
	catch(...)
	{
		//OutputdebugString("�����ļ�ʱ�쳣");
	}
}

void CD_TOOLDlg::Delete3DRes( bool bIsAll, CString name )
{
	char WorkFullPath[MAX_PATH] = {0};
	GetAbsPath(WorkFullPath);
	CString WorkResPath = WorkFullPath;
	WorkResPath  += "InterAdtive\\EMMOA\\3d\\";

	if( bIsAll )
	{
		CString dataINI = WorkResPath + "data.ini";
		BOOL bDelDataINI = DeleteFile(dataINI);

		for( LPCOPYNODE pNode = m_MeshCopyMap.begin(); pNode != m_MeshCopyMap.end(); pNode++ )
		{
			SFileCopy&  fileCopy = pNode->second;
		
			CString MeshName = WorkResPath + fileCopy.MeshName;
			BOOL bIsDel = DeleteFile( MeshName );

			CStringList& TexList = fileCopy.NameList;
			POSITION   pos = TexList.GetHeadPosition();
			while( pos != NULL )
			{
				CString str =  TexList.GetNext(pos);
				CString texDist   = WorkResPath + str;
				BOOL bDelTex = DeleteFile( texDist );
			}
		}
	}
	else
	{
		
	}
	m_DataMap.clear();
	m_MeshCopyMap.clear();

	UpdateClient();
}

void CD_TOOLDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UpdateData();
	g_IGA_Date[0].bIsShowPickMesh = m_bShowPickMesh;
	m_nCurPickID = g_IGA_Date[0].nPickObjID;
	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}

void CD_TOOLDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//AfxMessageBox("qsdqweqw");


	OnBnClickedButtonQuit();
}
