#pragma once


// CSimpleGrid

#include "CustomEdit.h"
#include "DateEdit.h"
#include "TimeEdit.h"
#include "CommaEdit.h"
#include "CustomCombo.h"

#define	GAL_LFT		(DT_LEFT|DT_SINGLELINE|DT_VCENTER)
#define	GAL_CTR		(DT_CENTER|DT_SINGLELINE|DT_VCENTER)
#define	GAL_RGT		(DT_RIGHT|DT_SINGLELINE|DT_VCENTER)

#define	GCT_NML		0			// �ʏ�Z��
#define	GCT_EDT		1			// ���͉\�Z��
#define	GCT_DED		2			// ���t���͉\�Z��
#define	GCT_TED		4			// �������͉\�Z��
#define	GCT_CED		8			// Comma edit cell
#define	GCT_CHK		16			// �`�F�b�N�{�b�N�X�Z��
#define	GCT_CMB		32			// �R���{�{�b�N�X�Z��
#define	GCT_CLR		64			// �F�I���Z��
#define	GCT_PNS		128			// ����I���Z��

#define	GCT_CDF		-1			// �J������`�ɏ]��

#define	GCT_NMO		512			// ���l���͂̂�

#define	GSL_NON		0			// �I���Ȃ�
#define	GSL_ROW		1			// �s�I�����
#define	GSL_CLM		2			// ��I�����


typedef	struct	{
	int			wid;			// ��
	int			alg;			// �I���W��
	int			typ;			// �Z���^�C�v
	int			dgt;			// �����i�o�C�g���j
}GRD_CLM;

class CSimpleGrid;

typedef	struct	{				// �O���b�h��`
	int			vlc;			// ���s���i�w�b�_�[�s�܂ށj
	int			vcc;			// ������
	int			lnh;			// �P�s����
	int			sel;			// �I�����
	COLORREF	hbc;			// �w�b�_�[�w�i�F
	COLORREF	hfc;			// �w�b�_�[�����F
	COLORREF	dbc;			// ���הw�i�F
	COLORREF	dfc;			// ���ו����F
	COLORREF	sbc;			// �I����Ԕw�i�F
	COLORREF	sfc;			// �I����ԕ����F
	int			fsa;			// �t�H���g�T�C�Y��������
	int			hdr;			// �w�b�_�[�L��
	int			cct;			// �J������
	GRD_CLM		*clm;			// �J������`
}GRD_DEF;

typedef	struct	{				// �O���b�h�ʒm�\����
	int			ntf;			// �ʒm�R�[�h
	int			cid;			// �R���g���[���h�c
	int			msx;			// �}�E�X���W�i�e�E�B���h�E�㑊�Έʒu�j
	int			msy;			// �}�E�X���W�i�e�E�B���h�E�㑊�Έʒu�j
	int			clm;			// �J�����ʒu�i��Έʒu�j
	int			row;			// ���[�ʒu�i��Έʒu�j
	CSimpleGrid	*grd;			// Instance of own grid
}GRD_NTF;

#define	WM_GRIDNOTIFY		(WM_USER+16384)		// �O���b�h�ʒm���b�Z�[�W

#define	GN_LBUTTONCLICK		1					// ���{�^���N���b�N
#define	GN_RBUTTONCLICK		2					// �E�{�^���N���b�N
#define	GN_EDITCOMPLETE		3					// �ҏW����
#define	GN_QUERYCOMBODATA	4					// �R���{�{�b�N�X�f�[�^�v��
#define	GN_COMBOCHANGED		5					// �R���{�{�b�N�X�ύX����
#define	GN_QUERYINPUTTYPE	6					// ���͕����v��
#define	GN_QUERYCMEPARAM	7					// ���͕����v��
#define	GN_QUERYCOLORDATA	8					// �F�f�[�^�v��
#define	GN_SELECTCHANGE		9					// Select change

#define	GN_LBUTTONDBLCLICK	10					// ���{�^���_�u���N���b�N

#define ON_WM_GRIDNOTIFY() \
	{ WM_GRIDNOTIFY, 0, 0, 0, AfxSig_lwl, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM) > \
		( &ThisClass :: OnGridNotify)) },

class CSimpleGrid : public CWnd
{
	DECLARE_DYNAMIC(CSimpleGrid)
private:
	CWnd		*m_pParent;
	RECT		m_Size[1];
	GRD_DEF		m_Grd_def[1];
	HFONT		m_Font;

	int				m_TopRow;			// �\���擪�s�i�P����X�^�[�g�j
	int				m_LeftColumn;		// �\���擪�J�����i�O����X�^�[�g�j
	int				m_LineCount;		// ���݂̑S�s���i�w�b�_�[�܂ށj
	WCHAR			*m_Str_tbl;			// �\��������e�[�u��
	int				*m_Val_tbl;			// ���ڃf�[�^�e�[�u��
	char			*m_Enb_tbl;			// �J��������L���f�[�^
	COLORREF		*m_Fcl_tbl;			// �t�H���g�F�e�[�u��
	COLORREF		*m_Bcl_tbl;			// �w�i�F�e�[�u��
	int				m_VScroll;			// �����X�N���[������
	int				m_HScroll;			// �����X�N���[������
	int				m_SelRow;			// �I���s
	int				m_SelColumn;		// �I���
	int				m_InputEnable;		// ���͗L���t���O
	int				m_Fst_flg;			// �ŏ��̌ďo���t���O
	CCustomEdit		*m_EditBox;			// �G�f�B�b�g�{�b�N�X
	CDateEdit		*m_DateEditBox;		// �G�f�B�b�g�{�b�N�X
	CTimeEdit		*m_TimeEditBox;		// �G�f�B�b�g�{�b�N�X
	CCommaEdit		*m_CommaEditBox;	// �G�f�B�b�g�{�b�N�X
	CCustomCombo	*m_ComboBox;		// �R���{�{�b�N�X
	int				m_EditFlg;			// �ҏW���t���O
	WCHAR			m_InputChar;		// �I����Ԃŉ����������L�[

	void	CalcCellRect( RECT *, int, int );
	int		MPointToCell( int *, int *, int, int );
	void	SetVScrollBar( void );
	void	SetHScrollBar( void );
	LRESULT	SendNotify( int, int, int, int, int, LPARAM );
	LRESULT	PostNotify( int, int, int, int, int, LPARAM );

	void	ChangeSelect( int, int );		// �I���J�����i���[�j�ύX
	void	OnArrowKey( UINT );
	void	OnSpaceKey( void );
	void	OnEnterKey( char );
	void	SelectCell( int, int );

public:
	CSimpleGrid( /*RECT *rct, CWnd *pParent*/ );
	virtual ~CSimpleGrid();
	int		AddRow( int );
	int		DelRow( int );
	void	ResetContent();
	void	GetCellText( WCHAR *, int , int, int );
	void	SetCellText( int, int, WCHAR * );
	void	SetCellColor( int, int, COLORREF, COLORREF );
	int		SetCheck( int, int, int );
	int		GetCheck( int, int );
	int		GetSelRow( void );
	int		GetSelCol( void );
	int		SetEnable( int, int, int );
	int		SetData( int, int, int );
	int		GetData( int, int );
	int		InputEnable( int );
	int		IsHeader();
	void	SetSelectCell( int, int );
	int		GetLineCount( int IncludingHeader );
	void	Redraw(void);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual BOOL Create(GRD_DEF *, const RECT& rect, UINT nID, CWnd* pParentWnd);
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnEditNotify( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnComboNotify( WPARAM wParam, LPARAM lParam );
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


