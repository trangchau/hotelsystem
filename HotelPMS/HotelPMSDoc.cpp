// HotelPMSDoc.cpp : CHotelPMSDoc �N���X�̎���
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "HotelPMSDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHotelPMSDoc

IMPLEMENT_DYNCREATE(CHotelPMSDoc, CDocument)

BEGIN_MESSAGE_MAP(CHotelPMSDoc, CDocument)
END_MESSAGE_MAP()


// CHotelPMSDoc �R���X�g���N�V����/�f�X�g���N�V����

CHotelPMSDoc::CHotelPMSDoc()
{
	// TODO: ���̈ʒu�� 1 �x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CHotelPMSDoc::~CHotelPMSDoc()
{
}

BOOL CHotelPMSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}




// CHotelPMSDoc �V���A����

void CHotelPMSDoc::Serialize(CArchive& ar)
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


// CHotelPMSDoc �f�f

#ifdef _DEBUG
void CHotelPMSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHotelPMSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

