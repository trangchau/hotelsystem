// HotelPMSDoc.cpp : CSearchDoc �N���X�̎���
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "SearchDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSearchDoc

IMPLEMENT_DYNCREATE(CSearchDoc, CDocument)

BEGIN_MESSAGE_MAP(CSearchDoc, CDocument)
END_MESSAGE_MAP()


// CSearchDoc �R���X�g���N�V����/�f�X�g���N�V����

CSearchDoc::CSearchDoc()
{
	// TODO: ���̈ʒu�� 1 �x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CSearchDoc::~CSearchDoc()
{
}

BOOL CSearchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("ReservationSearch"));

	return TRUE;
}




// CSearchDoc �V���A����

void CSearchDoc::Serialize(CArchive& ar)
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


// CSearchDoc �f�f

#ifdef _DEBUG
void CSearchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSearchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSearchDoc �R�}���h
