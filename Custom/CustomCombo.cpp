// CustomCombo.cpp : 実装ファイル
//

#include <afxwin.h>
#include "CustomCombo.h"

#define		wsizeof(x)	((x)*sizeof(WCHAR))

// CCustomCombo

IMPLEMENT_DYNAMIC(CCustomCombo, CComboBox)

CCustomCombo::CCustomCombo()
{
	HFONT hFont = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
	::GetObject( hFont, sizeof(LOGFONT), &m_LogFont );
	m_Font.CreateFontIndirect( &m_LogFont);
	m_ftColor = ::GetSysColor( COLOR_WINDOWTEXT );
	m_bgBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
}

CCustomCombo::~CCustomCombo()
{
	if ( m_bgBrush )	::DeleteObject( m_bgBrush );
	m_bgBrush = NULL;
	m_Font.DeleteObject();
}

void CCustomCombo::InitSubclass(unsigned int nID, CWnd * pParent)
{
	SubclassDlgItem( nID, pParent );
}

void CCustomCombo::SetBGColor(COLORREF bgColor)
{
	::DeleteObject( m_bgBrush );
	m_bgBrush = ::CreateSolidBrush( bgColor );
}

void CCustomCombo::SetColor( COLORREF nFontColor )
{
	m_ftColor = nFontColor;
	InvalidateRect( NULL, TRUE );
}

void CCustomCombo::SetFont(  WCHAR *pName, int nSize, COLORREF nFontColor )
{
	m_LogFont.lfHeight = nSize;
	m_LogFont.lfWidth  = 0;
	m_LogFont.lfWeight = 400;
	wcscpy_s( m_LogFont.lfFaceName, sizeof(m_LogFont.lfFaceName)/wsizeof(1), pName );
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont );
	CComboBox::SetFont( &m_Font,TRUE ); 
	m_ftColor = nFontColor;
}

BEGIN_MESSAGE_MAP(CCustomCombo, CComboBox)
	ON_WM_CTLCOLOR_REFLECT()
    ON_WM_CTLCOLOR()
	ON_WM_KILLFOCUS()
	ON_WM_RBUTTONUP()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, &CCustomCombo::OnCbnSelchange)
END_MESSAGE_MAP()



// CCustomCombo メッセージ ハンドラ


/*
HBRUSH CCustomCombo::CtlColor(CDC* pDC, UINT nCtlColor )
{
	
	if( nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_LISTBOX){
		pDC->SetBkMode( TRANSPARENT );
		pDC->SetTextColor( m_ftColor );
	}

	// TODO:  親のハンドラを呼び出せない場合は NULL 以外のブラシを返します。
	return	m_bgBrush;
	return NULL;
}
*/

void CCustomCombo::OnKillFocus(CWnd* pNewWnd)
{
	CComboBox::OnKillFocus(pNewWnd);

	int		cid = ::GetWindowLong( m_hWnd, GWL_ID );

	::SendMessage( GetParent()->m_hWnd,
		WM_COMBONOTIFY, MAKEWPARAM(WM_KILLFOCUS,cid), 0 );
}

void CCustomCombo::OnRButtonUp(UINT nFlags, CPoint point)
{
	int		cid = ::GetWindowLong( m_hWnd, GWL_ID );

	::SendMessage( GetParent()->m_hWnd, WM_COMBONOTIFY,
			MAKEWPARAM(CN_RBUTTONCLICK,cid), MAKELPARAM(point.x,point.y) );
	CComboBox::OnRButtonUp(nFlags, point);
}

void CCustomCombo::OnCbnSelchange()
{
	int		cid = ::GetWindowLong( m_hWnd, GWL_ID );

	::SendMessage( GetParent()->m_hWnd,
		WM_COMBONOTIFY, MAKEWPARAM(CN_CHANGED,cid), 0 );
}

BOOL CCustomCombo::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	dwStyle |= WS_VSCROLL;
	return CComboBox::Create(dwStyle, rect, pParentWnd, nID);
}

HBRUSH CCustomCombo::CtlColor(CDC* pDC, UINT nCtlColor)
{
    //pDC->SetTextColor( m_clrText );
    //pDC->SetBkColor( m_bgBrush );
	return m_bgBrush;
}


HBRUSH CCustomCombo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    //pDC->SetTextColor( m_clrText );
    //pDC->SetBkColor( m_bgBrush );
    return m_bgBrush;
}
