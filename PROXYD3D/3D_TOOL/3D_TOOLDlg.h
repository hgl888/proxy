// 3D_TOOLDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

#include "AddModifictionDlg.h"
#include "SetDlg.h"
#include "afxwin.h"
#include <map>
#include <iostream>
#include <fstream>

#include <direct.h>

using namespace std;

/*
	����ṹ��������¼ģ�͵����ơ�ʹ�õ���������ơ�
*/
struct SFileCopy
{
	static int nCount;
	int		mesh_id;
    bool	bIsCopy;
	CString     MeshName;
	CStringList NameList;
	SFileCopy()
	{
		mesh_id = -1;
		bIsCopy = false;
		MeshName = "";
		NameList.RemoveAll();
	}

	SFileCopy( const SFileCopy& fileCopyObj )
	{
		mesh_id = fileCopyObj.mesh_id;
		bIsCopy = fileCopyObj.bIsCopy;
		MeshName = fileCopyObj.MeshName;
		POSITION   pos = fileCopyObj.NameList.GetHeadPosition();
		while( pos != NULL )
		{
			CString str =  fileCopyObj.NameList.GetNext(pos);
			NameList.AddHead(str);
		}
	}

	~SFileCopy()
	{
		NameList.RemoveAll();
	}
};

typedef map<CString,SFileCopy>	MESH_COPY_MAP;
typedef map<CString,SFileCopy>::iterator LPCOPYNODE;
typedef pair<CString,SFileCopy> COPY_NODE;

//----------------------------------------------------------------------------
// CD_TOOLDlg �Ի���
class CD_TOOLDlg : public CDialog
{
// ����
public:
	CD_TOOLDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_D_TOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSet();
private:
	CString m_GameName;
	CString m_ResourcePath;
	CString m_ResourceOutputPath;
	CListCtrl m_3DInfoList;

	AddModifictionDlg	m_AddModifictionDlg;
	CSetDlg				m_SetDlg;

	/*
		�����ţ�ģ�͵������Ŵ�0��ʼ��ÿ���һ���ͼ�1��ɾ��ģ�Ͳ���������
	*/
	int m_nDataIndex;

	/*
		�����б�
	*/
	typedef map<int,S3DDATA>	DATA_MAP;
	typedef map<int,S3DDATA>::iterator LPNODE;
	typedef pair<int,S3DDATA>	DATA_NODE;

	DATA_MAP	m_DataMap;

	LPNODE	m_CurDataNode;
	int     m_nSelectIndex;

	/*
		����data.ini��Adinfo.ini		
	*/
	void UpdateDataINI();
	void UpdataAdInfoXML();

	//ģ���ļ��ĸ��Ƽ�¼����
	MESH_COPY_MAP	m_MeshCopyMap;

	/*
		��ѡ��Ķ���ID��������Ϸ����Ӷ��ģ�ͺ�Ҫ�޸�ʱ�����ֲ������һ��ģ�ͣ�ʹ����ͬģ��ʱ��
		�������Ҫÿ��һ��ʱ���¼һ�Ρ�
	*/
	

public:
	afx_msg void OnBnClickedButtonAdd();
private:
	void SetEnable(bool bIsEnable );
	CButton m_ButtonAdd;
	CButton m_ButtonDelete;
	CButton m_ButtonClear;
	CButton m_ButtonOutput;
	CButton m_ButtonSet;
	CButton m_ButtonQuit;
public:
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnNMRclickList3dinfo(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnOK(){};
	afx_msg void OnCancel(){};

	afx_msg void OnNMClickList3dinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonOutput();
	void UpdataCurList(S3DDATA& data);
	void GetMeshResNameList(CString name, CStringList* pCStringList);
	void Delete3DRes( bool bIsAll = true, CString name = "" );
	afx_msg void OnTimer(UINT nIDEvent);
private:
	int m_nCurPickID;
public:
	afx_msg void OnClose();
private:
	BOOL m_bShowPickMesh;
};
