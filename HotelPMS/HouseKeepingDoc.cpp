// HouseKeepingDoc.cpp : CHouseKeepingDoc �N���X�̎���
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "HouseKeepingDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHouseKeepingDoc

IMPLEMENT_DYNCREATE(CHouseKeepingDoc, CDocument)

BEGIN_MESSAGE_MAP(CHouseKeepingDoc, CDocument)
END_MESSAGE_MAP()


// CHouseKeepingDoc �R���X�g���N�V����/�f�X�g���N�V����

CHouseKeepingDoc::CHouseKeepingDoc()
{
	// TODO: ���̈ʒu�� 1 �x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CHouseKeepingDoc::~CHouseKeepingDoc()
{
}

BOOL CHouseKeepingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("HouseKeepingOperation"));

	return TRUE;
}




// CHouseKeepingDoc �V���A����

void CHouseKeepingDoc::Serialize(CArchive& ar)
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


// CHouseKeepingDoc �f�f

#ifdef _DEBUG
void CHouseKeepingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHouseKeepingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CHouseKeepingDoc �R�}���h
