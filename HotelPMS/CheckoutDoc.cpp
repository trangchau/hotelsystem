// CheckoutDoc.cpp : CCheckoutDoc �N���X�̎���
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "CheckoutDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCheckoutDoc

IMPLEMENT_DYNCREATE(CCheckoutDoc, CDocument)

BEGIN_MESSAGE_MAP(CCheckoutDoc, CDocument)
END_MESSAGE_MAP()


// CCheckoutDoc �R���X�g���N�V����/�f�X�g���N�V����

CCheckoutDoc::CCheckoutDoc()
{
	// TODO: ���̈ʒu�� 1 �x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CCheckoutDoc::~CCheckoutDoc()
{
}

BOOL CCheckoutDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("CheckoutOperation"));

	return TRUE;
}




// CCheckoutDoc �V���A����

void CCheckoutDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �i�[����R�[�h�������ɒǉ����Ă��������B
	}
	else
	{
		// TODO: �ǂݍ��ރR�[�h�������ɒǉ����Ă��������B
	}
}


// CCheckoutDoc �f�f

#ifdef _DEBUG
void CCheckoutDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCheckoutDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCheckoutDoc �R�}���h
