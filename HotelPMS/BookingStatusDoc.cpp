// BookingStatusDoc.cpp : CBookingStatusDoc �N���X�̎���
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "BookingStatusDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBookingStatusDoc

IMPLEMENT_DYNCREATE(CBookingStatusDoc, CDocument)

BEGIN_MESSAGE_MAP(CBookingStatusDoc, CDocument)
END_MESSAGE_MAP()


// CBookingStatusDoc �R���X�g���N�V����/�f�X�g���N�V����

CBookingStatusDoc::CBookingStatusDoc()
{
	// TODO: ���̈ʒu�� 1 �x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CBookingStatusDoc::~CBookingStatusDoc()
{
}

BOOL CBookingStatusDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("BookingStatus"));

	return TRUE;
}




// CBookingStatusDoc �V���A����

void CBookingStatusDoc::Serialize(CArchive& ar)
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


// CBookingStatusDoc �f�f

#ifdef _DEBUG
void CBookingStatusDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBookingStatusDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBookingStatusDoc �R�}���h
