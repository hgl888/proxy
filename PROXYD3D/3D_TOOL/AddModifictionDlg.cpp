// AddModifictionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "3D_TOOL.h"
#include "AddModifictionDlg.h"
#include ".\addmodifictiondlg.h"

/*
	���Ի����ָ�룬�����޸�ĳ�����ݡ�
	��Ҫ����ԡ����¡���ť�ģ�����֪ͨ�����ڸ��������б�ͬʱ�����������ļ�����Դ��
*/
#include "3D_TOOLDlg.h"
extern CD_TOOLDlg* g_pMainDlg;
extern IGA_DATA_MEM g_IGA_Date;
static float _scale = 1.0f;
static float _rot   = 1.57f;
static float _pos   = 0.5f;
// AddModifictionDlg �Ի���

IMPLEMENT_DYNAMIC(AddModifictionDlg, CDialog)
AddModifictionDlg::AddModifictionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AddModifictionDlg::IDD, pParent)
	, m_MeshName(_T(""))
	, m_URL(_T(""))
	, m_fPositionX(0)
	, m_fPositionY(0)
	, m_fPositionZ(0)
	, m_fRotationX(0)
	, m_fRotationY(0)
	, m_fRotationZ(0)
	, m_fScaling(0)
	, m_fPositionOffset(0.5f)
	, m_fRotationOffset(1.57f)
	, m_fScalingOffset(0.5f)
{
}

AddModifictionDlg::~AddModifictionDlg()
{
}

void AddModifictionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_ADD_MESH_NAME, m_MeshName);
	DDX_Text(pDX, IDC_EDIT_ADD_URL, m_URL);
	DDX_Control(pDX, IDC_XVIEWCTRL_ADD, m_XView);
	DDX_Control(pDX, IDC_LIST_ADD, m_MeshList);
	DDX_Text(pDX, IDC_STATIC_ADD_POSITION_X, m_fPositionX);
	DDX_Text(pDX, IDC_STATIC_ADD_POSITION_Y, m_fPositionY);
	DDX_Text(pDX, IDC_STATIC_ADD_POSITION_Z, m_fPositionZ);
	DDX_Text(pDX, IDC_STATIC_ADD_ROTATION_X, m_fRotationX);
	DDX_Text(pDX, IDC_STATIC_ADD_ROTATION_Y, m_fRotationY);
	DDX_Text(pDX, IDC_STATIC_ADD_ROTATION_Z, m_fRotationZ);
	DDX_Text(pDX, IDC_STATIC_ADD_SCALING, m_fScaling);
	DDX_Text(pDX, IDC_EDIT_ADD_POSITION_OFFSET, m_fPositionOffset);
	DDX_Text(pDX, IDC_EDIT_ADD_ROTATION_OFFSET, m_fRotationOffset);
	DDX_Text(pDX, IDC_EDIT_SCALING_OFFSET, m_fScalingOffset);
}


BEGIN_MESSAGE_MAP(AddModifictionDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ADD, OnNMClickListAdd)
	ON_BN_CLICKED(ID_ADD_OK, OnBnClickedAddOk)
	ON_BN_CLICKED(ID_ADD_CANCEL, OnBnClickedAddCancel)
	ON_EN_CHANGE(IDC_EDIT_ADD_URL, OnEnChangeEditAddUrl)
	ON_BN_CLICKED(ID_ADD_UPDATA, OnBnClickedAddUpdata)
	ON_BN_CLICKED(IDC_BUTTON_ADD_GETPOSITION, OnBnClickedButtonAddGetposition)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ADD_POSITION_X, OnDeltaposSpinAddPositionX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ADD_POSITION_Y, OnDeltaposSpinAddPositionY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_POSITION_Z, OnDeltaposSpinPositionZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ROTATION_X, OnDeltaposSpinRotationX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINROTATION_Y, OnDeltaposSpinrotationY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ADD_ROTATION_Z, OnDeltaposSpinAddRotationZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ADD_SCALING, OnDeltaposSpinAddScaling)
	ON_BN_CLICKED(IDC_BUTTON_ADD_OPEN_URL, OnBnClickedButtonAddOpenUrl)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// AddModifictionDlg ��Ϣ�������

BOOL AddModifictionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//UpdateData();
	m_MeshName	= "δѡ��";
	m_URL		= "";

	/*
		�����б���
	*/
	m_MeshList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_MeshList.InsertColumn(0,_T("ģ������"),LVCFMT_IMAGE|LVCFMT_LEFT);
	m_MeshList.SetColumnWidth(0 ,200);

	/*
		����ģ�͵��б��
	*/
	int _nIndex = 0;
	for( LPNODE p = m_MeshFullPathName.begin(); p != m_MeshFullPathName.end(); p++ )
	{
		m_MeshList.InsertItem( _nIndex, p->first );
		_nIndex++;
	}

	/*
		��ʼ����ǰ����
	*/
	m_fPositionX	= m_MeshInfo.x;
	m_fPositionY	= m_MeshInfo.y;
	m_fPositionZ	= m_MeshInfo.z;
	m_fRotationX	= m_MeshInfo.rot_x;
	m_fRotationY	= m_MeshInfo.rot_y;
	m_fRotationZ	= m_MeshInfo.rot_z;
	if( m_MeshInfo.scale_xyz > 0.99999f && m_MeshInfo.scale_xyz < 1.000001f )
	{
		m_fScaling = _scale;
	}
	else
	{
		m_fScaling		= m_MeshInfo.scale_xyz;	
	}

	m_fPositionOffset = _pos;
	m_fRotationOffset = _rot;
	//m_fScalingOffset  = 0.0f;

	m_MeshName = m_MeshInfo.mesh_name;
	m_URL	   = m_MeshInfo.url;

	if( m_MeshInfo.mesh_name != '\0' )
	{
		LPNODE p = m_MeshFullPathName.find(m_MeshName);
		if( p != m_MeshFullPathName.end() )
		{
			CString path = p->second;
			CString fullPathName = path + m_MeshName;
			m_XView.ShowX(fullPathName);
		}
	}
	
	GetDlgItem(IDC_EDIT_ADD_URL)->SetFocus();

	UpdateData(FALSE);

	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

int AddModifictionDlg::FindResource(CString cstrResourcePath)
{
	m_MeshFullPathName.clear();

	/*
		��Ŀ¼��Ѱ����Դ
	*/
	char path[MAX_PATH];
	strcpy( path, cstrResourcePath.GetBuffer() );
	cstrResourcePath.ReleaseBuffer();
	strcat( path, "\\*.*");
    _finddata_t file;
    long lf;
	if((lf = (long)_findfirst(path,&file))==-1l)//����ļ�·��
        /*cout<<"�ļ�û���ҵ�!\n"*/;
    else
    {
        do
        {
           // cout<<file.name;
   //         if(file.attrib == _A_NORMAL)
			//{
			//	/*cout<<"  ��ͨ�ļ�  "*/;
			//}
   //         else if(file.attrib == _A_RDONLY)
			//{
			//	/*cout<<"  ֻ���ļ�  "*/;
			//}
   //         else if(file.attrib == _A_HIDDEN )
			//{
			//	/*cout<<"  �����ļ�  "*/;
			//}
   //         else if(file.attrib == _A_SYSTEM )
			//{
			//	/*cout<<"  ϵͳ�ļ�  "*/;
			//}
   //         else 
			//if(file.attrib == _A_SUBDIR)
			{
				/*cout<<"  ��Ŀ¼  "*/;
				//CString MeshName = cstrResourcePath + file.name + CString("\\") + file.name + CString(".x");
				//m_cstrMeshList.AddHead(MeshName);

				if( strcmp(file.name,".") == 0 || strcmp(file.name,"..") == 0 )
				{
					continue;
				}
				CString FullPath = cstrResourcePath + CString("\\") + file.name + CString("\\");
				CString MeshName = file.name + CString(".x");
				
				{
					_finddata_t _file;
					long _lf;
					CString pathName = FullPath + MeshName; 
					if((_lf = (long)_findfirst(pathName,&_file))==-1l)
					{
						continue;
					}
				}

				m_MeshFullPathName.insert(MESH_NODE(MeshName,FullPath));
			}
        }
		while( _findnext( lf, &file ) == 0 );//����ҵ��¸��ļ������ֳɹ��Ļ��ͷ���0,���򷵻�-1
    }
    _findclose(lf);

	int num = (int)m_MeshFullPathName.size();
	return num;
}

void AddModifictionDlg::OnNMClickListAdd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int row = pNMLV->iItem;
	if( row > -1 )
	{
		m_MeshName = m_MeshList.GetItemText(row,0);	

		LPNODE p = m_MeshFullPathName.find(m_MeshName);
		if( p != m_MeshFullPathName.end() )
		{
			CString path = p->second;
			CString fullPathName = path + m_MeshName;
			m_XView.ShowX(fullPathName);
				
		}
		
		UpdateData(FALSE);	
	}

	*pResult = 0;
}

void AddModifictionDlg::OnBnClickedAddOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	m_MeshInfo.rot_x = m_fRotationX;
	m_MeshInfo.rot_y = m_fRotationY;
	m_MeshInfo.rot_z = m_fRotationZ;

	m_MeshInfo.scale_xyz = m_fScaling;

	m_MeshInfo.x = m_fPositionX;
	m_MeshInfo.y = m_fPositionY;
	m_MeshInfo.z = m_fPositionZ;

	char* url =	m_URL.GetBuffer();
	strcpy( m_MeshInfo.url, url );
	m_URL.ReleaseBuffer();

	char* mesh_name = m_MeshName.GetBuffer();
	strcpy( m_MeshInfo.mesh_name, mesh_name );
	m_MeshName.ReleaseBuffer();

	if( strcmp( m_MeshInfo.mesh_name, "" ) == 0 )
	{
		AfxMessageBox("û��ѡ��ģ�ͣ�");	
	}
	else
	{
		CDialog::OnOK();
	}
}

void AddModifictionDlg::OnBnClickedAddCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}

S3DDATA& AddModifictionDlg::Get3DData(void)
{
	return m_MeshInfo;
}

void AddModifictionDlg::OnEnChangeEditAddUrl()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	UpdateData();
}

void AddModifictionDlg::SetCurData( S3DDATA& data )
{
	m_MeshInfo = data;
}

void AddModifictionDlg::OnBnClickedAddUpdata()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( g_pMainDlg == NULL )
	{
		return;
	}

	UpdateData();

	m_MeshInfo.rot_x = m_fRotationX;
	m_MeshInfo.rot_y = m_fRotationY;
	m_MeshInfo.rot_z = m_fRotationZ;

	m_MeshInfo.scale_xyz = m_fScaling;

	m_MeshInfo.x = m_fPositionX;
	m_MeshInfo.y = m_fPositionY;
	m_MeshInfo.z = m_fPositionZ;

	char* url =	m_URL.GetBuffer();
	strcpy( m_MeshInfo.url, url );
	m_URL.ReleaseBuffer();

	char* mesh_name = m_MeshName.GetBuffer();
	strcpy( m_MeshInfo.mesh_name, mesh_name );
	m_MeshName.ReleaseBuffer();

	if( strcmp( m_MeshInfo.mesh_name, "" ) == 0 )
	{
		AfxMessageBox("û��ѡ��ģ�ͣ�");	
	}
	else
	{
		g_pMainDlg->UpdataCurList(m_MeshInfo);
	}
}

void AddModifictionDlg::OnBnClickedButtonAddGetposition()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_fPositionX = g_IGA_Date[0].x;
	m_fPositionY = g_IGA_Date[0].y;
	m_fPositionZ = g_IGA_Date[0].z;

	UpdateData(FALSE);
}

void AddModifictionDlg::OnDeltaposSpinAddPositionX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	int Key = pNMUpDown->iDelta;
	if( Key == -1 )	//��
	{
		m_fPositionX += m_fPositionOffset;
	}
	else if( Key == 1 ) //��
	{
		m_fPositionX -= m_fPositionOffset;
	}

	_pos = m_fPositionOffset;

	UpdateData(FALSE);

	*pResult = 0;
}

void AddModifictionDlg::OnDeltaposSpinAddPositionY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	int Key = pNMUpDown->iDelta;
	if( Key == -1 )	//��
	{
		m_fPositionY += m_fPositionOffset;
	}
	else if( Key == 1 ) //��
	{
		m_fPositionY -= m_fPositionOffset;
	}

	_pos = m_fPositionOffset;

	UpdateData(FALSE);


	*pResult = 0;
}

void AddModifictionDlg::OnDeltaposSpinPositionZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	int Key = pNMUpDown->iDelta;
	if( Key == -1 )	//��
	{
		m_fPositionZ += m_fPositionOffset;
	}
	else if( Key == 1 ) //��
	{
		m_fPositionZ -= m_fPositionOffset;
	}

	_pos = m_fPositionOffset;

	UpdateData(FALSE);


	*pResult = 0;
}

void AddModifictionDlg::OnDeltaposSpinRotationX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	int Key = pNMUpDown->iDelta;
	if( Key == -1 )	//��
	{
		m_fRotationX += m_fRotationOffset;
	}
	else if( Key == 1 ) //��
	{
		m_fRotationX -= m_fRotationOffset;
	}

	_rot = m_fRotationOffset;

	UpdateData(FALSE);

	*pResult = 0;
}

void AddModifictionDlg::OnDeltaposSpinrotationY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	int Key = pNMUpDown->iDelta;
	if( Key == -1 )	//��
	{
		m_fRotationY += m_fRotationOffset;
	}
	else if( Key == 1 ) //��
	{
		m_fRotationY -= m_fRotationOffset;
	}

	_rot = m_fRotationOffset;

	UpdateData(FALSE);

	*pResult = 0;
}

void AddModifictionDlg::OnDeltaposSpinAddRotationZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	int Key = pNMUpDown->iDelta;
	if( Key == -1 )	//��
	{
		m_fRotationZ += m_fRotationOffset;
	}
	else if( Key == 1 ) //��
	{
		m_fRotationZ -= m_fRotationOffset;
	}

	_rot = m_fRotationOffset;

	UpdateData(FALSE);

	*pResult = 0;
}

void AddModifictionDlg::OnDeltaposSpinAddScaling(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	
	float _fScaleOffset = fabs(m_fScalingOffset);
	int Key = pNMUpDown->iDelta;
	if( Key == -1 )	//��
	{
		float _fScale = m_fScaling + _fScaleOffset;
		if( _fScale < 1000 )
		{
			if( _fScale/_fScaleOffset > 10.0f )
			{
				_fScaleOffset *= 2.0f;
			}
			m_fScaling += _fScaleOffset;
		}
	}
	else if( Key == 1 ) //��
	{
		float _fScale = m_fScaling - _fScaleOffset;
		if( _fScale >= 0.0001f )
		{
			if( _fScale < 2*_fScaleOffset )
			{
				_fScaleOffset /= 2.0f;
			}
			m_fScaling -= _fScaleOffset;
		}

	}
	m_fScalingOffset = _fScaleOffset;
	_scale = m_fScaling;
	UpdateData(FALSE);

	*pResult = 0;
}

void AddModifictionDlg::OnBnClickedButtonAddOpenUrl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShellExecute( this->GetSafeHwnd(), "open", m_URL, NULL, NULL, SW_SHOW );
}

CString AddModifictionDlg::GetMeshPath(CString MeshName)
{
	LPNODE p = m_MeshFullPathName.find(MeshName);
	if( p != m_MeshFullPathName.end() )
	{
		return p->second;
	}
	return CString();
}

void AddModifictionDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	OnBnClickedAddCancel();
}
