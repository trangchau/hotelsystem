// HotelPMSDoc.h : CSearchDoc �N���X�̃C���^�[�t�F�C�X
//


#pragma once


class CSearchDoc : public CDocument
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CSearchDoc();
	DECLARE_DYNCREATE(CSearchDoc)

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
	virtual ~CSearchDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
};


