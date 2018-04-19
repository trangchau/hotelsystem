// CustomStatic.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include <afxwin.h>
#include "CustomStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKosuStatic

CCustomStatic::CCustomStatic()
{

	// �f�t�H���g��GUI�t�H���g���擾
	HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	::GetObject( hFont, sizeof( LOGFONT), &m_LogFont);
	//wcscpy_s(m_LogFont.lfFaceName, LF_FACESIZE, _T("MS UI Gothic"));				// �t�H���g�ύX
	// �t�H���g���쐬
	m_Font.CreateFontIndirect( &m_LogFont);
	// �f�t�H���g�̕����\���̐F���擾
	m_fgColor = ::GetSysColor( COLOR_WINDOWTEXT );
	// �w�i�̃u���V���쐬
	m_bgBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	AutoSize = FALSE;
}

CCustomStatic::~CCustomStatic()
{
	if ( m_bgBrush )	::DeleteObject( m_bgBrush );
	m_bgBrush = NULL;
	m_Font.DeleteObject();
}

BOOL CCustomStatic::SetTextUnderLine(BOOL bflg)
{
	// LOGFONT�\���̂�Italic������ǉ��E�폜
	m_LogFont.lfUnderline = bflg;

	// �t�H���g���쐬
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont);

	// �E�B���h�E���ĕ`��
	return RedrawWindow();
}

BOOL CCustomStatic::SetTextItalic(BOOL bflg)
{
	// LOGFONT�\���̂�Italic������ǉ��E�폜
	m_LogFont.lfItalic = bflg;

	// �t�H���g���쐬
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont);

    // �E�B���h�E���ĕ`��
	return RedrawWindow();
}

BOOL CCustomStatic::SetTextHeight(int height)
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

BOOL CCustomStatic::SetBackColor(COLORREF color)
{
	// �w�肳�ꂽ�F����u���V���쐬
	if ( m_bgBrush )	::DeleteObject( m_bgBrush );
	m_bgBrush = ::CreateSolidBrush( color );

	// �E�B���h�E���ĕ`��
	return RedrawWindow();
}

BOOL CCustomStatic::SetTextBold(BOOL bflg)
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

void CCustomStatic::ResetFont( HFONT hFont )
{
	::GetObject( hFont, sizeof( LOGFONT), &m_LogFont);
	// �t�H���g���쐬
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont);
}

BOOL CCustomStatic::SetForeColor(COLORREF color)
{
	// �t�H�A�J���[��ݒ�
	m_fgColor = color;
	// �E�B���h�E���ĕ`��
	return RedrawWindow();
}

void CCustomStatic::SetBGColor(COLORREF bgColor)
{
	::DeleteObject( m_bgBrush );
	m_bgBrush = ::CreateSolidBrush( bgColor );
	InvalidateRect( NULL, TRUE );
}

void CCustomStatic::SetFTColor(COLORREF fgColor)
{
	m_fgColor = fgColor;
	InvalidateRect( NULL, TRUE );
}

void CCustomStatic::SetFont(  WCHAR *pName, int nSize, COLORREF nFontColor )
{
	m_LogFont.lfHeight = nSize;
	m_LogFont.lfWidth  = 0;
	m_LogFont.lfWeight = 700;
	wcscpy_s( m_LogFont.lfFaceName, sizeof(m_LogFont.lfFaceName)/wsizeof(1), pName );
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont );
	CStatic::SetFont( &m_Font ); 
	m_fgColor = nFontColor;
}

void CCustomStatic::SetText(  WCHAR *pText )
{
	SIZE		siz[1];
	HGDIOBJ		obj;
	RECT		rct[1];
	CDC			*pDC = GetDC();

	SetWindowText( pText );
	if ( AutoSize ){
		GetClientRect( rct );
		obj = pDC->SelectObject( m_Font );
		GetTextExtentPoint32( pDC->m_hDC, pText, (int)wcslen(pText), siz );
		pDC->SelectObject( obj );
		SetWindowPos( NULL, 0, 0, siz->cx, rct->bottom, SWP_NOMOVE|SWP_NOZORDER	);
		ReleaseDC( pDC );
	}
}

void CCustomStatic::InitSubclass(
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

BEGIN_MESSAGE_MAP(CCustomStatic, CStatic)
	//{{AFX_MSG_MAP(CCustomStatic)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKosuStatic ���b�Z�[�W �n���h��

HBRUSH CCustomStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: ���̈ʒu�� DC �̃A�g���r���[�g��ύX���Ă�������
	
    if( nCtlColor == CTLCOLOR_STATIC){
		// �t�H���g��ݒ�
		//pDC->SelectObject( &m_Font );
		// �w�i�́A����
		pDC->SetBkMode( TRANSPARENT );
		// �e�L�X�g�̐F���w��
		pDC->SetTextColor( m_fgColor );
    }

	// TODO: �e�n���h�����Ăяo�������Ȃ��ꍇ�͂����Ńu���V�̃n���h���Ƃ��� NULL �ȊO��Ԃ��悤�ɂ��Ă�������
	if ( !!m_bgBrush )	return	m_bgBrush;
	return NULL;
}

void CCustomStatic::OnRButtonUp(UINT nFlags, CPoint point)
{
	int		cid = ::GetWindowLong( m_hWnd, GWL_ID );

	::PostMessage( GetParent()->m_hWnd, WM_STATICNOTIFY,
			MAKEWPARAM(SN_RBUTTONCLICK,cid), MAKELPARAM(point.x,point.y) );

	CStatic::OnRButtonUp(nFlags, point);
}

void CCustomStatic::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CStatic::OnRButtonDown(nFlags, point);
}
