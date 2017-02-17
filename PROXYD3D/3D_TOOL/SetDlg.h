#pragma once
#include "afxwin.h"


// CSetDlg �Ի���

class CSetDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CSetDlg)

public:
	CSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetDlg();
// ��д
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// �Ի�������
	enum { IDD = IDD_DIALOG_SET, IDH = IDR_HTML_SETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
private:
	CString m_cstrResourcePath;
	CString m_cstrResourceOutputPath;
	void SelectPath(char* path);

public:
	CString GetResourcePath(void);
	CString GetResourceOutputPath(void);
	afx_msg void OnBnClickedSetOk();
	afx_msg void OnBnClickedSetCancel();
	afx_msg void OnBnClickedButtonSetBrowse();
	afx_msg void OnBnClickedButtonSetResOutBrowse();
private:
	CEdit m_EditOutputPath;
public:
	void SetPath(CString ResourcePath, CString outputPath);
};
