// IGASSeeDoc.h :  CIGASSeeDoc ��Ľӿ�
//


#pragma once

class CIGASSeeDoc : public CDocument
{
protected: // �������л�����
	CIGASSeeDoc();
	DECLARE_DYNCREATE(CIGASSeeDoc)

// ����
public:

// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CIGASSeeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


