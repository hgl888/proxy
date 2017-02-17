// IGASSeeView.cpp : CIGASSeeView ���ʵ��
//

#include "stdafx.h"
#include "IGASSee.h"

#include "IGASSeeDoc.h"
#include "IGASSeeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIGASSeeView

IMPLEMENT_DYNCREATE(CIGASSeeView, CView)

BEGIN_MESSAGE_MAP(CIGASSeeView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CIGASSeeView ����/����

CIGASSeeView::CIGASSeeView()
{
	// TODO: �ڴ˴���ӹ������

}

CIGASSeeView::~CIGASSeeView()
{
}

BOOL CIGASSeeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CView::PreCreateWindow(cs);
}

// CIGASSeeView ����

void CIGASSeeView::OnDraw(CDC* /*pDC*/)
{
	CIGASSeeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CIGASSeeView ��ӡ

BOOL CIGASSeeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CIGASSeeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡǰ��Ӷ���ĳ�ʼ��
}

void CIGASSeeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡ������������
}


// CIGASSeeView ���

#ifdef _DEBUG
void CIGASSeeView::AssertValid() const
{
	CView::AssertValid();
}

void CIGASSeeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIGASSeeDoc* CIGASSeeView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIGASSeeDoc)));
	return (CIGASSeeDoc*)m_pDocument;
}
#endif //_DEBUG


// CIGASSeeView ��Ϣ�������
