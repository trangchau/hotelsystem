/*
	Functions for each element draw 
*/

#include <afx.h>
#include "fig_mnp.h"
#include "fig_drw.h"
#include "fig_mrk.h"

static	void	drw_txt_(			// 文字列描画
HDC		hdc,						// 描画対象デバイス
WCHAR	*txt,						// 文字列
Dpn		*pos,						// 描画位置（ワールド座標系）
Dpn		*srn,
double	siz,						// 文字サイズ（ワールド座標単位）
int		col,						// 文字色
HWND	wnd )						// ウィンドウハンドル
{
	int					omd;
	Dpn					bfr[2], tps[2];				// 座標変換前、後
	HFONT				fnh;					// フォントハンドル
	HGDIOBJ				ofn;					// ﾃﾞﾌｫﾙﾄﾌｫﾝﾄ
	LOGFONT				fnt;		   			// フォント情報構造体
	COLORREF			otc;					// デフォルト色
	Fig_mng				*mng;
	RECT				rct[1];

	mng = (Fig_mng *)(LONG_PTR)GetWindowLong( wnd, GWL_USERDATA );

	if ( mng ){
		memset( &fnt, 0, sizeof(LOGFONT) );
		bfr[0].x = pos->x;
		bfr[0].y = pos->y;
		bfr[1].x = srn->x;
		bfr[1].y = srn->y;
		Dpn_Cnv( tps, bfr, 2, mng->s2d );			// 変換座標取得
	
		rct->left = (int)tps[0].x;
		rct->top = (int)tps[0].y;
		rct->right = (int)tps[1].x;
		rct->bottom = (int)tps[1].y;

		fnt.lfCharSet = DEFAULT_CHARSET;			//SHIFTJIS_CHARSET;
		wcscpy_s( fnt.lfFaceName, memcnt(fnt.lfFaceName), _T("MS UI Gothic") );	// フォント名
		fnt.lfHeight = (int)(siz * mng->frt);		// フォントサイズ
		if ( !fnt.lfHeight )	fnt.lfHeight = 1;	// 最低値
		fnt.lfWeight = 400;			   				// スタイル～ボールド（400,700)
		fnt.lfQuality = PROOF_QUALITY;
		fnh = CreateFontIndirect( &fnt );
		if ( fnh != NULL ){
			ofn = SelectObject( hdc, fnh );
			otc = GetTextColor( hdc );
			SetTextColor( hdc, col );
			omd = SetBkMode( hdc, TRANSPARENT );
			DrawText( hdc, txt, (int)wcslen(txt), rct, DT_CENTER|DT_VCENTER|DT_TOP );
			SetBkMode( hdc, omd );
			SelectObject( hdc, ofn );
			SetTextColor( hdc, otc );
			DeleteObject( fnh );
		}
	}
}
/*

static	void	drw_txt_(			// 文字列描画
HDC		hdc,						// 描画対象デバイス
WCHAR	*txt,						// 文字列
Dpn		*pos,						// 描画位置（ワールド座標系）
Dpn		*srn,
double	siz,						// 文字サイズ（ワールド座標単位）
int		col,						// 文字色
HWND	wnd )						// ウィンドウハンドル
{
	int					omd;
	Dpn					bfr, tps;				// 座標変換前、後
	HFONT				fnh;					// フォントハンドル
	HGDIOBJ				ofn;					// ﾃﾞﾌｫﾙﾄﾌｫﾝﾄ
	LOGFONT				fnt;		   			// フォント情報構造体
	COLORREF			otc;					// デフォルト色
	Fig_mng				*mng;

	mng = (Fig_mng *)(LONG_PTR)GetWindowLong( wnd, GWL_USERDATA );

	if ( mng ){
		memset( &fnt, 0, sizeof(LOGFONT) );
		bfr.x = pos->x;
		bfr.y = pos->y;
		Dpn_Cnv( &tps, &bfr, 1, mng->s2d );			// 変換座標取得

		fnt.lfCharSet = DEFAULT_CHARSET;			//SHIFTJIS_CHARSET;
		wcscpy_s( fnt.lfFaceName, memcnt(fnt.lfFaceName), _T("MS UI Gothic") );	// フォント名
		fnt.lfHeight = (int)(siz * mng->frt);		// フォントサイズ
		if ( !fnt.lfHeight )	fnt.lfHeight = 1;	// 最低値
		fnt.lfWeight = 400;			   				// スタイル～ボールド（400,700)
		fnt.lfQuality = PROOF_QUALITY;
		fnh = CreateFontIndirect( &fnt );
		if ( fnh != NULL ){
			ofn = SelectObject( hdc, fnh );
			otc = GetTextColor( hdc );
			SetTextColor( hdc, col );
			omd = SetBkMode( hdc, TRANSPARENT );
			TextOut( hdc, (int)tps.x, (int)tps.y, txt, (int)wcslen(txt) );
			SetBkMode( hdc, omd );
			SelectObject( hdc, ofn );
			SetTextColor( hdc, otc );
			DeleteObject( fnh );
		}
	}
}
*/

static	void	drw_smk_(			// 選択マーク描画
HDC			hdc,					// 描画対象デバイス
POINT		*pnt,					// 座標列
int			ptc )					// 座票数
{
	int				top, btm, lft, rit;
	HBRUSH			brs, obs;
	HPEN			pen, opn;

	pen = CreatePen( PS_SOLID, 1, COL_BLK );
	brs = CreateSolidBrush( COL_BLK );

	if ( pen && brs ){
		opn = (HPEN)SelectObject( hdc, pen );
		obs = (HBRUSH)SelectObject( hdc, brs );
		while ( ptc-- ){
			top = pnt->y - SMK_SIZ / 2;
			lft = pnt->x - SMK_SIZ / 2;
			btm = pnt->y + SMK_SIZ / 2 + 1;
			rit = pnt->x + SMK_SIZ / 2 + 1;
			Rectangle( hdc, lft, top, rit, btm );
			pnt++;
		}
		SelectObject( hdc, opn );
		SelectObject( hdc, obs );
	}
	if ( pen )	DeleteObject( pen );
	if ( brs )	DeleteObject( brs );
}

static	int	drw_lin_(				// 線分要素描画
HDC			hdc,					// 描画対象デバイス
Fig_elm		*elm,					// 幾何構造ポインタ
Fig_mng		*mng,					// 図形管理構造
HWND		wnd )					// ウィンドウハンドル
{
	Dpn			dvc[2], wld[2];
	Lin			*mrk;
	int			i, cnt, chk=TRUE;
	POINT		pnt[2];

	if ( chk && ( elm->mrk&(MRK_LFT|MRK_RIT) ) ){
		if ( ( cnt = Add_Mrk( (void *)&mrk, elm, mng ) ) != -1 ){
			for ( i=0; i<cnt; i++ ){
				wld[0].x = (mrk+i)->sp->x;	wld[0].y = (mrk+i)->sp->y;
				wld[1].x = (mrk+i)->ep->x;	wld[1].y = (mrk+i)->ep->y;
				Dpn_Cnv( dvc, wld, 2, mng->s2d );
				chk = MoveToEx( hdc, (int)dvc[0].x, (int)dvc[0].y, NULL );
				if ( chk ){
					chk = LineTo( hdc, (int)dvc[1].x, (int)dvc[1].y );
				}
				if ( !chk )		break;
			}
			free( mrk );
		}
	}
	if ( chk ){
		wld[0] = *elm->elm.lin.sp;
		wld[1] = *elm->elm.lin.ep;

		Dpn_Cnv( dvc, wld, 2, mng->s2d );

		chk = MoveToEx( hdc, (int)dvc[0].x, (int)dvc[0].y, NULL );
		if ( chk ){
			chk = LineTo( hdc, (int)dvc[1].x, (int)dvc[1].y );
		}
		if ( mng->dcb )	mng->dcb( hdc, elm, drw_txt_, wnd, mng->usr );
	}
	if ( chk && elm->sel ){
		pnt[0].x = (int)dvc[0].x;
		pnt[0].y = (int)dvc[0].y;
		pnt[1].x = (int)dvc[1].x;
		pnt[1].y = (int)dvc[1].y;
		drw_smk_( hdc, pnt, 2 );
	}
	return( chk );
}

static	Trm_mrk	*drw_mrk_(			// 矩形の端点マーク描画
HDC			hdc,					// 描画対象デバイス
Trm_mrk		*mrk,					// 短点マーク構造体
Fig_mng		*mng )					// 図形管理構造
{
	int			i, cnt=mrk->pct;
	Dpn			*dvc;
	POINT		*pnt;
	HBRUSH		brs, obs;

	if ( ( brs = CreateSolidBrush( mrk->bcl ) ) != NULL ){
		obs = (HBRUSH)SelectObject( hdc, brs );
		pnt = (POINT *)calloc( cnt, sizeof(POINT) );
		dvc = (Dpn *)calloc( cnt, sizeof(Dpn) );
		if ( pnt && dvc ){
			Dpn_Cnv( dvc, mrk->pnt, cnt, mng->s2d );
			for ( i=0; i<cnt; i++ )	pnt[i].x=(int)dvc[i].x, pnt[i].y=(int)dvc[i].y;
			Polygon( hdc, pnt, cnt );
		}
		if ( dvc )	free( dvc );
		if ( pnt )	free( pnt );
		SelectObject( hdc, obs );
		DeleteObject( brs );
	}
	return( nxt_tmk(mrk) );
}

static	void	drw_rtx_(			// Drawing text on rect data
HDC		hdc,						// 描画対象デバイス
WCHAR	*txt,						// 文字列
RECT	*rct,						// 描画位置（ワールド座標系）
double	siz,						// 文字サイズ（ワールド座標単位）
int		col,						// 文字色
HWND	wnd )						// ウィンドウハンドル
{
	int					omd;
	HFONT				fnh;					// フォントハンドル
	HGDIOBJ				ofn;					// ﾃﾞﾌｫﾙﾄﾌｫﾝﾄ
	LOGFONT				fnt;		   			// フォント情報構造体
	COLORREF			otc;					// デフォルト色
	Fig_mng				*mng;

	mng = (Fig_mng *)(LONG_PTR)GetWindowLong( wnd, GWL_USERDATA );

	if ( mng ){
		memset( &fnt, 0, sizeof(LOGFONT) );

		fnt.lfCharSet = DEFAULT_CHARSET;			//SHIFTJIS_CHARSET;
		wcscpy_s( fnt.lfFaceName, memcnt(fnt.lfFaceName), _T("MS UI Gothic") );	// フォント名
		fnt.lfHeight = (int)(siz * mng->frt);		// フォントサイズ
		if ( !fnt.lfHeight )	fnt.lfHeight = 1;	// 最低値
		fnt.lfWeight = 400;			   				// スタイル～ボールド（400,700)
		fnt.lfQuality = PROOF_QUALITY;
		fnh = CreateFontIndirect( &fnt );
		if ( fnh != NULL ){
			ofn = SelectObject( hdc, fnh );
			otc = GetTextColor( hdc );
			SetTextColor( hdc, col );
			omd = SetBkMode( hdc, TRANSPARENT );
			DrawText( hdc, txt, (int)wcslen(txt), rct, DT_CENTER|DT_VCENTER|DT_TOP );
			SetBkMode( hdc, omd );
			SelectObject( hdc, ofn );
			SetTextColor( hdc, otc );
			DeleteObject( fnh );
		}
	}
}

static	int		drw_rct_(			// 矩形要素描画
HDC			hdc,					// 描画対象デバイス
Fig_elm		*elm,					// 幾何構造ポインタ
Fig_mng		*mng,					// 図形管理構造
HWND		wnd )					// ウィンドウハンドル
{
	Dpn			*dvc, wld[2];
	Trm_mrk		*mrk, *mpt;
	int			i, chk=TRUE, top, lft, btm, rit, cnt;
	POINT		*pnt;

	if ( elm->mrk&(MRK_RIT|MRK_LFT) ){
		if ( ( cnt = Add_Mrk( (void *)&mrk, elm, mng ) ) != -1 ){
			for ( i=0, mpt=mrk; i<cnt; i++ ){
				mpt = drw_mrk_( hdc, mpt, mng );
			}
			free( mrk );
		}
		else{
			chk = FALSE;
		}
	}

	if ( chk ){
		pnt = (POINT *)calloc( 5, sizeof(POINT) );
		dvc = (Dpn *)calloc( 2, sizeof(Dpn) );

		if ( dvc && pnt ){
			cnt = 5;
			wld[0] = *elm->elm.rct.sp;
			wld[1] = *elm->elm.rct.ep;

			Dpn_Cnv( dvc, wld, 2, mng->s2d );

			top = (int)min_( dvc[0].y, dvc[1].y );
			lft = (int)min_( dvc[0].x, dvc[1].x );
			btm = (int)max_( dvc[0].y, dvc[1].y );
			rit = (int)max_( dvc[0].x, dvc[1].x );

			pnt[0].x = lft;	pnt[0].y = top;	pnt[1].x = rit;	pnt[1].y = top;
			pnt[2].x = rit;	pnt[2].y = btm;	pnt[3].x = lft;	pnt[3].y = btm;
			pnt[4].x = lft;	pnt[4].y = top;
			if ( !(elm->mrk&(MRK_RIT|MRK_LFT)) ) chk = Polygon( hdc, pnt, cnt );
			//if ( elm->elm.rct.txt ){
				RECT		rct[1];
				rct->top = top;
				rct->bottom = btm;
				rct->left = lft;
				rct->right = rit;

				//drw_rtx_( hdc, elm->elm.rct.txt, rct,12, COL_WHT, wnd );
			//}
			if ( mng->dcb )	mng->dcb( hdc, elm, drw_txt_, wnd, mng->usr );
		}
	}

	if ( chk && elm->sel ){
		drw_smk_( hdc, pnt, 4 );
	}

	if ( pnt )		free( pnt );
	if ( dvc )		free( dvc );

	return( chk );
}

static	int		drw_elm_(			// 図形要素描画
HDC			hdc,					// 描画対象デバイス
Fig_elm		*elm,					// 幾何構造ポインタ
Fig_mng		*mng,					// 図形管理構造
HWND		wnd )					// ウィンドウハンドル
{
	HBRUSH			brs, obs;
	HPEN			pen, opn;
	int				chk=TRUE;

	if ( elm->vis ){
		pen = CreatePen( PS_SOLID, 1, elm->pcl );
		brs = CreateSolidBrush( elm->bcl );
		if ( pen && brs ){
			opn = (HPEN)SelectObject( hdc, pen );
			obs = (HBRUSH)SelectObject( hdc, brs );
			switch( elm->typ ){
			case ELM_LIN:	drw_lin_( hdc, elm, mng, wnd );	break;
			case ELM_RCT:	drw_rct_( hdc, elm, mng, wnd );	break;
			}

			SelectObject( hdc, opn );
			SelectObject( hdc, obs );
		}
		else{
			chk = FALSE;
		}
		if ( pen )	DeleteObject( pen );
		if ( brs )	DeleteObject( brs );
	}
	return( chk );
}

static	int		drw_lop_(			// 図形要素描画
HDC			hdc,					// 描画対象デバイス
Csr			rot,					// 幾何構造根
Fig_mng		*mng,					// 図形管理構造
int			sbd,					// 下層レイヤ描画
HWND		wnd )					// ウィンドウハンドル
{
	Csr				csr=NULL;
	Fig_elm			*elm;
	int				chk=TRUE;

	while( ( csr = Nxt_Lnk( rot, csr ) ) != NULL ){
		elm = Rec_Of(Fig_elm, csr);
		if ( ( chk = drw_elm_( hdc, elm, mng, wnd ) ) == FALSE )	break;
		if ( sbd ){
			if ( ( chk = drw_lop_( hdc, elm->sbl, mng, sbd, wnd ) ) == FALSE ){
				break;
			}
		}
	}
	return( chk );
}

int		Fig_Drw(			// 全図形描画処理
HDC		hdc,						// 描画対象デバイスコンテキスト
HWND	wnd )						// ウィンドウハンドル
{
	Fig_mng			*mng;

	mng = (Fig_mng *)(LONG_PTR)GetWindowLong( wnd, GWL_USERDATA );
	if ( mng ){
		if ( mng )	drw_lop_( hdc, mng->top, mng, TRUE, wnd );
	}
	return( !!mng );
}

void	Drw_Elm(					// 単一幾何図形描画
Fig_mng		*mng,					// 描画管理構造
Fig_elm		*elm )					// 幾何構造ポインタ
{
	HDC		hdc;

	hdc = GetDC( mng->wnd );
	drw_elm_( hdc, elm, mng, mng->wnd );
	ReleaseDC( mng->wnd, hdc );
}
