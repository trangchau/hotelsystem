#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBarCombo.h : �w�b�_�[ �t�@�C��
//

#include "..\Custom\CustomCombo.h"

/////////////////////////////////////////////////////////////////////////////
// CToolBarCombo �E�B���h�E

//class CToolBarCombo : public CComboBox
class CToolBarCombo : public CCustomCombo
{
// �R���X�g���N�V����
public:
	CToolBarCombo();
	CWnd		*m_NotifyWnd;

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CToolBarCombo)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CToolBarCombo();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CToolBarCombo)
	afx_msg void OnSelchange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B
