// CustomListBox.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include <afxwin.h>
#include "CustomListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomListBox

CCustomListBox::CCustomListBox()
{
	// �f�t�H���g��GUI�t�H���g���擾
	HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	::GetObject( hFont, sizeof( LOGFONT), &m_LogFont);
	strcpy_s(m_LogFont.lfFaceName, sizeof(m_LogFont.lfFaceName), "�l�r �S�V�b�N");				// �t�H���g�ύX
	// �t�H���g���쐬
	m_Font.CreateFontIndirect( &m_LogFont);
	// �f�t�H���g�̕����\���̐F���擾
	m_fgColor = ::GetSysColor( COLOR_WINDOWTEXT );
	// �w�i�̃u���V���쐬
	m_bgBrush = ::CreateSolidBrush(GetSysColor(COLOR_WINDOW));
}

CCustomListBox::~CCustomListBox()
{
	if ( m_bgBrush )	::DeleteObject( m_bgBrush );
	m_bgBrush = NULL;
	m_Font.DeleteObject();
}

BOOL CCustomListBox::SetTextUnderLine(BOOL bflg)
{
	// LOGFONT�\���̂�Italic������ǉ��E�폜
	m_LogFont.lfUnderline = bflg;

	// �t�H���g���쐬
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont);

	// �E�B���h�E���ĕ`��
	return RedrawWindow();
}

BOOL CCustomListBox::SetTextItalic(BOOL bflg)
{
	// LOGFONT�\���̂�Italic������ǉ��E�폜
	m_LogFont.lfItalic = bflg;

	// �t�H���g���쐬
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont);

    // �E�B���h�E���ĕ`��
	return RedrawWindow();
}

BOOL CCustomListBox::SetTextHeight(int height)
{
	// LOGFONT�\���̂�Italic������ǉ��E�폜
	m_LogFont.lfHeight = height;
	m_LogFont.lfWidth  = 0;
	// �t�H���g���쐬
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont );
	// �E�B���h�E���ĕ`��
	return RedrawWindow();
}

BOOL CCustomListBox::SetBackColor(COLORREF color)
{
	// �w�肳�ꂽ�F����u���V���쐬
	if ( m_bgBrush )	::DeleteObject( m_bgBrush );
	m_bgBrush = ::CreateSolidBrush( color );

	// �E�B���h�E���ĕ`��
	return RedrawWindow();
}

BOOL CCustomListBox::SetTextBold(BOOL bflg)
{
    // LOGFONT�\���̂�BOLD������ǉ��E�폜
    if( bflg )	m_LogFont.lfWeight = FW_BOLD;
    else		m_LogFont.lfWeight = FW_NORMAL;

	// �t�H���g���쐬
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont);

	// �E�B���h�E���ĕ`��
	return RedrawWindow();
}

void CCustomListBox::ResetFont( HFONT hFont )
{
	::GetObject( hFont, sizeof( LOGFONT), &m_LogFont);
	// �t�H���g���쐬
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont);
}

BOOL CCustomListBox::SetForeColor(COLORREF color)
{
	// �t�H�A�J���[��ݒ�
	m_fgColor = color;
	// �E�B���h�E���ĕ`��
	return RedrawWindow();
}

void CCustomListBox::SetFontByHandle( HFONT hFont, bool rdw )
{
	::GetObject( hFont, sizeof( LOGFONT), &m_LogFont);
	if ( rdw )	RedrawWindow();
}

void CCustomListBox::InitSubclass(
UINT		nID,					// �_�C�A���O�h�c
COLORREF	fcl,					// �t�H���g�J���[
COLORREF	bcl,					// �t�H���g�J���[
int			bld,					// �{�[���h�L��
int			itl,					// �C�^���b�N�L��
CWnd		*par )					// �e�E�B���h�E
{
	SubclassDlgItem( nID, par );
	SetBackColor( bcl );
	SetForeColor( fcl );
	SetTextBold( bld );
	SetTextItalic( itl );
}

BEGIN_MESSAGE_MAP(CCustomListBox, CListBox)
	//{{AFX_MSG_MAP(CCustomListBox)
//	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomListBox ���b�Z�[�W �n���h��

/*
HBRUSH CCustomListBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: ���̈ʒu�� DC �̃A�g���r���[�g��ύX���Ă�������
	
    if( nCtlColor == CTLCOLOR_LISTBOX){
		// �t�H���g��ݒ�
		pDC->SelectObject( &m_Font );
		// �w�i�́A����
		pDC->SetBkMode( TRANSPARENT );
		// �e�L�X�g�̐F���w��
		pDC->SetTextColor( m_fgColor );
    }

	// TODO: �e�n���h�����Ăяo�������Ȃ��ꍇ�͂����Ńu���V�̃n���h���Ƃ��� NULL �ȊO��Ԃ��悤�ɂ��Ă�������
	if ( !!m_bgBrush )	return	m_bgBrush;
	return NULL;
}
*/

void CCustomListBox::DrawItem( LPDRAWITEMSTRUCT dis )
{
	char			*txt;
	int				siz;
	HBRUSH			brs;
	COLORREF		bcl=GetSysColor(COLOR_WINDOW), fcl=GetSysColor(COLOR_WINDOWTEXT);
	
	::SelectObject( dis->hDC, m_Font.m_hObject );
	::SetBkMode( dis->hDC, TRANSPARENT );
	::SetTextColor( dis->hDC, m_fgColor );

	siz = GetTextLen( dis->itemID );
	if ( txt = (char *)malloc( siz+1 ) ){
		GetText( dis->itemID, txt );
	}
	else{
		txt = "";
	}
	if ( dis->itemData ){
		bcl = RGB(255,0,0);
		fcl = RGB(255,255,255);
	}
	brs = ::CreateSolidBrush( bcl );
	::FillRect( dis->hDC, &dis->rcItem, brs );
	::DeleteObject( brs );
	::SetTextColor( dis->hDC, fcl );
	::DrawText( dis->hDC, txt, -1, &dis->rcItem, DT_LEFT|DT_SINGLELINE|DT_VCENTER );
	if ( txt )	free( txt );
}

