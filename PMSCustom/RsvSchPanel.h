#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsvSchPanel.h : �w�b�_�[ �t�@�C��
//

#include "..\Custom\SimpleGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CRsvSchPanel �E�B���h�E

class CRsvSchPanel : public CStatic
{
private:

	// �R���X�g���N�V����
public:
	CRsvSchPanel();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

	// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CRsvSchPanel)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CRsvSchPanel();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CRsvSchPanel)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B
