// Calendar.cpp : インプリメンテーション ファイル
//

#include "afxwin.h"
#include "Calendar.h"
#include <math.h>
#include "..\Common\bsc_def.h"
#include "..\Common\dte_mng.h"

//#define	memcnt(x)		(sizeof(x)/sizeof((x)[9]))

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ID_TFLEFT		1024
#define	ID_TFRIGHT		1025

#define	HRZ_MGN			4			// 水平方向マージン
#define	HDR_HGT			28			// ヘッダー高さ

#define	TMR_INI			1024		// 初期化タイマー


/////////////////////////////////////////////////////////////////////////////
// CCalendar

CCalendar::CCalendar( CWnd *pParent )
{
	m_pParent = pParent;

	m_iLanguage = CLG_JAPANESE;
	m_hFont = NULL;
	m_bFirst = TRUE;
	m_bDrag = FALSE;
	m_nDragCont = -1;
	m_hLeftButton = m_hRightButton = NULL;
	m_iPrvCX = m_iPrvCY = -1;
	m_iSelWeek = -1;
	m_hButtonFont = NULL;
	m_iSelMode = UCN_MOD_NONE;
	m_iResvYear = m_iResvMonth = m_iResvDay = 0;
	m_iLimitMinYear = 0;
	m_iLimitMinMonth = 0;
	m_iLimitMaxYear = 9999;
	m_iLimitMaxMonth = 99;
	memset( m_Wnd_data, 0, sizeof(m_Wnd_data) );

	m_bShowButtons = FALSE;

	memset( m_cSelDays, 0, sizeof(m_cSelDays) );

	m_lpClassName = (WCHAR *)AfxRegisterWndClass( CS_DBLCLKS, 
		LoadCursor( NULL, IDC_ARROW ), (HBRUSH)::GetStockObject(WHITE_BRUSH), NULL );
}

CCalendar::~CCalendar()
{
}


BEGIN_MESSAGE_MAP(CCalendar, CWnd)
	//{{AFX_MSG_MAP(CCalendar)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


static	int		cnv_pxl_(			// ポイント数→ピクセル数変換
HDC			hdc,					// 対象デバイスコンテクスト
int			pnt )					// 指定ポイント数
{
	double		dpi = (double)GetDeviceCaps(hdc, LOGPIXELSX);
	return( (int)(dpi / 72.0 * (double)pnt) );
}

static	int		cnv_pnt_(			// ピクセル数→ポイント数変換
HDC			hdc,					// 対象デバイスコンテクスト
int			pxl )					// 指定ピクセル数
{
	double		dpi = (double)GetDeviceCaps(hdc, LOGPIXELSX);
	return( (int)((double)pxl / (dpi / 72.0) ) );
}

static HFONT			Cre_Fnt(			// 文字列用フォント作成
WCHAR		*fnm,					// フォント名
int			siz,					// フォントサイズ（ポイント）
int			bld,					// ボールド指定
int			itr,					// イタリック
HDC			hdc )					// 作成対象デバイスコンテクスト
{
	HFONT				fnh;			// フォントハンドル
	LOGFONT				fnt[1];		   	// フォント情報構造体

	memset( fnt, 0, sizeof(fnt) );
	fnt->lfCharSet = DEFAULT_CHARSET;		// SHIFTJIS_CHARSET;
	wcscpy_s( fnt->lfFaceName, memcnt(fnt->lfFaceName), fnm );			// フォント名
	fnt->lfHeight = cnv_pxl_(hdc,siz) * -1;	// フォントサイズ
	fnt->lfWidth = cnv_pxl_(hdc,siz) / 2;	// フォントサイズ
	fnt->lfWeight = bld ? 700: 400;			// スタイル～ボールド（400,700)
	fnt->lfQuality = PROOF_QUALITY;			// 文字品質
	fnh = CreateFontIndirect( fnt );		// フォント作成

	return( fnh );
}

static int				Get_Fsz(			// 文字サイズ（水平ピクセル数取得）
int			pnt,					// ポイント数
HDC			hdc )					// 計算対象デバイスコンテクスト
{
	return( cnv_pxl_(hdc,pnt) / 2 );
}

#define	is_leap(x)	(((x)%400)==0||(((x)%4)==0&&((x)%100)!=0))

static	int		clc_dow(		// 曜日計算
int		yer,					// 年
int		mth,					// 月
int		day )					// 日
{
	int			six;

    if ( mth == 2)				day += 31;
    if ( mth == 1 || mth == 2 )	yer--, mth=13;

	six = (int)(floor((double)yer*1.25)) - (int)(floor((double)yer/100.0))
		+ (int)(floor((double)yer/400.0)) + (int)(floor((double)mth*2.6+1.6)) + day;

    return( six % 7 );
}

static	int		inr_rct(		// 矩形内検定
int			mx,
int			my,
RECT		*rct )
{
	return( mx>=rct->left && mx<=rct->right && my>=rct->top && my<=rct->bottom );
}

static	void	drw_bfs(					// ボタン表面描画
HDC				hdc,						// デバイスコンテクスト
int				sts,						// ボタン状態
HWND			wnd )						// ウィンドウハンドル
{
	RECT		rct[1];
	HPEN		pen, opn;
	HBRUSH		brs, obs;
	POINT		pnt[3];
	int			id = GetWindowLong( wnd, GWL_ID );

	GetClientRect( wnd, rct );
	pnt->x = id==ID_TFLEFT ? rct->left+4: rct->right-4;
	pnt->y = rct->bottom / 2;
	(pnt+1)->x = id==ID_TFLEFT ? rct->right-6: rct->left+6;
	(pnt+1)->y = rct->top+4;
	(pnt+2)->x = (pnt+1)->x;
	(pnt+2)->y = rct->bottom-4;

	brs = CreateSolidBrush( sts ? RGB(77,59,113): RGB(134,118,227) );
	pen = CreatePen( PS_SOLID, 1, RGB(134,118,277) );
	opn = (HPEN)::SelectObject( hdc, pen );
	obs = (HBRUSH)::SelectObject( hdc, brs );
	Ellipse( hdc, 0, 0, rct->right, rct->bottom );
	SelectObject( hdc, opn );
	DeleteObject( pen );
	pen = CreatePen( PS_SOLID, 1, sts ? RGB(0,128,128): RGB(0,255,255) );
	SelectObject( hdc, pen );
	SelectObject( hdc, obs );
	DeleteObject( brs );
	brs = CreateSolidBrush( sts ? RGB(0,128,128): RGB(0,255,255) );
	SelectObject( hdc, brs );
	Polygon( hdc, pnt, 3 );
	SelectObject( hdc, opn );
	SelectObject( hdc, obs );
	DeleteObject( brs );
	DeleteObject( pen );
}

static	void	snd_cmd(					// 親ウィンドウへコマンド送信
HWND		wnd,							// ウィンドハンドル
UINT		msg )							// メッセージコード
{
	HWND		par = GetParent( wnd );
	WPARAM		wpr;
	int			id = GetWindowLong( wnd, GWL_ID );

	wpr = MAKEWPARAM( id, msg );
	PostMessage( par, WM_COMMAND, wpr, (LPARAM)wnd );
}

static	int		inr_ccl(					// 点が円の中に包含されるか検定
int			px,								// 点のＸ座標
int			py,								// 点のＹ座標
int			rad )							// 円の半径
{
	double		fx=(double)(px-rad), fy=(double)(py-rad);

	return( sqrt( fx*fx + fy*fy ) <= (double)rad );
}

static	LRESULT CALLBACK btn_wpc(			// ボタンのプロシージャ
HWND		wnd,							// ウィンドハンドル
UINT		msg,							// メッセージコード
DWORD		wpr,							// wParam
DWORD		lpr )							// lParam
{
	HDC			hdc;
	RECT		rct[1];
	PAINTSTRUCT	ps[1];
	WND_DATA	*wdt = (WND_DATA *)((LONG_PTR)GetWindowLong( wnd, GWL_USERDATA ));
	int			def=TRUE, inr;

	switch( msg ){
	case WM_PAINT:			hdc = BeginPaint( wnd, ps );
							drw_bfs( hdc, wdt->lck ? 1: 0, wnd );
							EndPaint( wnd, ps );
							def = FALSE;
							break;
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:	GetClientRect( wnd, rct );
							if ( inr_ccl( LOWORD(lpr), HIWORD(lpr), rct->right/2 ) ){
								SetCapture( wnd );
								wdt->cap = TRUE;
								wdt->lck = msg!=WM_LBUTTONUP;
								InvalidateRect( wnd, NULL, FALSE );
							}
							def = FALSE;
							break;
	case WM_LBUTTONUP:		ReleaseCapture();
							if ( wdt->lck )	snd_cmd( wnd, msg );
							wdt->cap = FALSE;
							wdt->lck = FALSE;
							InvalidateRect( wnd, NULL, FALSE );
							def = FALSE;
							break;
	case WM_MOUSEMOVE:		GetClientRect( wnd, rct );
							if ( wdt->cap ){
								inr = inr_ccl( LOWORD(lpr), HIWORD(lpr), rct->right/2 );
								if ( inr != wdt->lck ){
									wdt->lck = inr;
									InvalidateRect( wnd, NULL, FALSE );
								}
							}
							def = FALSE;
							break;
	}

	return( def ? CallWindowProc( wdt->prv, wnd, msg, wpr, lpr ): FALSE );
}

/////////////////////////////////////////////////////////////////////////////
// CCalendar メソッド
void CCalendar::SetFont( WCHAR *pFontName, BOOL iRedraw )
{
	HFONT		fnt;
	LOGFONT		lgf[1];

	wcsncpy_s( m_pFontName, LF_FACESIZE, pFontName, LF_FACESIZE );

	memset( lgf, 0, sizeof(lgf) );
	lgf->lfCharSet = DEFAULT_CHARSET;					// SHIFTJIS_CHARSET;
	wcscpy_s( lgf->lfFaceName, LF_FACESIZE, m_pFontName );				// フォント名
	lgf->lfHeight = (long)(( m_iHeight * 0.8 ) * -1);	// フォントサイズ
	lgf->lfWidth = 0;//(long)(( m_iHeight * 0.8 ) / 2);		// フォントサイズ
	lgf->lfWeight = 700;								// スタイル～ボールド（400,700)
	lgf->lfQuality = PROOF_QUALITY;						// 文字品質

	fnt = CreateFontIndirect( lgf );					// フォント作成

	if ( m_hFont )	::DeleteObject( m_hFont );
	m_hFont = fnt;
	if ( iRedraw )	InvalidateRect( NULL, TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// CCalendar メッセージ ハンドラ

BOOL CCalendar::Create( int iYear, int iMonth, RECT *rct, UINT nID, DWORD dwUser, int iSpecial ) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	DWORD	dwStyle = WS_CHILD|WS_VISIBLE;

	m_iYear = iYear;
	m_iMonth = iMonth;

	m_iWidth = ( rct->right - rct->left - 8 ) / ( ( iSpecial & UCN_CSP_WEKNUM ) ? 8: 7 );
	m_iHeight = ( rct->bottom - rct->top - HDR_HGT ) / 7;
	m_iSpecial = iSpecial;
	m_dwUser = dwUser;

	NeedHoliday();

	return CWnd::Create( m_lpClassName, _T(""), dwStyle, *rct, m_pParent, nID, NULL );
}

BOOL CCalendar::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	return CWnd::PreCreateWindow(cs);
}

void CCalendar::DrawTitle( HDC hDC )
{
	RECT		rct[1];
	HBRUSH		brs, obs;
	HPEN		pen, opn;
	HFONT		fnt, ofn;
	WCHAR		buf[32], *dow[7] = { _T("日"), _T("月"), _T("火"), _T("水"), _T("木"), _T("金"), _T("土") };
	WCHAR		*mth[] = { _T("Jan"),_T("Feb"),_T("Mar"),_T("Apl"),_T("May"),_T("Jun"),_T("Jul"),_T("Aug"),_T("Sep"),_T("Oct"),_T("Nov"),_T("Dec") };
	WCHAR		*dwe[7] = { _T("Sun"), _T("Mon"), _T("Tue"), _T("Wed"), _T("Thu"), _T("Fri"), _T("Sat") };
	int			i, sx, sy, ex, ey;
	SIZE		siz[1];

	// タイトル描画
	fnt = Cre_Fnt( m_pFontName, cnv_pnt_( hDC, HDR_HGT-6 ), FALSE, FALSE, hDC );
	ofn = (HFONT)::SelectObject( hDC, fnt );

	GetClientRect( rct );
	rct->bottom = HDR_HGT - 1;
	if ( m_iLanguage == CLG_JAPANESE ){
		swprintf_s( buf, memcnt(buf), _T("%04d年%d月"), m_iYear, m_iMonth );
	}
	else{
		swprintf_s( buf, memcnt(buf), _T("%s %04d"), mth[m_iMonth-1], m_iYear  );
	}
	GetTextExtentPoint32( hDC, buf, (int)wcslen(buf), siz );

	sx = ( rct->right - siz->cx ) / 2;
	ex = sx + siz->cx;
	sy = ( HDR_HGT - siz->cy ) / 2;
	ey = sy + siz->cy;

	brs = ::CreateSolidBrush( RGB(170,164,238) );
	::FillRect( hDC, rct, brs );
	::DeleteObject( brs );
	brs = ::CreateSolidBrush( RGB(134,118,227) );
	pen = ::CreatePen( PS_SOLID, 1, RGB(134,118,277) );
	opn = (HPEN)::SelectObject( hDC, pen );
	obs = (HBRUSH)::SelectObject( hDC, brs );
	RoundRect( hDC, sx-ey/2, sy, ex+ey/2, ey, ey, ey );
//	rad = HDR_HGT - 8;
//	Ellipse( hDC, 4, 4, 4+rad, 4+rad );


	::SelectObject( hDC, opn );
	::SelectObject( hDC, obs );
	::DeleteObject( brs );
	::DeleteObject( pen );

	::SetTextColor( hDC, RGB( 255,255,255 ) );
	::SetBkMode( hDC, TRANSPARENT );
	::DrawText( hDC, buf, (int)wcslen(buf), rct, DT_VCENTER|DT_CENTER|DT_SINGLELINE );
//	::MoveToEx( hDC, rct->left, m_iHeight * 2 - 1, NULL );
//	::LineTo( hDC, rct->right, m_iHeight * 2 - 1 );
	::SelectObject( hDC, ofn );
	::DeleteObject( fnt );
	if ( m_iSpecial & UCN_CSP_WEKNUM ){
		::MoveToEx( hDC, rct->left+m_iWidth, HDR_HGT - 1, NULL );
		::LineTo( hDC, rct->left+m_iWidth, m_iHeight * 8 );
	}
	rct->left = ( m_iSpecial & UCN_CSP_WEKNUM ) ? m_iWidth+HRZ_MGN: HRZ_MGN;
	rct->top = HDR_HGT;
	rct->right = rct->left + m_iWidth - 1;
	rct->bottom = rct->top + m_iHeight - 1;
	for ( i=0; i<7; i++ ){
		if ( !i )			::SetTextColor( hDC, RGB( 255, 0, 0 ) );
		else if ( i==6 )	::SetTextColor( hDC, RGB( 0, 0, 255 ) );
		else				::SetTextColor( hDC, RGB( 0, 0, 0 ) );
		::DrawText( hDC, m_iLanguage==CLG_JAPANESE ? dow[i]: dwe[i], 
			(int)wcslen(m_iLanguage==CLG_JAPANESE ? dow[i]: dwe[i]), rct, DT_VCENTER|DT_CENTER|DT_SINGLELINE );
		rct->left += m_iWidth;
		rct->right += m_iWidth;
	}
}

void CCalendar::DrawWeekTriangle( HDC hDC, int idx )
{
	POINT		pnt[3];
	HPEN		pen;
	HBRUSH		brs;
	HGDIOBJ		opn, obs;
	int			ofs;

//	ofs = m_iHeight * 2 + m_iHeight * idx;
	ofs = HDR_HGT + m_iHeight + m_iHeight * idx;
	
	pnt[0].x = pnt[2].x = m_iWidth / 6;
	pnt[1].x = ( m_iWidth / 6 ) * 5;
	pnt[0].y = m_iHeight / 6 + ofs;
	pnt[1].y = ( m_iHeight / 6 ) * 3 + ofs;
	pnt[2].y = ( m_iHeight / 6 ) * 5 + ofs;
	pen = CreatePen( PS_SOLID, 1, RGB(255,149,149) );
	brs = CreateSolidBrush( RGB(255,149,149) );
	opn = SelectObject( hDC, pen );
	obs = SelectObject( hDC, brs );
	SetPolyFillMode( hDC, ALTERNATE );
	Polygon( hDC, pnt, 3 );
	SelectObject( hDC, opn );
	SelectObject( hDC, obs );
}

void CCalendar::DrawCircle( HDC hDC, RECT *rct )
{
	HPEN		rpn;
	HBRUSH		rbs;
	HGDIOBJ		opn, obs;
	int			wid, hgt, rad, sx, sy, ex, ey;

	rpn = CreatePen( PS_SOLID, 1, RGB(255,149,149) );
	rbs = CreateSolidBrush( RGB(255,149,149) );

	wid = rct->right - rct->left + 1;
	hgt = rct->bottom - rct->top + 1;
	rad = wid > hgt ? hgt: wid;
	sx = rct->left + ( wid - rad ) / 2;
	sy = rct->top + ( hgt - rad ) / 2;
	ex = sx + rad;
	ey = sy + rad;
	opn = ::SelectObject( hDC, rpn );
	obs = ::SelectObject( hDC, rbs );
	//Arc(hDC, sx, sy, ex, ey, sx, sy, sx, sy);
	Ellipse( hDC, sx, sy, ex, ey );
	::SelectObject( hDC, opn );
	::SelectObject( hDC, obs );
	::DeleteObject( rpn );
	::DeleteObject( rbs );
}

void CCalendar::DrawDetail( HDC hDC )
{
	static int	mxt[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	int			mxd, idx, i, wek=1, day=1, yer, mth, ix=0, iy=0, cyr, cmn, cdy, mgn;
	WCHAR		buf[20];
	RECT		rct[1], wrc[1];
	HBRUSH		sbh=NULL, nbh=NULL, cbh=NULL, fbh=NULL, tbh=NULL;
	time_t		now;
//	struct tm	*nowtm;
	struct tm	nowtm[1];

	time( &now );

	localtime_s( nowtm, &now ); /* 現地時刻に変換 */
	cyr = nowtm->tm_year + 1900;
	cmn = nowtm->tm_mon + 1;
	cdy = nowtm->tm_mday;

	m_WeekCount = 1;
	memset( m_cDays, 0, sizeof(m_cDays) );

	if ( m_iMonth==2 )	mxd = is_leap(m_iYear) ? (short)29: (short)28;
	else				mxd = mxt[m_iMonth-1];

	sbh = CreateSolidBrush( RGB(0, 0, 128) );
	nbh = CreateSolidBrush( RGB(255, 255, 255) );
	fbh = CreateSolidBrush( RGB(0, 255, 0) );
	tbh = CreateSolidBrush( RGB(255, 255, 0) );
//	cbh = CreateSolidBrush( RGB(255, 255, 0) );

	mgn = ( m_iSpecial & UCN_CSP_WEKNUM ) ? m_iWidth+HRZ_MGN: HRZ_MGN;

	::SetBkMode( hDC, TRANSPARENT );
	idx = clc_dow( m_iYear, m_iMonth, 1 );
	if ( idx>0  && !(m_iSpecial&UCN_CSP_RIGHT) ){
		yer = m_iYear;
		mth = m_iMonth;
		if ( --mth == 0 ){
			mth = 12;
			yer--;
		}
		if ( mth==2 )	day = is_leap(mth) ? (short)29: (short)28;
		else			day = mxt[mth-1];
		rct->left = ( idx - 1 ) * m_iWidth + mgn + 1;
		rct->right = rct->left + m_iWidth;
		rct->top = HDR_HGT + m_iHeight;
		rct->bottom = rct->top + m_iHeight;
		for ( i=0; i<idx; i++ ){
			m_cDays[ix][iy].iYear = yer;
			m_cDays[ix][iy].iMonth = mth;
			m_cDays[ix][iy].iDay = day;
			m_cDays[ix][iy].iWeek = 0;
			m_cDays[ix][iy].iYear = 0;
			m_cDays[ix][iy].iMonth = 0;
			FillRect( hDC, rct, nbh );
			::SetTextColor( hDC, RGB( 192, 192, 192 ) );
//			if ( yer==m_iResvYear && mth==m_iResvMonth && m_iResvDay==day ){
			if ( yer==cyr && mth==cmn && day==cdy ){
				DrawCircle( hDC, rct );
			}
			swprintf_s( buf, memcnt(buf), _T("%d"), day-- );
			::DrawText( hDC, buf, (int)wcslen(buf), rct, DT_VCENTER|DT_CENTER|DT_SINGLELINE );
			rct->left -= m_iWidth;
			rct->right -= m_iWidth;
			ix++;
		}
	}
	else{
		ix = idx;
	}

	rct->left = idx * m_iWidth + mgn;
	rct->right = rct->left + m_iWidth;
	rct->top = HDR_HGT + m_iHeight;
	rct->bottom = rct->top + m_iHeight;
	wrc->left = 0;
	wrc->top = HDR_HGT + m_iHeight;
	wrc->right = m_iWidth;
	wrc->bottom = wrc->top + m_iHeight;
	if ( m_iSpecial & UCN_CSP_WEKNUM ){
		::FillRect( hDC, wrc, nbh );
		if ( m_iSelWeek==0 )	DrawWeekTriangle( hDC, 0 );
		swprintf_s( buf, sizeof(buf), m_iLanguage==CLG_JAPANESE ? _T("%d週"): _T("th"), wek++ );
		::SetTextColor( hDC, RGB( 0, 0, 0 ) );
		::DrawText( hDC, buf, (int)wcslen(buf), wrc, DT_VCENTER|DT_RIGHT|DT_SINGLELINE );
	}
	for ( i=0; i<mxd; i++ ){
		m_cDays[ix][iy].iYear = m_iYear;
		m_cDays[ix][iy].iMonth = m_iMonth;
		m_cDays[ix][iy].iDay = i+1;
		m_cDays[ix][iy].iWeek = m_WeekCount - 1;
		swprintf_s( buf, memcnt(buf), _T("%d"), i+1 );
		if ( m_cSelDays[ix][iy] ){
			//rct->bottom++;
			//rct->right++;
			::FillRect( hDC, rct, sbh );
			//rct->bottom--;
			//rct->right--;
			::SetTextColor( hDC, RGB( 255, 255, 255 ) );
		}
		else{
			//rct->bottom++;
			//rct->right++;
			if ( m_pReserve[i]==UCN_RSV_FIX ){
				FillRect( hDC, rct, fbh );
			}
			else if ( m_pReserve[i]==UCN_RSV_TMP ){
				FillRect( hDC, rct, tbh );
			}
			else{
				::FillRect( hDC, rct, nbh );
			}
			//rct->bottom--;
			//rct->right--;
/*
			if ( !idx )			::SetTextColor( hDC, RGB( 255, 0, 0 ) );
			else if ( idx==6 )	::SetTextColor( hDC, RGB( 0, 0, 255 ) );
			else if ( m_pHoliday[i]=='1' )
								::SetTextColor( hDC, RGB( 255, 0, 0 ) );
			else				::SetTextColor( hDC, RGB( 0, 0, 0 ) );
*/
//			if ( !idx ) {
//				::SetTextColor( hDC, RGB( 255, 0, 0 ) );
//			} else {
				if ( m_pHoliday[i]=='2' || m_pHoliday[i]=='3' ) {
					::SetTextColor( hDC, RGB( 255, 0, 0 ) );
				} else if ( m_pHoliday[i]=='1' ) {
					::SetTextColor( hDC, RGB( 0, 0, 255 ) );
				} else {
					::SetTextColor( hDC, RGB( 0, 0, 0 ) );
				}
//			}
		}
//		if ( m_iYear==m_iResvYear && m_iMonth==m_iResvMonth && m_iResvDay==i+1 ){
		if ( m_iYear==cyr && m_iMonth==cmn && i+1==cdy ){
			DrawCircle( hDC, rct );
		}
		::DrawText( hDC, buf, (int)wcslen(buf), rct, DT_VCENTER|DT_CENTER|DT_SINGLELINE );
		if ( idx==6 ){
			ix = 0, iy++;
			idx = 0;
			rct->left = idx * m_iWidth + mgn;
			rct->right = rct->left + m_iWidth;
			rct->top += m_iHeight;
			rct->bottom += m_iHeight;
			if ( i<mxd-1 ){
				++m_WeekCount;
				if ( m_iSpecial & UCN_CSP_WEKNUM ){
					swprintf_s( buf, memcnt(buf), m_iLanguage==CLG_JAPANESE ? _T("%d週"): _T("th"), wek++ );
					::SetTextColor( hDC, RGB( 0, 0, 0 ) );
					wrc->top += m_iHeight;
					wrc->bottom += m_iHeight;
					::FillRect( hDC, wrc, nbh );
					if ( m_iSelWeek==iy )	DrawWeekTriangle( hDC, iy );
					::DrawText( hDC, buf, (int)wcslen(buf), wrc, DT_VCENTER|DT_RIGHT|DT_SINGLELINE );
				}
			}
		}
		else{
			idx++;
			ix++;
			rct->left += m_iWidth;
			rct->right += m_iWidth;
		}
	}
	if ( idx && !(m_iSpecial&UCN_CSP_LEFT) ){
		for ( i=0; idx<7; idx++ ){
			m_cDays[ix][iy].iYear = m_iYear;
			m_cDays[ix][iy].iMonth = m_iMonth;
			if ( ++m_cDays[ix][iy].iMonth > 12 ){
				m_cDays[ix][iy].iMonth = 1;
				++m_cDays[ix][iy].iYear;
			}
			yer = m_cDays[ix][iy].iYear;
			mth = m_cDays[ix][iy].iMonth;
			m_cDays[ix][iy].iDay = i+1;
			m_cDays[ix][iy].iYear = 0;
			m_cDays[ix][iy].iMonth = 0;
			m_cDays[ix][iy].iWeek = m_WeekCount - 1;
			FillRect( hDC, rct, nbh );
			::SetTextColor( hDC, RGB( 192, 192, 192 ) );
//			if ( yer==m_iResvYear && mth==m_iResvMonth && m_iResvDay==i+1 ){
			if ( yer==cyr && mth==cmn && i+1==cdy ){
				DrawCircle( hDC, rct );
			}
			swprintf_s( buf, memcnt(buf), _T("%d"), ++i );
			::DrawText( hDC, buf, (int)wcslen(buf), rct, DT_VCENTER|DT_CENTER|DT_SINGLELINE );
			rct->left += m_iWidth;
			rct->right += m_iWidth;
			ix++;
		}
	}
	GetClientRect( rct );
	if ( !!(m_iSpecial&UCN_CSP_RIGHT) ){
		::MoveToEx( hDC, rct->left+1, rct->top, NULL );
		::LineTo( hDC, rct->left+1, rct->bottom );
	}
	if ( !!(m_iSpecial&UCN_CSP_LEFT) ){
		::MoveToEx( hDC, rct->right-1, rct->top, NULL );
		::LineTo( hDC, rct->right-1, rct->bottom );
	}

	::DeleteObject( sbh );
	::DeleteObject( nbh );
	::DeleteObject( fbh );
	::DeleteObject( tbh );
}

void CCalendar::OnPaint() 
{
	HGDIOBJ			ofn;

	if ( m_bFirst ){
		m_bFirst = FALSE;
		SetTimer( TMR_INI, 1, NULL );
	}

	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	
	if ( m_hFont )	ofn = ::SelectObject( dc.m_hDC, m_hFont );
	// タイトル描画
	DrawTitle( dc.m_hDC );
	DrawDetail( dc.m_hDC );

	if ( m_hFont )	::SelectObject( dc.m_hDC, ofn );
	// 描画用メッセージとして CWnd::OnPaint() を呼び出してはいけません
}

void CCalendar::PostNcDestroy() 
{
	::SendMessage( m_hLeftButton, WM_CLOSE, 0, 0 );
	::SendMessage( m_hRightButton, WM_CLOSE, 0, 0 );
	if ( m_hFont )	::DeleteObject( m_hFont );
	if ( m_hButtonFont )	::DeleteObject( m_hButtonFont );
	m_hFont = m_hButtonFont = NULL;
	
	CWnd::PostNcDestroy();
}

////////////////////////////////////////////////////////////////////////
//	Ｎｏｔｉｆｙ関係
	
void CCalendar::SetNtfyData( TFCLNDRNTFY *ntf )
{
	memset( ntf, 0, sizeof(TFCLNDRNTFY) );

	ntf->iYear = m_iYear;
	ntf->iMonth = m_iMonth;
	ntf->pHoliday = m_pHoliday;
	ntf->pReserve = m_pReserve;
	ntf->iSpecial = m_iSpecial;
	ntf->dwUser = m_dwUser;
}

void CCalendar::NeedHoliday()
{
	TFCLNDRNTFY		ntf[1];

	SetNtfyData( ntf );
	ntf->iDay = 1;
	::SendMessage( m_pParent->m_hWnd, WM_USER_Calendar_NOTIFY, UCN_NEEDHOLIDAY, (LPARAM)ntf );
}

void CCalendar::NeedReserve()
{
	TFCLNDRNTFY		ntf[1];

	SetNtfyData( ntf );
	ntf->iDay = 1;
	::SendMessage( m_pParent->m_hWnd, WM_USER_Calendar_NOTIFY, UCN_NEEDRESERVE, (LPARAM)ntf );
}

void CCalendar::NeedSelect()
{
	TFCLNDRNTFY		ntf[1];
	char			sel[60];
	int				ix, iy, idx;
	Day				dte[1];

	SetNtfyData( ntf );
	ntf->iDay = 1;
	ntf->pSelect = sel;
	::SendMessage( m_pParent->m_hWnd, WM_USER_Calendar_NOTIFY, UCN_NEEDSELDAYS, (LPARAM)ntf );
	if ( m_iSelMode == UCN_MOD_SINGLE ){
		if ( ntf->iYear==m_iYear && ntf->iMonth==m_iMonth ){
			dte->yer = ntf->iYear;
			dte->mth = ntf->iMonth;
			dte->day = 1;
			idx = Get_Dow( dte );
			sel[idx+ntf->iDay-1] = '1';
		}
	}
	for ( iy=0; iy<6; iy++ ){
		for ( ix=0; ix<7; ix++ ){
			m_cSelDays[ix][iy] = sel[iy*7+ix]==_T('1');
			if ( sel[iy*7+ix]=='1' && m_iSelMode==UCN_MOD_SINGLE ){
				m_iPrvCX = ix;
				m_iPrvCY = iy;
				iy = 99;
				break;
			}
		}
	}
	InvalidateRect( NULL, TRUE );
}

void CCalendar::OnTimer(UINT nIDEvent) 
{
	HINSTANCE		ins;
	int				rad, lft;
	RECT			rct[1];

	KillTimer( nIDEvent );

	GetClientRect( rct );
	ins = AfxGetApp()->m_hInstance;

	rad = HDR_HGT - HRZ_MGN * 2;
	if ( !(m_iSpecial&UCN_CSP_RIGHT) ){	// 右仕様でなければ左ボタン描画
		m_hLeftButton = CreateWindow( _T("BUTTON"), _T(""), WS_CHILD|WS_VISIBLE, 
			HRZ_MGN, HRZ_MGN, rad, rad, m_hWnd, (HMENU)ID_TFLEFT, ins, 0 );//ID_TFLEFT, ins );
		::SendMessage( m_hLeftButton, WM_SETFONT, 0, TRUE);
		m_Wnd_data->prv = (WNDPROC)(LONG_PTR)::SetWindowLong( m_hLeftButton, GWL_WNDPROC, (LONG)(LONG_PTR)btn_wpc );	
		::SetWindowLong( m_hLeftButton, GWL_USERDATA, (LONG)(LONG_PTR)m_Wnd_data );	
	}
	if ( !(m_iSpecial&UCN_CSP_LEFT) ){	// 左仕様でなければ右ボタン描画
		lft =  rct->right - ( rad + HRZ_MGN );
		m_hRightButton = CreateWindow( _T("BUTTON"), _T(""), WS_CHILD|WS_VISIBLE, 
			lft, HRZ_MGN, rad, rad, m_hWnd, (HMENU)ID_TFRIGHT, ins, 0 );//ID_TFLEFT, ins );
		::SendMessage( m_hRightButton, WM_SETFONT, 0, TRUE);
		(m_Wnd_data+1)->prv = (WNDPROC)(LONG_PTR)::SetWindowLong( m_hRightButton, GWL_WNDPROC, (LONG)(LONG_PTR)btn_wpc );	
		::SetWindowLong( m_hRightButton, GWL_USERDATA, (LONG)(LONG_PTR)(m_Wnd_data+1) );	
	}

	NeedHoliday();
	NeedReserve();
	CWnd::OnTimer(nIDEvent);
}

BOOL CCalendar::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	WORD		msg = HIWORD( wParam );

	switch( LOWORD(wParam) ){
	case ID_TFLEFT:	decrement();	break;
	case ID_TFRIGHT:increment();	break;
	}
	return CWnd::OnCommand(wParam, lParam);
}

////////////////////////////////////////////////////////////////////////
//	メソッド関係

void CCalendar::SetLanguage( int iLanguage )
{
	if ( iLanguage != m_iLanguage ){
		m_iLanguage = iLanguage;
		InvalidateRect( NULL, TRUE );
	}
}

void CCalendar::SetReserveDate( int iYear, int iMonth, int iDay )
{
	m_iResvYear = iYear;
	m_iResvMonth = iMonth;
	m_iResvDay = iDay;
	if ( iYear == m_iYear && iMonth==m_iMonth ){
		InvalidateRect( NULL, TRUE );
	}
}

void CCalendar::SetLimit( int iMinYear, int iMinMonth, int iMaxYear, int iMaxMonth )
{
	m_iLimitMaxYear = iMaxYear;			// 最大年月
	m_iLimitMaxMonth = iMaxMonth;		// 最大年月
	m_iLimitMinYear = iMinYear;			// 最小年月
	m_iLimitMinMonth= iMinMonth;		// 最小年月
}

int	CCalendar::Increment()
{
	int		chk;

	if ( chk = ( m_iYear * 100 + m_iMonth < m_iLimitMaxYear * 100 + m_iLimitMaxMonth ) ){
		if ( ++m_iMonth == 13 ){
			m_iMonth = 1;
			m_iYear++;
		}
		ResetSelect();
	}
	return( chk );
}

void CCalendar::increment()
{
	TFCLNDRNTFY		ntf[1];

	if ( Increment()){
		SetNtfyData( ntf );
		ntf->iDay = 1;
		::SendMessage( m_pParent->m_hWnd, WM_USER_Calendar_NOTIFY, UCN_INCREMENT, (LPARAM)ntf );
	}
}

int CCalendar::Decrement()
{
	int			chk;

	if ( chk = ( m_iYear * 100 + m_iMonth > m_iLimitMinYear * 100 + m_iLimitMinMonth ) ){
		if ( --m_iMonth == 0 ){
			m_iMonth = 12;
			m_iYear--;
		}
		ResetSelect();
	}
	return( chk );
}

void CCalendar::decrement()
{
	TFCLNDRNTFY		ntf[1];

	if ( Decrement() ){
		SetNtfyData( ntf );
		ntf->iDay = 1;
		::SendMessage( m_pParent->m_hWnd, WM_USER_Calendar_NOTIFY, UCN_DECREMENT, (LPARAM)ntf );
	}
}

void CCalendar::ResetSelect()
{
	memset( m_cSelDays, 0, sizeof(m_cSelDays) );
	NeedHoliday();
	NeedReserve();
	InvalidateRect( NULL, TRUE );
}

void CCalendar::ResetSelWeek()
{
	RECT		rct[1];
	
	m_iSelWeek = -1;
	rct->top = m_iHeight * 2;
	rct->bottom = m_iHeight * 8;
	rct->left = 0;
	rct->right = m_iWidth;
	InvalidateRect( rct, TRUE );
}

TFCNSELDATE *CCalendar::GetSelDays( int *cnt )
{
	int			ix, iy, idx=0;
	TFCNSELDATE	*sel=NULL;

	*cnt = 0;
	for ( ix=0; ix<7; ix++ ){
		for ( iy=0; iy<6; iy++ ){
			if ( m_cSelDays[ix][iy] )	++(*cnt);
		}
	}
	if ( *cnt ){
		if ( ( sel = (TFCNSELDATE *)calloc( *cnt, sizeof(TFCNSELDATE) ) ) ){
			for ( iy=0; iy<6; iy++ ){
				for ( ix=0; ix<7; ix++ ){
					if ( m_cSelDays[ix][iy] ){
						*(sel+idx) = m_cDays[ix][iy];
						idx++;
					}
				}
			}
		}
		else{
			*cnt = -1;
		}
	}
	return( sel );
}

int CCalendar::GetSelMonth( TFCNSELDATE *dst )
{
	int			ret=0;

	memset( dst, 0, sizeof(TFCNSELDATE) );
	dst->iYear = m_iYear;
	dst->iMonth = m_iMonth;
	return( ret );
}

int CCalendar::GetSelWeek( TFCNSELDATE *dst )
{
	int			ret=-1;

	memset( dst, 0, sizeof(TFCNSELDATE) );
	if ( m_iSelWeek != -1 ){
		if ( m_cDays[0][m_iSelWeek].iYear != 0 ){
			*dst = m_cDays[0][m_iSelWeek];
			ret = m_iSelWeek;
		}
	}
	return( ret );
}

int CCalendar::GetWeekDate( TFCNSELDATE *dst, int idx )
{
	int			ret = -1;

	memset( dst, 0, sizeof(TFCNSELDATE) );
	if ( m_cDays[0][idx].iYear != 0 ){
		*dst = m_cDays[0][idx];
		ret = idx;
	}
	return( ret );
}

void CCalendar::SetSelMode( int iSelMode )
{
	if ( m_iSelMode != iSelMode ){
		m_iSelMode = iSelMode;
		ResetSelWeek();
	}
}

void CCalendar::SetSelDays()
{
	if ( m_iSelMode != UCN_MOD_NONE )	NeedSelect();
}

int CCalendar::ShowButtons( int bShow )
{
	if ( bShow != m_bShowButtons ){
		if ( !(m_iSpecial&UCN_CSP_RIGHT) ){	// 右仕様でなければ左ボタン描画
			::ShowWindow( m_hLeftButton, bShow ? SW_NORMAL: SW_HIDE );
		}
		if ( !(m_iSpecial&UCN_CSP_LEFT) ){	// 左仕様でなければ左ボタン描画
			::ShowWindow( m_hRightButton, bShow ? SW_NORMAL: SW_HIDE );
		}
	}
	return( m_bShowButtons = bShow );
}

void CCalendar::ChangeYears( int iYear, int iMonth )
{
	m_iYear = iYear;
	m_iMonth = iMonth;
	NeedHoliday();

	InvalidateRect( NULL, FALSE );
}

void CCalendar::Redraw()
{
	NeedHoliday();
	InvalidateRect( NULL, FALSE );
}

////////////////////////////////////////////////////////////////////////
//	選択操作関係
	
int CCalendar::MouseToCell( int *cx, int *cy, int mx, int my )
{
	RECT	cel[1];
	int		i, knd=0;

	GetClientRect( cel );
	cel->bottom = m_iHeight - 1;
	if ( inr_rct( mx, my, cel ) ){
		knd = UCN_SEL_MONTH;
		*cx = *cy = -1;
	}
	if ( !knd && (m_iSpecial&UCN_CSP_WEKNUM) ){
		cel->top = HDR_HGT + m_iHeight;
		cel->left = HRZ_MGN;
		cel->bottom = cel->top + m_iHeight-1;
		cel->right = cel->left + m_iWidth-1;
		for ( i=0; i<m_WeekCount; i++ ){
			if ( inr_rct( mx, my, cel ) ){
				*cx = -1;
				*cy = i;
				knd = UCN_SEL_WEEK;
				break;
			}
			cel->top += m_iHeight;
			cel->bottom += m_iHeight;
		}
	}
	if ( !knd ){
		cel->top = HDR_HGT + m_iHeight;
		cel->left = ( (m_iSpecial&UCN_CSP_WEKNUM) ? m_iWidth: 0 ) + HRZ_MGN;
		cel->bottom = cel->top + m_iHeight-1;
		cel->right = cel->left + m_iWidth-1;
		for ( i=0; i<7 * m_WeekCount; i++ ){
			if ( inr_rct( mx, my, cel ) ){
				*cx = i % 7;
				*cy = i / 7;
				knd = UCN_SEL_DAY;
				break;
			}
			if ( ( i % 7 ) == 6 ){
				cel->top += m_iHeight;
				cel->bottom += m_iHeight;
				cel->left = ( (m_iSpecial&UCN_CSP_WEKNUM) ? m_iWidth: 0 ) + HRZ_MGN;
				cel->right = cel->left + m_iWidth-1;
			}
			else{
				cel->left += m_iWidth;
				cel->right += m_iWidth;
			}
		}
	}
	return( knd );
}

void CCalendar::GetCellRect( RECT *rct, int cx, int cy, int knd )
{
	if ( knd==UCN_SEL_MONTH ){
		GetClientRect( rct );
		rct->bottom = HDR_HGT - 1;
	}
	else if ( knd==UCN_SEL_WEEK ){
		rct->top = HDR_HGT + m_iHeight * ( cy + 1 );
		rct->bottom = rct->top + m_iHeight;
		rct->left =  HRZ_MGN;
		rct->right = m_iWidth;
	}
	else if ( knd==UCN_SEL_DAY ){
		rct->top = HDR_HGT + m_iHeight * ( cy + 1 );
		rct->bottom = rct->top + m_iHeight;
		rct->left = ((m_iSpecial&UCN_CSP_WEKNUM) ? m_iWidth: 0) + m_iWidth * cx + HRZ_MGN;
		rct->right = rct->left + m_iWidth;
	}
	else{
		memset( rct, 0, sizeof(RECT) );
	}
}

void CCalendar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnLButtonDown( nFlags, point ); 
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CCalendar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int			cx, cy;
	int			knd = MouseToCell( &cx, &cy, point.x, point.y );
	RECT		rct[1];
	
	if ( m_iSelMode ){
		if ( knd==UCN_SEL_MONTH ){
			::SendMessage( m_pParent->m_hWnd, WM_USER_Calendar_NOTIFY,
				UCN_SEL_MONTH, MAKELPARAM(0, m_iSpecial) );
		}
		else if ( knd==UCN_SEL_WEEK && (m_iSpecial&UCN_CSP_WEKNUM ) ){
			if ( cy != m_iSelWeek ){
				if ( m_iSelWeek != -1 ){
					GetCellRect( rct, cx, m_iSelWeek, knd );
					InvalidateRect( rct, TRUE );
				}
				m_iSelWeek = cy;
				GetCellRect( rct, cx, m_iSelWeek, knd );
				InvalidateRect( rct, TRUE );
				::SendMessage( m_pParent->m_hWnd, WM_USER_Calendar_NOTIFY,
					UCN_SEL_WEEK, MAKELPARAM(m_iSelWeek, m_iSpecial) );
			}
		}
		else if ( knd==UCN_SEL_DAY ){
			if ( m_cDays[cx][cy].iYear != 0 && m_cDays[cx][cy].iMonth != 0 ){
				if ( m_iSelMode==UCN_MOD_SINGLE ){
					if ( m_iPrvCX!=-1 && m_iPrvCY != -1 ){
						m_cSelDays[m_iPrvCX][m_iPrvCY] = 0;
						GetCellRect( rct, m_iPrvCX, m_iPrvCY, knd );
						InvalidateRect( rct, TRUE );
					}
				}
				m_iPrvCX = cx, m_iPrvCY = cy;
				m_iSttCX = cx, m_iSttCY = cy;
				m_cSelDays[cx][cy] = m_nDragCont = m_cSelDays[cx][cy] ? 0: 1;
				GetCellRect( rct, cx, cy, knd );
				InvalidateRect( rct, TRUE );
			}
		}
		m_bDrag = TRUE;
		m_iDragMode = knd;
		::SetCapture( m_hWnd );
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CCalendar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int			cx, cy;
	int			knd = MouseToCell( &cx, &cy, point.x, point.y );
	TFCLNDRNTFY	ntf[1];

	if ( m_iSelMode ){
		m_bDrag = FALSE;
		m_nDragCont = -1;
		::ReleaseCapture();
		SetNtfyData( ntf );
		if ( m_iDragMode==UCN_SEL_DAY ){
			::SendMessage( m_pParent->m_hWnd, WM_USER_Calendar_NOTIFY, 
				UCN_SEL_DAY, (LPARAM)ntf );
		}
	}	
	CWnd::OnLButtonUp(nFlags, point);
}

void CCalendar::OnMouseMove(UINT nFlags, CPoint point) 
{
	int			knd, cx, cy;
	RECT		rct[1];

	// ドラッグ選択処理
	if ( m_iSelMode==UCN_MOD_MULTI ){
		if ( m_bDrag && m_iPrvCX!=-1 && m_iPrvCY!=-1 ){
			knd = MouseToCell( &cx, &cy, point.x, point.y );
			if ( knd==UCN_SEL_DAY && ( cx!=m_iPrvCX || cy!=m_iPrvCY ) ){
				m_iPrvCX = cx, m_iPrvCY = cy;
/*
				if ( m_cDays[m_iSttCX][m_iSttCY].iYear != 0 && m_cDays[cx][cy].iMonth != 0 ){
					if ( !m_cSelDays[m_iSttCX][m_iSttCY] ){
						m_cSelDays[m_iSttCX][m_iSttCY] = 1;
						GetCellRect( rct, m_iSttCX, m_iSttCY, knd );
						InvalidateRect( rct, TRUE );
					}
				}
*/
				if ( m_cDays[cx][cy].iYear != 0 && m_cDays[cx][cy].iMonth != 0 ){
					//m_cSelDays[cx][cy] = 1;//m_cSelDays[cx][cy] ? 0: 1;
					m_cSelDays[cx][cy] = m_nDragCont;
					GetCellRect( rct, cx, cy, knd );
					InvalidateRect( rct, TRUE );
				}
			}
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}
