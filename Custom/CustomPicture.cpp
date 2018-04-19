// Picture.cpp : インプリメンテーション ファイル
//

#include <afxwin.h>
#include "CustomPicture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomPicture

CCustomPicture::CCustomPicture()
{
	m_Init = TRUE;
	m_UserData = 0;
}

CCustomPicture::~CCustomPicture()
{
}


BEGIN_MESSAGE_MAP(CCustomPicture, CWnd)
	//{{AFX_MSG_MAP(CPicture)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPicture メッセージ ハンドラ

BOOL CCustomPicture::Create(
const RECT	&rect,
CWnd		*pParentWnd,
UINT		nID )
{
	m_Rect = rect;
	return CWnd::Create( NULL, "",
			WS_CHILD|WS_VISIBLE, rect, pParentWnd, nID, NULL );
}

BOOL CCustomPicture::PreCreateWindow(CREATESTRUCT& cs) 
{
//	cs.style |= WS_TABSTOP | 0x200;
//	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	return CWnd::PreCreateWindow(cs);
}

void CCustomPicture::SetBitmap(				// ビットマップの設定
UINT	nIDResource )					// リソース
{
	BITMAP		Bitmap[1];
	CDC			mdcImg, mdcMask;
	RECT		rct[1];

	m_Bitmap.DeleteObject();
	m_BitMask.DeleteObject();
	m_Bitmap.LoadBitmap( nIDResource );

	m_Bitmap.GetObject( sizeof(BITMAPINFOHEADER), Bitmap );
	m_Width = Bitmap->bmWidth;
	m_Height = Bitmap->bmHeight;

	m_BitMask.CreateBitmap( m_Width, m_Height, 1,1, NULL);
/*
	mdcImg.CreateCompatibleDC(NULL);
	mdcMask.CreateCompatibleDC(NULL);
	mdcImg.SelectObject( &m_Bitmap );
	mdcMask.SelectObject( &m_BitMask );

	col = mdcImg.GetPixel( 0, 0 );					//ｲﾒｰｼﾞの左上隅の点の色を取得
	mdcImg.SetBkColor( col );						//便宜的にその色を背景色（透過色）とする
	mdcMask.BitBlt( 0, 0, m_Width, m_Height, &mdcImg, 0, 0, SRCCOPY );  
	mdcImg.SetBkColor( RGB( 0, 0, 0 ) );			//背景色を黒に
	mdcImg.SetTextColor( RGB( 255, 255, 255 ) );	//前景色を白に
	mdcImg.BitBlt( 0, 0, m_Width, m_Height, &mdcMask, 0, 0, SRCAND );
	mdcImg.DeleteDC();
	mdcMask.DeleteDC();
*/
	rct->left = rct->top = 0;
	rct->right = m_Width - 1;
	rct->bottom = m_Height - 1;
//	m_Rect.right = m_Rect.left + m_Width - 1;
//	m_Rect.bottom = m_Rect.top + m_Height - 1;
	MoveWindow( m_Rect.left, m_Rect.top, m_Width, m_Height, TRUE );
	InvalidateRect( rct, TRUE );
}

void CCustomPicture::PostNcDestroy() 
{
	m_Bitmap.DeleteObject();
	m_BitMask.DeleteObject();
	CWnd::PostNcDestroy();
}

void CCustomPicture::OnPaint() 
{
	CPaintDC	dc(this); // 描画用のデバイス コンテキスト
	CDC			sDC;
	CGdiObject	*pBmp;
		
	sDC.CreateCompatibleDC( &dc );
	dc.SetBkColor( RGB( 255, 255, 255 ) );
	dc.SetTextColor( RGB( 0, 0, 0 ) );
//	pBmp = sDC.SelectObject( &m_BitMask );
//	dc.BitBlt( 0, 0, m_Width, m_Height, &sDC, 0, 0, SRCAND );
//	sDC.SelectObject( &m_Bitmap );
//	dc.BitBlt( 0, 0, m_Width, m_Height, &sDC, 0, 0, SRCPAINT );
	pBmp = sDC.SelectObject( &m_Bitmap );
	dc.BitBlt( 0, 0, m_Width, m_Height, &sDC, 0, 0, SRCCOPY );
	sDC.SelectObject( pBmp );
	sDC.DeleteDC();
	
	// 描画用メッセージとして CWnd::OnPaint() を呼び出してはいけません
}
