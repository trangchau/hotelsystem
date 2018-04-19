/*
	プレビュー画面出力ファイル
	Copyright(C) Arsnova Corp. 2003
*/

#include <afx.h>
#include <Windows.h>
#include <Winnt.h>
#include <stdlib.h>
#include "..\Common\bsc_def.h"
#include "afn_cnv.h"
#include "print.h"
#include "prv_win.h"
#include "ppr_siz.h"

#define	CTL_BTN			1		// コントロールタイプ（ボタン）
#define	CTL_CBX			2		// コントロールタイプ（コンボボックス）
#define	CTL_EDT			3		// コントロールタイプ（エディットボックス）
#define	CTL_STC			4		// コントロールタイプ（スタティック）

#define	CNO_CLS			1		// クローズボタン番号（ﾌﾟﾚﾋﾞｭｰｳｲﾝﾄﾞｳ）
#define	CNO_SCL			2		// スケール変更コンボボックス番号（ﾌﾟﾚﾋﾞｭｰｳｲﾝﾄﾞｳ）

typedef	struct	{				// コントロールプロパティ
	HWND	wnd;				// コントロールのハンドル
	int		typ;				// コントロールタイプ
	int		lft;				// 左位置
	int		top;				// 上位置
	int		wid;				// 幅
	int		hit;				// 高さ
	char	*cap;				// キャプション
	int		cno;				// コントロール番号
	WNDPROC	org;				// オリジナルのウィンドウプロシージャ
	WNDPROC	prc;				// サブクラス化したウィンドウプロシージャ
	int		sid;				// サブクラスのＩＤ
}Ctl_Prp;

#define	WM_USERITNIT	(WM_USER+16384)

typedef	struct	{			 		// ウィンドウメッセージ構造体
	HWND		wnd;		 		// ウィンドウハンドル
	UINT		msg;		 		// メッセージコード
	UINT		wpr;		 		// パラメータ（ＵＩＮＴ）
	LONG		lpr;		 		// パラメータ（ＬＯＮＧ）
}Win_Msg;

#define	Nxt_Ctl(x,y)	(((x)+1)==memcnt(y) ? 0: (x)+1)
#define	Prv_Ctl(x,y)	(((x)==0) ? memcnt(y)-1 : x-1 )
#define	MBR_HIT			41					// メニューバー高さ
#define	MGN_HIT			10					// 表示マージン高さ
#define	MGN_WID			10					// 表示マージン幅

static	LONG	APIENTRY	sub_prc_( HWND, UINT, UINT, LONG );

static	Ctl_Prp		Ctl_Tbl[] = {
{ NULL,CTL_BTN,60, (MBR_HIT-28)/2,60,28,  "閉じる",CNO_CLS,NULL,sub_prc_, 0 },
{ NULL,CTL_CBX,130,(MBR_HIT-28)/2,70,32*4,"",      CNO_SCL,NULL,sub_prc_, 0 }
};

typedef	struct	{							// 描画関係各値
	int		cvw;							// キャンバスの幅
	int		cvh;							// キャンバスの高さ
	int		hmx;							// 水平最大スクロール量
	int		vmx;							// 垂直最大スクロール量
	int		orx;							// 表示原点Ｘ座標
	int		ory;							// 表示原点Ｙ座標
	RECT	ppr;							// 用紙包含矩形
	int		scl;							// 現在のスケール値
	int		mbr;							// メニューバー有無
	int		mbh;							// メニューバー高さ
	int		hmg;							// 表示マージン高さ
	int		wmg;							// 表示マージン幅
}Drw_ctl;

typedef	struct	{							// プレビュー管理構造
	Prn_mng		mng[1];						// 印刷管理構造
	Drw_ctl		drw[1];						// 描画管理構造
}Prv_mng;

static	LONG	APIENTRY	sub_prc_(		// サブクラスプロシージャ
HWND	wnd,								// ウィンドウハンドル
UINT	msg,								// メッセージコード
UINT	wpr,								// パラメータ（ＵＩＮＴ）
LONG	lpr )								// パラメータ（ＬＯＮＧ）
{
	int			cid, nxt, i, ret=1;
	HWND		par;

	cid = GetWindowLong( wnd, GWL_ID );

	for ( i=0; i<memcnt(Ctl_Tbl); i++ ){
		if ( Ctl_Tbl[i].sid == cid ){
			cid = i;
			break;
		}
	}

	if ( msg == WM_KEYDOWN ){

		if ( wpr==VK_TAB ){
			if ( GetKeyState(VK_SHIFT)<0 )	nxt = Prv_Ctl(cid, Ctl_Tbl);
			else							nxt = Nxt_Ctl(cid, Ctl_Tbl);
			if ( Ctl_Tbl[nxt].wnd )			SetFocus( Ctl_Tbl[nxt].wnd );
			ret = 0;
		}
		if ( wpr==VK_RETURN ){
			if ( Ctl_Tbl[cid].cno == CNO_SCL ){
				par = GetParent( Ctl_Tbl[cid].wnd );
				SendMessage( par, WM_COMMAND,
						(WPARAM)MAKELONG(CNO_SCL,CBN_KILLFOCUS),
										(LPARAM)Ctl_Tbl[cid].wnd );
				ret = 0;
			}
			if ( Ctl_Tbl[cid].cno == CNO_CLS ){
				wpr = VK_SPACE;
			}

		}
	}

	if ( msg == WM_CHAR ){
		if ( Ctl_Tbl[cid].cno == CNO_SCL ){
			if ( !isdigit( wpr ) && !iscntrl( wpr )){
				Beep( 440,100 );
				ret = 0;
			}
		}
	}


	if ( msg == WM_KEYUP || msg == WM_CHAR ){	// ぷっ！を消す
		if ( ( wpr==VK_TAB || wpr==VK_RETURN ) && Ctl_Tbl[cid].cno == CNO_SCL ){
			ret = 0;
		}
		if ( wpr==VK_RETURN && Ctl_Tbl[cid].cno == CNO_CLS){
			wpr = VK_SPACE;
		}
	}

	return( ret ? Ctl_Tbl[cid].org( wnd, msg, wpr, lpr ): ret );
}

static void	drw_ini_(				// 描画構造体初期化
Prv_mng		*mng )				// プレビュー構造
{
	int				wid, hgt;

	Get_Pdv_Inf( mng->mng, MOD_PRV );

	if ( Ppr_Siz( &wid, &hgt, mng->mng->ppr, mng->mng->mtx ) ){
		mng->drw->ppr.left   = 0;
		mng->drw->ppr.top    = 0;
		mng->drw->ppr.right  = mng->drw->ppr.left + wid - 1;
		mng->drw->ppr.bottom = mng->drw->ppr.top + hgt - 1;
		mng->drw->cvw = wid + mng->drw->wmg * 2;
		mng->drw->cvh = hgt + mng->drw->hmg * 2 + mng->drw->mbh;
	}
	mng->drw->orx = mng->drw->ory = 0;
}

static void	drw_mbr_(				// メニューバーの描画
Drw_ctl		*drw,					// 描画構造体
HWND		wnd,					// ウィンドウハンドル
HDC			dvh )					// デバイスコンテキスト
{
	RECT		rct;
	HGDIOBJ		opn, obs;			// ﾃﾞﾌｫﾙﾄﾍﾟﾝ、ﾌﾞﾗｼ

	GetClientRect( wnd, &rct );

	opn = SelectObject( dvh, GetStockObject( WHITE_PEN ) );
	obs = SelectObject( dvh, GetStockObject( LTGRAY_BRUSH ) );

	Rectangle( dvh, 0, 0, rct.right, drw->mbh );
	SelectObject( dvh, GetStockObject( BLACK_PEN ) );
	MoveToEx( dvh, 0, drw->mbh-1, NULL );
	LineTo( dvh, rct.right-1, drw->mbh-1 );
	LineTo( dvh, rct.right-1, 0 );
	SelectObject( dvh, opn );
	SelectObject( dvh, obs );
}

static void out_ppr_(				// 用紙描画
RECT		*ppr,					// 用紙矩形
HWND		wnd,					// ウィンドウハンドル
HDC			dvh )					// デバイスコンテキスト
{
	HPEN		pen;							// ペンobj
	HGDIOBJ		opn, obs;						// ﾃﾞﾌｫﾙﾄﾍﾟﾝ、ﾌﾞﾗｼ

	pen = CreatePen( PS_SOLID, 1, RGB( 0, 0, 0 ) );
	if ( pen ){
		opn = SelectObject( dvh, pen );
		obs = SelectObject( dvh, GetStockObject( WHITE_BRUSH ) );

		Rectangle( dvh,ppr->left-1,ppr->top-1,ppr->right+1,ppr->bottom+1 );
		SelectObject( dvh, opn );
		DeleteObject( pen );
		SelectObject( dvh, obs );
	}
}

//---------------------------------------------------------------------------
// プレビュー画面出力
// プレビューは、まず紙とその中に印刷されるデータを、メモリＤＣに作成し、
// それをウィンドウＤＣにBitBltを使用してコピーする。
// これは、WM_PAINTによるクリッピングリージョンを、データの描画ルーチンで
// 破壊する恐れがあるのを回避するとともに、スクロールロジックの簡素化を図る
// 目的でのロジックである。

static void out_prv_(			// プレビューイメージ描画	
HWND		wnd,				// ウィンドウハンドル
HDC			dvh,				// 描画用デバイスコンテキスト
Prv_mng		*mng,				// プレビュー構造
PAINTSTRUCT	*ps )				// 描画構造体
{
	HRGN		rgn;			// クリッピングリージョン
	HBITMAP		bmp;			// 出力先Bmp
	HGDIOBJ		obm;			// デフォルトBmp
	POINT		opt;			// ﾃﾞﾌｫﾙﾄ原点位置
	RECT		cli;
	HDC			cpy;
	int			wid, hit;

	cpy = CreateCompatibleDC( dvh );

	if ( cpy && GetClientRect( wnd, &cli ) ){
		wid = cli.right - cli.left + 1;
		hit = cli.bottom - cli.top + 1;
		bmp = CreateCompatibleBitmap( dvh, wid, hit );
		if ( bmp ){
			obm = SelectObject( cpy, bmp );
			FillRect( cpy, &cli, (HBRUSH)::GetStockObject( GRAY_BRUSH ) );
			SetViewportOrgEx( cpy, mng->drw->hmg+mng->drw->orx,
								mng->drw->hmg+mng->drw->mbh+mng->drw->ory, &opt );
			if ( mng->mng->prn ){
				out_ppr_( &mng->drw->ppr, wnd, cpy );
				rgn = CreateRectRgn(
					mng->drw->ppr.left+mng->drw->hmg+mng->drw->orx,
						mng->drw->ppr.top+mng->drw->hmg+mng->drw->mbh+mng->drw->ory,
							mng->drw->ppr.right+mng->drw->hmg+mng->drw->orx,
								mng->drw->ppr.bottom+mng->drw->hmg+mng->drw->mbh+mng->drw->ory );
				if ( rgn ){
					SelectObject( cpy, rgn );
					Prn_Out_Dat( cpy, mng->mng, MOD_PRV, rgn );
					DeleteObject( rgn );
				}
			}
			SetViewportOrgEx( cpy, opt.x, opt.y, NULL );

			BitBlt( dvh, 0, 0, wid, hit, cpy, 0, 0, SRCCOPY );
			SelectObject( cpy, obm );
			DeleteObject( bmp );
		}
		DeleteDC( cpy );
	}

	if ( mng->drw->mbr )	drw_mbr_( mng->drw, wnd, dvh );
}

static HWND	mak_ctl_(			// 子コントロール作成
Ctl_Prp		*prp,				// プロパティハンドル
HINSTANCE	ins,				// インスタンスハンドル
HWND		par )				// 親ウィンドウハンドル
{
	HWND		wnd, sub;
	LPCWSTR		cnm;
	DWORD		sty = WS_CHILD|WS_VISIBLE|WS_TABSTOP;
	POINT		pt;

	switch( prp->typ ){
	case CTL_BTN:	cnm = _T("button");
					sty |= BS_PUSHBUTTON;//BS_DEFPUSHBUTTON;
					break;
	case CTL_CBX:	cnm = _T("combobox");
					sty |= CBS_DROPDOWN|CBS_HASSTRINGS|WS_VSCROLL;
					break;
	case CTL_EDT:	cnm = _T("edit");
					sty |= WS_BORDER | ES_LEFT;
					break;
	}
//	2018/01/01
//	wnd = CreateWindow( cnm, prp->cap, sty, prp->lft, prp->top,
//						prp->wid, prp->hit, par, (HMENU)prp->cno, ins, NULL );
	if ( wnd ){
		prp->wnd = wnd;
		if ( prp->typ == CTL_CBX ){		// コンボボックスの場合は、その中の子ウインドウである
			pt.x = 5;					// エディットコントロールのWinProcをフック
			pt.y = 5;					// する必要がある。
			sub = ChildWindowFromPoint( wnd, pt );
			if ( sub ){
				prp->sid = GetWindowLong( sub, GWL_ID );
			}
		}
		else{
			prp->sid = prp->cno;
			sub = wnd;
		}
		if ( sub ){
			prp->org = (WNDPROC)(LONG_PTR)GetWindowLong( sub, GWL_WNDPROC );
			SetWindowLong( sub, GWL_WNDPROC, (LONG)(LONG_PTR)prp->prc );
		}
	}
	return( wnd );
}

static	void	cre_fnc_(		// ウィンドウ生成時関数
Prv_mng		*mng,				// プレビュー構造
Win_Msg		*msg )				// ウィンドウメッセージ
{
	static	int	 scl[] = {					// 拡大率インデックス
		200, 150, 100, 75, 50, 25, 10
	};

	HWND			wnd;
	HINSTANCE		ins;
	int				i, j, idx;
	WCHAR			buf[20];

	drw_ini_( mng );

	if ( mng->mng->prv->mbr ){
		ins = ((LPCREATESTRUCT)(LONG_PTR)(msg->lpr))->hInstance;

		for ( i=0; i<memcnt(Ctl_Tbl); i++ ){
			wnd = mak_ctl_( &Ctl_Tbl[i], ins, msg->wnd );

			if ( wnd && Ctl_Tbl[i].cno == CNO_SCL ){
				for ( j=0, idx=-1; j<memcnt(scl); j++ ){
					swprintf_s( buf, memcnt(buf), _T("%3d%%"), scl[j] );
					SendMessage( wnd, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)buf );
					if ( mng->drw->scl == scl[j] )	idx = j;
				}
				if ( idx == -1 ){
					swprintf_s( buf, memcnt(buf), _T("%3d%%"), mng->drw->scl );
					SendMessage( wnd, WM_SETTEXT, 0, (LPARAM)buf );
				}
				else{
					SendMessage( wnd, CB_SETCURSEL, (WPARAM)idx, 0 );
				}
				SendMessage( wnd, CB_LIMITTEXT, (WPARAM)3, 0 );
				SetFocus( wnd );
			}
		}
	}
}

static	void	siz_fnc_(		// WM_SIZE時の関数
Drw_ctl		*drw,				// 描画関係構造体
Win_Msg		*msg )				// ウィンドウメッセージ
{
	RECT		rct;
	int			min;

	GetClientRect( msg->wnd, &rct );

	if ( drw->cvw > rct.right-rct.left-1 ){
		min = 0;
		drw->hmx = drw->cvw - ( rct.right - rct.left - 1 );
	}
	else{
		min = drw->hmx = 0;
	}
	SetScrollRange( msg->wnd, SB_HORZ, min, drw->hmx, true );
	SetScrollPos( msg->wnd, SB_HORZ, 0, true );

	if ( drw->cvh > rct.bottom-rct.top-1 ){
		min = 0;
		drw->vmx = drw->cvh - ( rct.bottom - rct.top - 1 );
	}
	else{
		min = drw->vmx = 0;
	}
	SetScrollRange( msg->wnd, SB_VERT, min, drw->vmx, true );
	SetScrollPos( msg->wnd, SB_VERT, 0, true );

	if ( drw->orx != 0 || drw->ory != 0 ){
		drw->orx = drw->ory = 0;
		InvalidateRect( msg->wnd, NULL, true );
		SendMessage( msg->wnd, WM_PAINT, 0, 0 );
	}
	else{
		rct.bottom = drw->mbh - 1;
//		rct.left = rct.right - 50;
		InvalidateRect( msg->wnd, &rct, false );
		SendMessage( msg->wnd, WM_PAINT, 0, 0 );
	}
}

static	void	cmd_fnc_(		// WM_COMMAND時の関数
Prv_mng		*mng,				// プレビュー構造
Win_Msg		*msg )				// ウィンドウメッセージ
{
	int			cid, cod, val, idx;
	WCHAR		buf[20];
	HWND		cmb;

	cid = LOWORD(msg->wpr);
	cod = HIWORD(msg->wpr);

	switch( cid ){
	case CNO_CLS:	if ( cod == BN_CLICKED	){
						SendMessage( msg->wnd, WM_CLOSE, 0, 0 );
					}
					break;
	case CNO_SCL:   if ( cod == CBN_KILLFOCUS || cod == CBN_SELCHANGE ){
						cmb = (HWND)(LONG_PTR)msg->lpr;
						val = -1;
						if ( cod == CBN_KILLFOCUS ){
							SendMessage(cmb,WM_GETTEXT,sizeof(buf),(LPARAM)buf);
							val = _wtoi( buf );
						}
						else{
							idx = (int)SendMessage( cmb, CB_GETCURSEL, 0, 0 );
							if ( idx != CB_ERR ){
								SendMessage( cmb,CB_GETLBTEXT,idx,(LPARAM)buf );
								val = _wtoi( buf );
							}
						}
						if ( val != -1 ){
							if ( val != 0 ){
								if ( val != mng->drw->scl ){
									mng->drw->scl = mng->mng->prv->scl = val;
									drw_ini_( mng );
									siz_fnc_( mng->drw, msg );
									InvalidateRect( msg->wnd, NULL, TRUE );
									SendMessage( msg->wnd, WM_PAINT, 0, 0 );
									swprintf_s( buf, memcnt(buf), _T("%3d%%"), val );
									SendMessage( cmb,WM_SETTEXT,0,(LPARAM)buf );
									SetFocus( cmb );
								}
							}
							else{
								swprintf_s( buf, memcnt(buf), _T("%3d%%"), mng->drw->scl );
								SendMessage( cmb,WM_SETTEXT,0,(LPARAM)buf );
								Beep( 440, 250 );
								SetFocus( cmb );
							}
						}
					}
					break;
	}
}

static	void	hrz_scr_(		// 水平スクロール
Drw_ctl		*drw,				// 描画関係構造体
HWND		wnd,				// ウィンドウハンドル
UINT		wpr,				// つまみ位置, スクロール種別
LONG		lpr )			  	// スクロール バーのハンドル
{
	RECT		scr;
	int			dtx, amt = drw->orx;
	UINT		knd, pos;

	knd = LOWORD( wpr );
	pos = HIWORD( wpr );

	switch( knd ){
	case SB_LINERIGHT:
	case SB_PAGERIGHT:		amt -= knd==SB_LINERIGHT ? 1: 10;
							if ( abs(amt) > drw->hmx )	amt = drw->hmx * -1;
							break;
	case SB_LINELEFT:
	case SB_PAGELEFT:		amt += knd==SB_LINELEFT ? 1: 10;
							if ( amt > 0 )		amt = 0;
							break;
	case SB_TOP:			amt = 0;
							break;
	case SB_BOTTOM:			amt = drw->hmx * -1;
							break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:	amt = LOWORD( pos ) * -1;
							break;
	}
	if ( amt != drw->orx ){
		dtx = amt - drw->orx;
		drw->orx = amt;
		if ( GetClientRect( wnd, &scr ) ){
			scr.top += drw->mbh;
			ScrollWindow( wnd, dtx, 0, &scr, &scr );
			if ( dtx < 0 )	scr.left = scr.right + dtx;
			else			scr.right = scr.left + dtx;
			SetScrollPos( wnd, SB_HORZ, abs(amt), true );
			InvalidateRect( wnd, &scr, TRUE );
			SendMessage( wnd, WM_PAINT, (WPARAM)0, (LPARAM)0 );
		}
	}
}

static	void	hsb_fnc_(		// 水平スクロールバー処理
Drw_ctl		*drw,				// 描画関係構造体
Win_Msg		*msg )				// ウィンドウメッセージ
{
	hrz_scr_( drw, msg->wnd, msg->wpr, msg->lpr );
}

static	void	vrt_scr_(		// 垂直スクロール
Drw_ctl		*drw,				// 描画関係構造体
HWND		wnd,				// ウィンドウハンドル
UINT		wpr,				// つまみ位置, スクロール種別
LONG		lpr )				// スクロール バーのハンドル
{
	RECT		scr;
	int			dtx, amt = drw->ory;
	UINT		knd, pos;

	knd = LOWORD( wpr );
	pos = HIWORD( wpr );

	switch( knd ){
	case SB_LINEDOWN:
	case SB_PAGEDOWN:		amt -= knd==SB_LINEDOWN ? 10: 100;
							if ( abs(amt) > drw->vmx )	amt = drw->vmx * -1;
							break;
	case SB_LINEUP:
	case SB_PAGEUP:			amt += knd==SB_LINEUP ? 10: 100;
							if ( amt > 0 )		amt = 0;
							break;
	case SB_TOP:			amt = 0;
							break;
	case SB_BOTTOM:			amt = drw->vmx * -1;
							break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:	amt = LOWORD( pos ) * -1;
							break;
	}
	if ( amt != drw->ory ){
		dtx = amt - drw->ory;
		drw->ory = amt;
		if ( GetClientRect( wnd, &scr ) ){
			scr.top += drw->mbh;
			ScrollWindow( wnd, 0, dtx, &scr, &scr );
			if ( dtx < 0 )	scr.top = scr.bottom + dtx;
			else			scr.bottom = scr.top + dtx;
			SetScrollPos( wnd, SB_VERT, abs(amt), true );
			InvalidateRect( wnd, &scr, TRUE );
			SendMessage( wnd, WM_PAINT, (WPARAM)0, (LPARAM)0 );
		}
	}
}

static	void	vsb_fnc_(		// 垂直スクロールバー処理
Drw_ctl		*drw,				// 描画関係構造体
Win_Msg		*msg )				// ウィンドウメッセージ
{
	vrt_scr_( drw, msg->wnd, msg->wpr, msg->lpr );
}

static LONG APIENTRY WinProc(	// ウィンドウプロシージャ
HWND		wnd,				// ウィンドウハンドル
UINT		msg,				// ウィンドウメッセージ
UINT		wpr,				// パラメータ（ＷＯＲＤ）
LONG		lpr )				// パラメータ（ＬＯＮＧ）
{
	LONG			ret;
	PAINTSTRUCT		ps;
	HDC				hdc;
	Win_Msg			wms;
	HWND			par = (HWND)(LONG_PTR)GetWindowLong( wnd, GWL_HWNDPARENT );
	Prv_mng			*mng = (Prv_mng *)(LONG_PTR)GetWindowLong( par, GWL_USERDATA );

	wms.wnd = wnd;
	wms.msg = msg;
	wms.wpr = wpr;
	wms.lpr = lpr;
	
	switch( msg ){
	case WM_CREATE:		cre_fnc_( mng, &wms );
						ret = 0;
						break;
	case WM_PAINT:		if ( !!GetUpdateRect( wnd, NULL, FALSE) ){
							hdc = BeginPaint( wnd, &ps );
							out_prv_( wnd, hdc, mng, &ps );
							EndPaint( wnd, &ps );
						}
						ret = 0;
						break;
	case WM_SIZE:       if ( mng )	siz_fnc_( mng->drw, &wms );
						ret = 0;
						break;
	case WM_COMMAND:    cmd_fnc_( mng, &wms );
						ret = 0;
						break;
	case WM_HSCROLL:	hsb_fnc_( mng->drw, &wms );
						ret = 0;
						break;
	case WM_VSCROLL:
						vsb_fnc_( mng->drw, &wms );
						ret = 0;
						break;
	case WM_DESTROY:	PostQuitMessage( 0 );
						ret = 0;
						break;
	default:			ret = (int)DefWindowProc( wnd, msg, wpr, lpr );
						break;
	}
	return( ret );
}

static	int		reg_cls_(		// ウィンドウクラス登録
HINSTANCE		ins )			// ｱﾌﾟﾘｹｰｼｮﾝｲﾝｽﾀﾝｽ
{
	WNDCLASS	cls;


	cls.style         = 0;//CS_HREDRAW | CS_VREDRAW;
	cls.lpfnWndProc   = WinProc;
	cls.cbClsExtra    = 0;
	cls.cbWndExtra    = 0;
	cls.hInstance     = ins;
	cls.hIcon         = LoadIcon( ins, _T("MAINICON") );
	cls.hCursor		  = LoadCursor( NULL, IDC_ARROW );
	cls.hbrBackground = (HBRUSH)::GetStockObject( GRAY_BRUSH );
	cls.lpszMenuName  = NULL;
	cls.lpszClassName = WND_CLS;

	UnregisterClass( WND_CLS, ins );
	return( RegisterClass( &cls ) );
}

//#define	WS_OVRLAP	WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MAXIMIZEBOX
#define	WS_OVRLAP	WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL

static	UINT	prv_win(		// プレビューウィンドウスレッド
LPVOID		pParam )
{
	Prn_mng		*mng = (Prn_mng *)pParam;
	HWND		wnd;
	MSG			msg;
	Prv_def		*sty = mng->prv;
	Prv_mng		*prv;

	msg.wParam = -1;

	if ( prv = (Prv_mng *)malloc( sizeof(Prv_mng) ) ){
		*(prv->mng) = *mng;
		if ( mng->prv->scl>999 || mng->prv->scl<1 )	mng->prv->scl = 100;
		if ( prv->drw->mbr = prv->mng->prv->mbr ){
			prv->drw->mbh = MBR_HIT;
			prv->drw->hmg = MGN_HIT;
			prv->drw->wmg = MGN_WID;
		}
		else{
			prv->drw->mbh = 0;
			prv->drw->hmg = 0;
			prv->drw->wmg = 0;
		}
		prv->drw->scl = mng->prv->scl;
		SetWindowLong( mng->par, GWL_USERDATA, (LONG)(LONG_PTR)prv );

		if ( reg_cls_( mng->ins ) ){
			wnd = CreateWindow( WND_CLS, _T(""), WS_OVRLAP, sty->lft, sty->top,
				sty->wid, sty->hit, mng->par, NULL, mng->ins, NULL  );
			if ( wnd ){
				mng->wnd = prv->mng->wnd = wnd;
				ShowWindow( wnd, SW_NORMAL );
				UpdateWindow( wnd );
				while( GetMessage( &msg, NULL, 0, 0 ) ){
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}
		}
		SetWindowLong( mng->par, GWL_USERDATA, NULL );
		free( prv );
	}
	return( 0 );
}

// この関数は親ウィンドウのユーザー定義領域を使用するので、
// 親ウィンドウ側でユーザー定義領域を使用してはならない。

int		Cre_Prv_Win(			// プレビューウィンド生成
Prn_mng		*mng )				// 印刷管理構造
{
	int			chk;

//	2018/01/01
//	chk = !!AfxBeginThread( prv_win, (LPVOID)mng, THREAD_PRIORITY_NORMAL, 0, 0, NULL );
	return( chk );
}

void	Dst_Prv_Win(			// プレビューウィンド生成
Prn_mng		*mng )				// 印刷管理構造
{
	::SendMessage( mng->wnd, WM_CLOSE, 0, 0 ); 
}