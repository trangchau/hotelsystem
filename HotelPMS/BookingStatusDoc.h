// BookingStatusDoc.h : CBookingStatusDoc �N���X�̃C���^�[�t�F�C�X
//


#pragma once


class CBookingStatusDoc : public CDocument
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CBookingStatusDoc();
	DECLARE_DYNCREATE(CBookingStatusDoc)

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
	virtual ~CBookingStatusDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
};


