// HotelPMSDoc.h : CReservationDoc �N���X�̃C���^�[�t�F�C�X
//

#include "HotelPMSDoc.h"

#pragma once


class CReservationDoc : public CDocument
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CReservationDoc();
	DECLARE_DYNCREATE(CReservationDoc)

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
	virtual ~CReservationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
};


