// HotelPMSDoc.cpp : CCheckinDoc �N���X�̎���
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "CheckinDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCheckinDoc

IMPLEMENT_DYNCREATE(CCheckinDoc, CDocument)

BEGIN_MESSAGE_MAP(CCheckinDoc, CDocument)
END_MESSAGE_MAP()


// CCheckinDoc �R���X�g���N�V����/�f�X�g���N�V����

CCheckinDoc::CCheckinDoc()
{
	// TODO: ���̈ʒu�� 1 �x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CCheckinDoc::~CCheckinDoc()
{
}

BOOL CCheckinDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("Checkin"));

	return TRUE;
}




// CCheckinDoc �V���A����

void CCheckinDoc::Serialize(CArchive& ar)
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


// CCheckinDoc �f�f

#ifdef _DEBUG
void CCheckinDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCheckinDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCheckinDoc �R�}���h
