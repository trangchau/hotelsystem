// CustomListBox.cpp : インプリメンテーション ファイル
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
	// デフォルトのGUIフォントを取得
	HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	::GetObject( hFont, sizeof( LOGFONT), &m_LogFont);
	strcpy_s(m_LogFont.lfFaceName, sizeof(m_LogFont.lfFaceName), "ＭＳ ゴシック");				// フォント変更
	// フォントを作成
	m_Font.CreateFontIndirect( &m_LogFont);
	// デフォルトの文字表示の色を取得
	m_fgColor = ::GetSysColor( COLOR_WINDOWTEXT );
	// 背景のブラシを作成
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
	// LOGFONT構造体にItalic属性を追加・削除
	m_LogFont.lfUnderline = bflg;

	// フォントを作成
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont);

	// ウィンドウを再描画
	return RedrawWindow();
}

BOOL CCustomListBox::SetTextItalic(BOOL bflg)
{
	// LOGFONT構造体にItalic属性を追加・削除
	m_LogFont.lfItalic = bflg;

	// フォントを作成
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont);

    // ウィンドウを再描画
	return RedrawWindow();
}

BOOL CCustomListBox::SetTextHeight(int height)
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

BOOL CCustomListBox::SetBackColor(COLORREF color)
{
	// 指定された色からブラシを作成
	if ( m_bgBrush )	::DeleteObject( m_bgBrush );
	m_bgBrush = ::CreateSolidBrush( color );

	// ウィンドウを再描画
	return RedrawWindow();
}

BOOL CCustomListBox::SetTextBold(BOOL bflg)
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

void CCustomListBox::ResetFont( HFONT hFont )
{
	::GetObject( hFont, sizeof( LOGFONT), &m_LogFont);
	// フォントを作成
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect( &m_LogFont);
}

BOOL CCustomListBox::SetForeColor(COLORREF color)
{
	// フォアカラーを設定
	m_fgColor = color;
	// ウィンドウを再描画
	return RedrawWindow();
}

void CCustomListBox::SetFontByHandle( HFONT hFont, bool rdw )
{
	::GetObject( hFont, sizeof( LOGFONT), &m_LogFont);
	if ( rdw )	RedrawWindow();
}

void CCustomListBox::InitSubclass(
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

BEGIN_MESSAGE_MAP(CCustomListBox, CListBox)
	//{{AFX_MSG_MAP(CCustomListBox)
//	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomListBox メッセージ ハンドラ

/*
HBRUSH CCustomListBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: この位置で DC のアトリビュートを変更してください
	
    if( nCtlColor == CTLCOLOR_LISTBOX){
		// フォントを設定
		pDC->SelectObject( &m_Font );
		// 背景は、透過
		pDC->SetBkMode( TRANSPARENT );
		// テキストの色を指定
		pDC->SetTextColor( m_fgColor );
    }

	// TODO: 親ハンドラを呼び出したくない場合はここでブラシのハンドルとして NULL 以外を返すようにしてください
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

