// InhouseDoc.cpp : CInhouseDoc �N���X�̎���
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "InhouseDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInhouseDoc

IMPLEMENT_DYNCREATE(CInhouseDoc, CDocument)

BEGIN_MESSAGE_MAP(CInhouseDoc, CDocument)
END_MESSAGE_MAP()


// CInhouseDoc �R���X�g���N�V����/�f�X�g���N�V����

CInhouseDoc::CInhouseDoc()
{
	// TODO: ���̈ʒu�� 1 �x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CInhouseDoc::~CInhouseDoc()
{
}

BOOL CInhouseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("InhouseOperation"));

	return TRUE;
}




// CInhouseDoc �V���A����

void CInhouseDoc::Serialize(CArchive& ar)
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


// CInhouseDoc �f�f

#ifdef _DEBUG
void CInhouseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CInhouseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CInhouseDoc �R�}���h
