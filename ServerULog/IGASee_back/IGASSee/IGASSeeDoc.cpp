// IGASSeeDoc.cpp :  CIGASSeeDoc ���ʵ��
//

#include "stdafx.h"
#include "IGASSee.h"

#include "IGASSeeDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIGASSeeDoc

IMPLEMENT_DYNCREATE(CIGASSeeDoc, CDocument)

BEGIN_MESSAGE_MAP(CIGASSeeDoc, CDocument)
END_MESSAGE_MAP()


// CIGASSeeDoc ����/����

CIGASSeeDoc::CIGASSeeDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CIGASSeeDoc::~CIGASSeeDoc()
{
}

BOOL CIGASSeeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CIGASSeeDoc ���л�

void CIGASSeeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CIGASSeeDoc ���

#ifdef _DEBUG
void CIGASSeeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIGASSeeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CIGASSeeDoc ����
