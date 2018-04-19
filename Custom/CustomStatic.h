#if !defined(AFX_KOSUSTATIC_H__1238DD05_39B7_438E_8E83_8ABC6DA72EA2__INCLUDED_)
#define AFX_KOSUSTATIC_H__1238DD05_39B7_438E_8E83_8ABC6DA72EA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomStatic.h : �w�b�_�[ �t�@�C��
//

#define	WM_STATICNOTIFY		(WM_USER+32777)

#define	SN_LBUTTONCLICK		0x1000				// �E�{�^���N���b�N
#define	SN_RBUTTONCLICK		0x1001				// �E�{�^���N���b�N
#define	SN_EDITCOMPLETE		0x1002				// �ҏW����

#define ON_WM_STATICNOTIFY() \
	{ WM_STATICNOTIFY, 0, 0, 0, AfxSig_lwl, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM) > \
		( &ThisClass :: OnStaticNotify)) },

#define	wsizeof(x)			((x)*sizeof(WCHAR))

/////////////////////////////////////////////////////////////////////////////
// CCustomStatic �E�B���h�E

class CCustomStatic : public CStatic
{
private:
	CFont		m_Font;
	LOGFONT		m_LogFont;
	COLORREF	m_fgColor;
	HBRUSH		m_bgBrush;
	COLORREF	m_ftColor;
// �R���X�g���N�V����
public:
	CCustomStatic();
	void	ResetFont( HFONT hFont );
	int			AutoSize;

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
	void SetBGColor(COLORREF bgColor);
	void SetFTColor(COLORREF fcColor);
	void SetFont(  WCHAR *pName, int nSize, COLORREF nFontColor );
	void SetText(  WCHAR *pText );
	//void SetFontByHandle( HFONT hFont, bool rdw );
// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CCustomStatic)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CCustomStatic();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CCustomStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_KOSUSTATIC_H__1238DD05_39B7_438E_8E83_8ABC6DA72EA2__INCLUDED_)
