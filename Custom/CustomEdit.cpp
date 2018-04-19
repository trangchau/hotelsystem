// CustomEdit.cpp : �����t�@�C��
//

#include <afxwin.h>
#include "CustomEdit.h"

#define		wsizeof(x)	((x)*sizeof(WCHAR))
#define		memcnt(x)	((sizeof(x))/(sizeof(x[0])))	// �z����擾


// CCustomEdit

IMPLEMENT_DYNAMIC(CCustomEdit, CEdit)

CCustomEdit::CCustomEdit()
{
	HFONT hFont = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
	::GetObject( hFont, sizeof(LOGFONT), &m_LogFont );
	m_Font.CreateFontIndirect( &m_LogFont);
	m_ftColor = ::GetSysColor( COLOR_WINDOWTEXT );
	m_bgBrush = ::CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	m_UserData = 0;
	memset( m_Txt_dat, 0, sizeof(m_Txt_dat) );
}

CCustomEdit::~CCustomEdit()
{
	if ( m_bgBrush )	::DeleteObject( m_bgBrush );
	m_bgBrush = NULL;
	m_Font.DeleteObject();
}
void CCustomEdit::InitSubclass(unsigned int nID, CWnd * pParent)
{
	SubclassDlgItem( nID, pParent );
}

void CCustomEdit::SetBGColor(COLORREF bgColor)
{
	::DeleteObject( m_bgBrush );
	m_bgBrush = ::CreateSolidBrush( bgColor );
	InvalidateRect( NULL, TRUE );
}

void CCustomEdit::SetFont(  WCHAR *pName, int nSize, COLORREF nFontColor )
{
	m_LogFont.lfHeight = nSize;
	m_LogFont.lfWidth  = 0;
	m_LogFont.lfWeight = 400;
	wcscpy_s( m_LogFont.lfFaceName, memcnt(m_LogFont.lfFaceName), pName );
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont );
	CEdit::SetFont( &m_Font,TRUE ); 
	m_ftColor = nFontColor;
}

void CCustomEdit::SetColor( COLORREF nFontColor )
{
	m_ftColor = nFontColor;
	InvalidateRect( NULL, TRUE );
}

void CCustomEdit::SetUserData( DWORD nData )
{
	m_UserData = nData;
}


BEGIN_MESSAGE_MAP(CCustomEdit, CEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_KEYUP()
END_MESSAGE_MAP()



// CCustomEdit ���b�Z�[�W �n���h��



void CCustomEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	int		cid = ::GetWindowLong( m_hWnd, GWL_ID );

//	::SendMessage( GetParent()->m_hWnd,
//		WM_EDITNOTIFY, MAKEWPARAM(WM_KILLFOCUS,cid), 0 );
	GetWindowText( m_Txt_dat, memcnt(m_Txt_dat) );

	::PostMessage( GetParent()->m_hWnd,
		WM_EDITNOTIFY, MAKEWPARAM(WM_KILLFOCUS,cid), m_UserData );
}

HBRUSH CCustomEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	
    if( nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_STATIC ){
		pDC->SetBkMode( TRANSPARENT );
		pDC->SetBkColor( RGB(255,0,0) );
		pDC->SetTextColor( m_ftColor );
    }

	// TODO:  �e�̃n���h�����Ăяo���Ȃ��ꍇ�� NULL �ȊO�̃u���V��Ԃ��܂��B
	//return m_bgBrush;
	return( IsWindowEnabled() ? m_bgBrush: NULL );
}

void CCustomEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	int		cid = ::GetWindowLong( m_hWnd, GWL_ID );

	::PostMessage( GetParent()->m_hWnd, WM_EDITNOTIFY,
			MAKEWPARAM(EN_LBUTTONCLICK,cid), MAKELPARAM(point.x,point.y) );

	CEdit::OnRButtonUp(nFlags, point);
}

void CCustomEdit::OnRButtonUp(UINT nFlags, CPoint point)
{
	int		cid = ::GetWindowLong( m_hWnd, GWL_ID );

	::PostMessage( GetParent()->m_hWnd, WM_EDITNOTIFY,
			MAKEWPARAM(EN_RBUTTONCLICK,cid), MAKELPARAM(point.x,point.y) );
	CEdit::OnRButtonUp(nFlags, point);
}

void CCustomEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ( nChar==VK_RETURN ){
//		nChar = VK_TAB;
//		::PostMessage( GetParent()->m_hWnd, WM_KEYDOWN, VK_RETURN, nRepCnt );
		PostMessage( WM_KEYDOWN, VK_TAB, nRepCnt );
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCustomEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CCustomEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ( nChar==VK_RETURN ){
//		nChar = VK_TAB;
//		::PostMessage( GetParent()->m_hWnd, WM_KEYDOWN, VK_RETURN, nRepCnt );
		PostMessage( WM_KEYDOWN, VK_TAB, nRepCnt );
	}


	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}
