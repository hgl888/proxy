// IGASSeeView.h : CIGASSeeView ��Ľӿ�
//


#pragma once


class CIGASSeeView : public CView
{
protected: // �������л�����
	CIGASSeeView();
	DECLARE_DYNCREATE(CIGASSeeView)

// ����
public:
	CIGASSeeDoc* GetDocument() const;

// ����
public:

// ��д
	public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CIGASSeeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // IGASSeeView.cpp �ĵ��԰汾
inline CIGASSeeDoc* CIGASSeeView::GetDocument() const
   { return reinterpret_cast<CIGASSeeDoc*>(m_pDocument); }
#endif

