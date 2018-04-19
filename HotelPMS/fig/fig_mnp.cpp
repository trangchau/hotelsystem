/*
	図形作成・操作ルーチン実処理関数
*/

#include "..\resource.h"
//#include <windows.h>
#include "fig_mnp.h"
#include "fig_mrk.h"
#include "fig_win.h"
#include "fig_ope.h"

#define	QUE_CNT			64					// 初期キュー可能数

static	void Fld_Chk(				/*  縮尺率正規化  */
	Fld_def			*src,			/*  参照側        */
	Fld_def			*dst)			/*  出力側        */
{
	double         dx, dy, tmp, drt, srt;
	double         a2 = 2.0;

	drt = (     dst->ey - dst->sy + 1) / (     dst->ex - dst->sx + 1);
	srt = (dy = src->ey - src->sy + 1) / (dx = src->ex - src->sx + 1);

	if (srt < drt){
		tmp = dy;
		dy  = dx * drt;
		dy = (dy - tmp) / a2;
		src->ey += dy, src->sy -= dy;
	} else if (drt < srt){
		tmp = dx;
		dx  = dy / drt;
		dx = (dx - tmp) / a2;
		src->ex += dx, src->sx -= dx;
	}
}

static void mak_cnv_(						// 変換行列作成
Fig_mng			*mng )						// 描画管理構造
{
	Fld_def			dvc, wld;

	wld.sx = mng->wpt.lft;
	wld.sy = mng->wpt.top;
	wld.ex = mng->wpt.lft + mng->wpt.wid - 1;
	wld.ey = mng->wpt.top + mng->wpt.hit - 1;

	dvc.sx = mng->vpt.lft;
	dvc.sy = mng->vpt.top;
	dvc.ex = mng->vpt.lft + mng->vpt.wid - 1;
	dvc.ey = mng->vpt.top + mng->vpt.hit - 1;

	Fld_Chk( &wld, &dvc );

	mng->frt = ( dvc.ex - dvc.sx + 1 ) / ( wld.ex - wld.sx + 1 );
	Set_Mtx( mng->s2d, &dvc, &wld, 1 );
	Set_Mtx( mng->d2s, &wld, &dvc, 1 );
}

Fig_mng		*Fig_Set_Cbk(					// コールバック関数設定
HANDLE		hnd,							// 描画管理構造ハンドル
Mus_Cbk		mus,							// マウスコールバック
Ope_Cbk		ope,							// 操作コールバック
Drw_Cbk		drw,							// 描画コールバック
LPVOID		usr )							// ユーザーデータ
{
	Fig_mng			*mng = (Fig_mng *)hnd;

	if ( mng ){
		mng->mcb = mus;
		mng->ocb = ope;
		mng->dcb = drw;
		mng->usr = usr;
	}

	return( mng );
}

HWND		Fig_Ini(							// 描画初期化処理
HINSTANCE	ins,							// アプリケーションインスタンス
HWND		par,							// 親ウィンドウハンドル
Area		*dvc,							// 子ウィンドウ位置・サイズ
Area		*vpt,							// デバイスビューポート
Area		*wpt,							// システムウィンドウポート
Csr			top,							// 上層の幾何構造列
LPVOID		usr )							// User definition data
{
	Fig_mng			*mng;

	if ( ( mng = (Fig_mng *)calloc( 1, sizeof(Fig_mng) ) ) != NULL ){
		mng->par = par;
		mng->dvc = *dvc;
		mng->vpt = *vpt;
		mng->wpt = *wpt;
		mng->top = top;
		mng->usr = usr;
		if ( ( mng->wnd = Crt_Win( ins, mng ) ) != NULL ){
			mak_cnv_( mng );
		}
		else{
			free( mng );
			mng = NULL;
		}
	}

	return( (HWND)mng );
}

void	Fig_Shw(							// ウィンドウ表示／非表示処理
HANDLE	hnd,								// 描画管理構造ハンドル
int		shw )								// 表示フラグ
{
	Fig_mng			*mng = (Fig_mng *)hnd;

	if ( mng ){
		ShowWindow( mng->wnd, shw ? SW_SHOW: SW_HIDE );
	}
}

void	Fig_Set_Swn(						// システム座標系の更新
HANDLE	hnd,								// 描画管理構造ハンドル
Area	*wpt,								// 新しいシステム座標領域
int		rdw )								// 再描画フラグ
{
	Fig_mng			*mng = (Fig_mng *)hnd;

	if ( mng ){
		mng->wpt = *wpt;
		mak_cnv_( mng );
		if ( rdw )	Fig_Rdw( hnd );
	}
}

void	Fig_Set_Dwn(						// デバイス座標系の更新
HANDLE	hnd,								// 描画管理構造ハンドル
Area	*wpt,								// 新しいシステム座標領域
Area	*dvc,								// 新しいデバイス領域
Area	*vpt,								// 新しいデバイス描画領域
int		rdw,								// 再描画フラグ
int		mtx )								// 変換マトリックス更新フラグ
{
	Fig_mng			*mng = (Fig_mng *)hnd;

	if ( mng ){
		mng->wpt = *wpt;
		mng->dvc = *dvc;
		mng->vpt = *vpt;
		if ( mtx )	mak_cnv_( mng );
		MoveWindow( mng->wnd, (int)dvc->lft, (int)dvc->top, (int)dvc->wid, (int)dvc->hit, rdw );
	}
}

void	Fig_Rdw(							// 全再描画処理
HANDLE	hnd )								// 描画管理構造ハンドル
{
	Fig_mng			*mng = (Fig_mng *)hnd;
	RECT			rct;

	if ( mng ){
		rct.top    = (int)mng->vpt.top;
		rct.left   = (int)mng->vpt.lft;
		rct.bottom = (int)mng->vpt.top + (int)mng->vpt.hit - 1;
		rct.right  = (int)mng->vpt.lft + (int)mng->vpt.wid - 1;

		InvalidateRect( mng->wnd, &rct, TRUE );
	}
}

void	Fig_Rct_Drw(						// 指定矩形内再描画
HANDLE	hnd,								// 描画管理構造ハンドル
Dpn		*sp,								// 矩形始点
Dpn		*ep )								// 矩形終点
{
	Fig_mng			*mng = (Fig_mng *)hnd;
	Dpn				dvc[2], wld[2];
	RECT			rct;

	wld[0] = *sp;
	wld[1] = *ep;

	Dpn_Cnv( dvc, wld, 2, mng->s2d );

	if ( mng ){
		rct.top    = (int)dvc[1].y;
		rct.left   = (int)dvc[0].x;
		rct.bottom = (int)dvc[0].y + 1;
		rct.right  = (int)dvc[1].x + 1;
		InvalidateRect( mng->wnd, &rct, TRUE );
	}
}

int		Fig_Set_Ope(						// 図形操作の設定
HANDLE		hnd,							// 描画管理構造ハンドル
int			ope,							// 開始する操作種別
int			rtp )							// ラバーバンドの種別（作成用）
{
	Fig_mng			*mng = (Fig_mng *)hnd;
	LONG			cur = ( ope & OPE_CRT ) ? IDC_CURSOR_CRE: IDC_CURSOR_ARW;

	if ( mng ){
		mng->ope = ope;
		mng->rtp = rtp;
//		SendMessage( mng->wnd, WM_LBUTTONUP, 0, 0 );
	}
	return( mng->ope );
}

void	Fig_Drw_Elm(						// 単一幾何構造の再描画
HANDLE	hnd,								// 描画管理構造ハンドル
Fig_elm	*elm )								// 描画対象幾何構造
{
	Fig_mng			*mng = (Fig_mng *)hnd;
	Dpn				dvc[2], wld[2];
	RECT			rct;

	wld[0] = elm->srn.sp;
	wld[1] = elm->srn.ep;

	Dpn_Cnv( dvc, wld, 2, mng->s2d );

	if ( mng ){
		rct.top    = (int)dvc[1].y - RDW_OFS;
		rct.left   = (int)dvc[0].x - RDW_OFS;
		rct.bottom = (int)dvc[0].y + 1 + RDW_OFS;
		rct.right  = (int)dvc[1].x + 1 + RDW_OFS;
		InvalidateRect( mng->wnd, &rct, TRUE );
	}
}

int		Fig_Fnt_Siz(						// デバイス系文字サイズ取得
HANDLE	hnd,								// 描画管理構造ハンドル
double	siz )								// 文字サイズ（ワールド座標単位）
{
	Fig_mng			*mng = (Fig_mng *)hnd;
	int				ret = 1;

	if ( mng ){
		ret = (int)(siz * mng->frt);
	}
	return( ret );
}

int		Fig_Ent_Tmk(						// 端点マーク登録
HANDLE	hnd,								// 描画管理構造ハンドル
Trm_mrk	*ent )								// 登録する端点形状管理構造
{
	Fig_mng			*mng = (Fig_mng *)hnd;
	int				ret = 0;

	if ( mng ){
		ret = Ent_Mrk( ent, mng );
	}
	return( ret );
}

void	Fig_Trm(							// 描画終了処理
HANDLE	hnd )								// 描画管理構造ハンドル
{
	Fig_mng			*mng = (Fig_mng *)hnd;

	if ( mng ){
		SendMessage( mng->wnd, WM_LBUTTONUP, 0, 0 );
		Del_All( mng->mrk );
		SendMessage( mng->wnd, WM_CLOSE, 0, 0 );
		free( mng );
	}
}

