// StatusPanel.cpp : インプリメンテーション ファイル
//

#include <afxwin.h>
#include "StatusPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusPanel

CStatusPanel::CStatusPanel()
{
	m_Init = TRUE;
	m_UserData = 0;
	memset( m_FrameID, 0, sizeof(m_FrameID) );
	memset( m_StatusID, 0, sizeof(m_StatusID) );
	memset( m_Status, 0, sizeof(m_Status) );
}

CStatusPanel::~CStatusPanel()
{
}

BEGIN_MESSAGE_MAP(CStatusPanel, CWnd)
	//{{AFX_MSG_MAP(CStatusPanel)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStatusPanel オペレーション
void CStatusPanel::SetFrameID( UINT nID )
{
	BITMAP		Bitmap[1];

	m_FrameID[0] = nID;
	m_FrmBitmap->LoadBitmap( nID );
	m_FrmBitmap->GetObject( sizeof(BITMAPINFOHEADER), Bitmap );
	m_Width = Bitmap->bmWidth;
	m_Height = Bitmap->bmHeight;
	MoveWindow( m_Rect.left, m_Rect.top, Bitmap->bmWidth, Bitmap->bmHeight, TRUE );
	InvalidateRect( NULL, TRUE );
}


void CStatusPanel::SetStatusID( int idx, UINT nOFFID, UINT nONID, int sts )
{
	BITMAP		Bitmap[1];

	m_StatusID[idx][0] = nOFFID;
	m_StatusID[idx][1] = nONID;

	m_StsBitmap[idx][0].LoadBitmap( nOFFID );
	m_StsBitmap[idx][0].GetObject( sizeof(BITMAPINFOHEADER), Bitmap );
	m_BmpSize[idx].x = Bitmap->bmWidth;
	m_BmpSize[idx].y = Bitmap->bmHeight;
	m_StsBitmap[idx][1].LoadBitmap( nONID );
	m_StsBitmap[idx][1].GetObject( sizeof(BITMAPINFOHEADER), Bitmap );
	m_Status[idx] = sts;
	InvalidateRect( NULL, TRUE );
}

void CStatusPanel::UpdteStatus( int idx, int sts )
{
	RECT		rct[1];
	int			i;

	if ( m_Status[idx] != sts ){
		m_Status[idx] = sts ;
		rct->left = 8;
		rct->top = 5;
		for ( i=0; i<idx; i++ )	rct->left += m_BmpSize[i].x;
		rct->right = rct->left + m_BmpSize[idx].x;
		rct->bottom = rct->top + m_BmpSize[idx].y;
		InvalidateRect( rct, TRUE );
	}
}


/////////////////////////////////////////////////////////////////////////////
// CStatusPanel メッセージ ハンドラ

BOOL CStatusPanel::Create(
const RECT	&rect,
CWnd		*pParentWnd,
UINT		nID )
{
	m_Rect = rect;
	return CWnd::Create( NULL, "",
			WS_CHILD|WS_VISIBLE, rect, pParentWnd, nID, NULL );
}

BOOL CStatusPanel::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	return CWnd::PreCreateWindow(cs);
}

void CStatusPanel::PostNcDestroy() 
{
	int			idx;

	m_FrmBitmap->DeleteObject();
	for ( idx=0; idx<3 && m_StatusID[idx][0]; idx++ ){
		m_StsBitmap[idx][0].DeleteObject();
		m_StsBitmap[idx][1].DeleteObject();
	}
	m_BitMask.DeleteObject();
	CWnd::PostNcDestroy();
}

/*
void CStatusPanel::SetBitmap(				// ビットマップの設定
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

//	mdcImg.CreateCompatibleDC(NULL);
//	mdcMask.CreateCompatibleDC(NULL);
//	mdcImg.SelectObject( &m_Bitmap );
//	mdcMask.SelectObject( &m_BitMask );

//	col = mdcImg.GetPixel( 0, 0 );					//ｲﾒｰｼﾞの左上隅の点の色を取得
//	mdcImg.SetBkColor( col );						//便宜的にその色を背景色（透過色）とする
//	mdcMask.BitBlt( 0, 0, m_Width, m_Height, &mdcImg, 0, 0, SRCCOPY );  
//	mdcImg.SetBkColor( RGB( 0, 0, 0 ) );			//背景色を黒に
//	mdcImg.SetTextColor( RGB( 255, 255, 255 ) );	//前景色を白に
//	mdcImg.BitBlt( 0, 0, m_Width, m_Height, &mdcMask, 0, 0, SRCAND );
//	mdcImg.DeleteDC();
//	mdcMask.DeleteDC();

	rct->left = rct->top = 0;
	rct->right = m_Width - 1;
	rct->bottom = m_Height - 1;
//	m_Rect.right = m_Rect.left + m_Width - 1;
//	m_Rect.bottom = m_Rect.top + m_Height - 1;
	MoveWindow( m_Rect.left, m_Rect.top, m_Width, m_Height, TRUE );
	InvalidateRect( rct, TRUE );
}
*/

void CStatusPanel::OnPaint() 
{
	CPaintDC	dc(this); // 描画用のデバイス コンテキスト
	CDC			sDC;
	CGdiObject	*pBmp;
	int			px=8, py=5, idx;
		
	sDC.CreateCompatibleDC( &dc );
	dc.SetBkColor( RGB( 255, 255, 255 ) );
	dc.SetTextColor( RGB( 0, 0, 0 ) );
//	pBmp = sDC.SelectObject( &m_BitMask );
//	dc.BitBlt( 0, 0, m_Width, m_Height, &sDC, 0, 0, SRCAND );
//	sDC.SelectObject( &m_Bitmap );
//	dc.BitBlt( 0, 0, m_Width, m_Height, &sDC, 0, 0, SRCPAINT );
	pBmp = sDC.SelectObject( m_FrmBitmap );
	dc.BitBlt( 0, 0, m_Width, m_Height, &sDC, 0, 0, SRCCOPY );
	for ( idx=0; m_StatusID[idx][0] && idx<3; idx++ ){
		sDC.SelectObject( m_StsBitmap[idx][m_Status[idx]]  );
		dc.BitBlt( px, py, m_BmpSize[idx].x, m_BmpSize[idx].y, &sDC, 0, 0, SRCCOPY );
		px += m_BmpSize[idx].x;
	}
	sDC.SelectObject( pBmp );
	sDC.DeleteDC();
	
	// 描画用メッセージとして CWnd::OnPaint() を呼び出してはいけません
}
