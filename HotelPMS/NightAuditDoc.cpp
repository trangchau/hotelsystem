// HotelPMSDoc.cpp : CNightAuditDoc �N���X�̎���
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "NightAuditDoc.h"
#include "NightAuditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNightAuditDoc

IMPLEMENT_DYNCREATE(CNightAuditDoc, CDocument)

BEGIN_MESSAGE_MAP(CNightAuditDoc, CDocument)
END_MESSAGE_MAP()


// CNightAuditDoc �R���X�g���N�V����/�f�X�g���N�V����

CNightAuditDoc::CNightAuditDoc()
{
	// TODO: ���̈ʒu�� 1 �x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CNightAuditDoc::~CNightAuditDoc()
{
}

BOOL CNightAuditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("NightAudit"));

	return TRUE;
}




// CNightAuditDoc �V���A����

void CNightAuditDoc::Serialize(CArchive& ar)
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


// CNightAuditDoc �f�f

#ifdef _DEBUG
void CNightAuditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNightAuditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNightAuditDoc �R�}���h
