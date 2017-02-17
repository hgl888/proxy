// ServerULogConfigDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

// CServerULogConfigDlg �Ի���
class CServerULogConfigDlg : public CDialog
{
// ����
public:
	CServerULogConfigDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVERULOGCONFIG_DIALOG };

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

private:
	CString m_strServerUPath;
	CString m_strLogPath;
	CString m_strFilePath;
	CListCtrl m_lsctrlFilterFile;
	CString m_strModulePath ;
	CListCtrl m_lsctrlFilePath;

public:

	afx_msg void OnBnClickedBtnServerupath();
	afx_msg void OnBnClickedBtnLogpath();
	afx_msg void OnBnClickedBtnFilepath();
	afx_msg void OnBnClickedBtnAddfilterfile();

	void OpenFilePath(CString &strFilePath);
	void InitData();
	afx_msg void OnNMDblclkListFilterfile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSavadata();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	afx_msg void OnNMDblclkListFilepath(NMHDR *pNMHDR, LRESULT *pResult);
	bool Savadata();
};
