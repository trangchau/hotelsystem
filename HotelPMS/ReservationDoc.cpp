// HotelPMSDoc.cpp : CReservationDoc �N���X�̎���
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "ReservationDoc.h"
#include "ReservationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CReservationDoc

IMPLEMENT_DYNCREATE(CReservationDoc, CDocument)

BEGIN_MESSAGE_MAP(CReservationDoc, CDocument)
END_MESSAGE_MAP()


// CReservationDoc �R���X�g���N�V����/�f�X�g���N�V����

CReservationDoc::CReservationDoc()
{
	// TODO: ���̈ʒu�� 1 �x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CReservationDoc::~CReservationDoc()
{
}

BOOL CReservationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("ReservationInformation"));

	return TRUE;
}




// CReservationDoc �V���A����

void CReservationDoc::Serialize(CArchive& ar)
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


// CReservationDoc �f�f

#ifdef _DEBUG
void CReservationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CReservationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CReservationDoc �R�}���h
