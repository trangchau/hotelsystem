#if !defined(AFX_CUSTOMLISTBOX_H__7819539E_A89B_450E_AB33_1D1EE6630E13__INCLUDED_)
#define AFX_CUSTOMLISTBOX_H__7819539E_A89B_450E_AB33_1D1EE6630E13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomListBox.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CCustomListBox �E�B���h�E

class CCustomListBox : public CListBox
{
private:
	LOGFONT		m_LogFont;
	COLORREF	m_fgColor;
	HBRUSH		m_bgBrush;
	CFont		m_Font; 
// �R���X�g���N�V����
public:
	CCustomListBox();
	void	ResetFont( HFONT hFont );

// �A�g���r���[�g
public:

// �I�y���[�V����
public:
	// �����ɃA���_�[���C��������
	BOOL SetTextUnderLine( BOOL bflg = FALSE);
	// �������C�^���b�N�ɂ���
	BOOL SetTextItalic( BOOL bflg = FALSE);
	// �����ݒ�
	BOOL SetTextHeight(int height);
	// �w�i�F�̎w��
	BOOL SetBackColor( COLORREF color);
	// �������{�[���h�ɂ���
	BOOL SetTextBold( BOOL bflg = FALSE);
	// �����̐F���w�肷��
    BOOL SetForeColor( COLORREF color);
	// �������E�T�u�N���X��
	void InitSubclass( UINT, COLORREF, COLORREF, int, int, CWnd * );
	void SetFontByHandle( HFONT hFont, bool rdw );
// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CCustomListBox)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CCustomListBox();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CCustomListBox)
//	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CUSTOMLISTBOX_H__7819539E_A89B_450E_AB33_1D1EE6630E13__INCLUDED_)
