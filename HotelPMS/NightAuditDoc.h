// HotelPMSDoc.h : CNightAuditDoc �N���X�̃C���^�[�t�F�C�X
//

#include "HotelPMSDoc.h"

#pragma once


class CNightAuditDoc : public CDocument
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CNightAuditDoc();
	DECLARE_DYNCREATE(CNightAuditDoc)

// ����
public:

// ����
public:

// �I�[�o�[���C�h
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ����
public:
	virtual ~CNightAuditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
};


