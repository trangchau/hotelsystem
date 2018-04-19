// CustomStatic.cpp : インプリメンテーション ファイル
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

	// デフォルトのGUIフォントを取得
	HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	::GetObject( hFont, sizeof( LOGFONT), &m_LogFont);
	//wcscpy_s(m_LogFont.lfFaceName, LF_FACESIZE, _T("MS UI Gothic"));				// フォント変更
	// フォントを作成
	m_Font.CreateFontIndirect( &m_LogFont);
	// デフォルトの文字表示の色を取得
	m_fgColor = ::GetSysColor( COLOR_WINDOWTEXT );
	// 背景のブラシを作成
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
	// LOGFONT構造体にItalic属性を追加・削除
	m_LogFont.lfUnderline = bflg;

	// フォントを作成
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont);

	// ウィンドウを再描画
	return RedrawWindow();
}

BOOL CCustomStatic::SetTextItalic(BOOL bflg)
{
	// LOGFONT構造体にItalic属性を追加・削除
	m_LogFont.lfItalic = bflg;

	// フォントを作成
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont);

    // ウィンドウを再描画
	return RedrawWindow();
}

BOOL CCustomStatic::SetTextHeight(int height)
{
	// LOGFONT構造体にItalic属性を追加・削除
	m_LogFont.lfHeight = height;
	m_LogFont.lfWidth  = 0;
	// フォントを作成
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont );
	// ウィンドウを再描画
	return RedrawWindow();
}

BOOL CCustomStatic::SetBackColor(COLORREF color)
{
	// 指定された色からブラシを作成
	if ( m_bgBrush )	::DeleteObject( m_bgBrush );
	m_bgBrush = ::CreateSolidBrush( color );

	// ウィンドウを再描画
	return RedrawWindow();
}

BOOL CCustomStatic::SetTextBold(BOOL bflg)
{
    // LOGFONT構造体にBOLD属性を追加・削除
    if( bflg )	m_LogFont.lfWeight = FW_BOLD;
    else		m_LogFont.lfWeight = FW_NORMAL;

	// フォントを作成
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont);

	// ウィンドウを再描画
	return RedrawWindow();
}

void CCustomStatic::ResetFont( HFONT hFont )
{
	::GetObject( hFont, sizeof( LOGFONT), &m_LogFont);
	// フォントを作成
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont);
}

BOOL CCustomStatic::SetForeColor(COLORREF color)
{
	// フォアカラーを設定
	m_fgColor = color;
	// ウィンドウを再描画
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
UINT		nID,					// ダイアログＩＤ
COLORREF	fcl,					// フォントカラー
COLORREF	bcl,					// フォントカラー
int			bld,					// ボールド有無
int			itl,					// イタリック有無
CWnd		*par )					// 親ウィンドウ
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
// CKosuStatic メッセージ ハンドラ

HBRUSH CCustomStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: この位置で DC のアトリビュートを変更してください
	
    if( nCtlColor == CTLCOLOR_STATIC){
		// フォントを設定
		//pDC->SelectObject( &m_Font );
		// 背景は、透過
		pDC->SetBkMode( TRANSPARENT );
		// テキストの色を指定
		pDC->SetTextColor( m_fgColor );
    }

	// TODO: 親ハンドラを呼び出したくない場合はここでブラシのハンドルとして NULL 以外を返すようにしてください
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
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CStatic::OnRButtonDown(nFlags, point);
}
