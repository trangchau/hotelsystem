// HotelPMSDoc.h : CCheckinDoc �N���X�̃C���^�[�t�F�C�X
//


#pragma once


class CCheckinDoc : public CDocument
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CCheckinDoc();
	DECLARE_DYNCREATE(CCheckinDoc)

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
	virtual ~CCheckinDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
};


