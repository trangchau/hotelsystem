/*------------------------------------------
    工数入力プログラム for Windows Ver 5.0
		Copyright 2001 S.Nagaoka
			印刷関係関数ファイル
--------------------------------------------*/
#include <afx.h>
#include <Windows.h>
#include <Winnt.h>
#include <math.h>
#include <stdlib.h>
#include "..\Common\bsc_def.h"
#include "afn_cnv.h"
#include "print.h"
#include "dib_ctl.h"

//void	GetDIBSizes_( HBITMAP, int *, int * );
//int		GetDIB_( HBITMAP, HPALETTE, BITMAPINFO *, char * );

void	Get_Pdv_Inf(			// プリンター情報取得
Prn_mng		*mng,				// 座標変換マトリックス
int			mod )				// 印刷モード
{
	Fld_def		out, inp;						// 出力側、入力側
	HDC			dph;							// ﾃﾞｨｽﾌﾟﾚｲDCﾊﾝﾄﾞﾙ
	const int	ppi = 72;						// ﾎﾟｲﾝﾄ/inch
	const float	mpi = (float)25.4;						// mm/inch
	double sqrt( double );

	if ( mod == MOD_PRN ){
		if ( mng->hdc ){
			mng->dev->xmm = GetDeviceCaps( mng->hdc, HORZSIZE );	  	// 印刷可能領域(mm)
			mng->dev->ymm = GetDeviceCaps( mng->hdc, VERTSIZE );	  	// 印刷可能領域(mm)
			mng->dev->xpc = GetDeviceCaps( mng->hdc, HORZRES );  	  	// 印刷可能領域(ﾄﾞｯﾄ)
			mng->dev->ypc = GetDeviceCaps( mng->hdc, VERTRES ); 	  	// 印刷可能領域(ﾄﾞｯﾄ)
			mng->dev->hmg = GetDeviceCaps( mng->hdc, PHYSICALOFFSETX );// 水平マージン(ﾄﾞｯﾄ)
			mng->dev->vmg = GetDeviceCaps( mng->hdc, PHYSICALOFFSETY );// 垂直マージン(ﾄﾞｯﾄ)
			out.sx = 0;
			out.ey = 0;
			out.ex = (double)mng->dev->xpc / mng->dev->xmm;	// 1mm当たりのﾄﾞｯﾄ数（ｘ）
			out.sy = (double)mng->dev->ypc / mng->dev->ymm;	// 1mm当たりのﾄﾞｯﾄ数（Ｙ）
			inp.sx = inp.ey = 0;
			inp.ex = inp.sy = 1 / MM_UNT;
			Get_Mtx( mng->mtx, &out, &inp, DEF_MTX );
			mng->dev->dpp =  (float)mng->dev->ypc / mng->dev->ymm  * mpi / ppi;
		}
	}
	if ( mod == MOD_PRV ){
		dph = CreateIC( _T("DISPLAY"), NULL, NULL, NULL );
		if ( dph ){
			mng->dev->xmm = GetDeviceCaps( dph, HORZSIZE );// 物理画面の幅 (mm)
			mng->dev->ymm = GetDeviceCaps( dph, VERTSIZE );// 物理画面の高さ (mm)
			mng->dev->xpc = GetDeviceCaps( dph, HORZRES ); // ディスプレイの幅(ピクセル単位)
			mng->dev->ypc = GetDeviceCaps( dph, VERTRES ); // ディスプレイの高さ(ラスタ行単位)
			out.sx = out.ey = 0;
			out.ex = (double)mng->dev->xpc / mng->dev->xmm;	// 1mm当たりのﾋﾟｸｾﾙ数（ｘ）
			out.sy = (double)mng->dev->ypc / mng->dev->ymm;	// 1mm当たりのﾋﾟｸｾﾙ数（Ｙ）
			inp.sx = inp.ey =0;
//			inp.ex = inp.sy = 1 / MM_UNT / ( (double)FScale / 100 );//拡大率（辺）
			inp.ex = inp.sy = ( 1 / MM_UNT ) / ( sqrt ( mng->prv->scl ) / 10 );//拡大率（面積）
			Get_Mtx( mng->mtx, &out, &inp, DEF_MTX );
			mng->dev->dpp =  (float)(mng->dev->ypc / mng->dev->ymm  * mpi / ppi
														* sqrt( mng->prv->scl / 100.0 ));
			DeleteDC( dph );
		}
	}
}

static	void	mak_pen_(				// ペンハンドル生成
HPEN		*pen,						// ペンハンドル
int			sty,						// スタイル
int			wid,						// 線幅（1/10mm）
int			col )						// 線色
{
	LOGBRUSH	brs;						// ブラシ構造体

	brs.lbStyle =  BS_SOLID;
	brs.lbColor = col;
	brs.lbHatch = 0;

	*pen = ExtCreatePen( PS_GEOMETRIC | sty | PS_ENDCAP_SQUARE | PS_JOIN_MITER,
													wid, &brs, 0, NULL );
}

static	void	slc_brs_(				// ブラシモード選択
int			brs,						// ユーザー選択ブラシ
int			*htg,						// ハッチング有無
int			*mod )						// ハッチングモード
{
	if ( brs == BRS_SLD ){
		*htg = 0;
		*mod = 0;
	}
	else{
		*htg = 1;
		switch( brs ){
		case	BRS_BDI:	*mod = HS_BDIAGONAL;
							break;
		case	BRS_CRS:	*mod = HS_CROSS;
							break;
		case	BRS_DCS:	*mod = HS_DIAGCROSS;
							break;
		case	BRS_FDI:	*mod = HS_FDIAGONAL;
							break;
		case	BRS_HRZ:	*mod = HS_HORIZONTAL;
							break;
		case	BRS_VRT:	*mod = HS_VERTICAL;
							break;
		default:			*htg = 0;
							*mod = 0;
		}
	}
}

static	void	out_txt_(				// テキストタイプ出力
HDC			dvh,						// デバイスハンドル
Txt_def		*dat, 						// データポインタ
Prn_mng		*mng,						// 印刷データ構造
int			mag )						// マージン有無
{
	int					i, omd;
	RECT				rct;					// 出力矩形
	Dpn					bfr[2], aft[2];			// 座標変換前、後
	HFONT				fnh;					// フォントハンドル
	HGDIOBJ				ofn;					// ﾃﾞﾌｫﾙﾄﾌｫﾝﾄ
	LOGFONT				fnt;		   			// フォント情報構造体
	COLORREF			otc, obc;				// デフォルト色

	memset( &fnt, 0, sizeof( LOGFONT ) );
	if ( dvh && dat->vsb && dat->txt ){
		bfr[0].x = dat->xps;
		bfr[0].y = dat->yps;
		bfr[1].x = dat->xps + dat->wid;
		bfr[1].y = dat->yps + dat->hgt;
		Dpn_Cnv( aft, bfr, 2, mng->mtx );			// 変換座標取得

		if ( mag ){
			for ( i=0; i<2; i++ ){
				aft[i].x -= mng->dev->hmg;
				aft[i].y -= mng->dev->vmg;
			}
		}
		rct.left = (long)aft[0].x;
		rct.top =  (long)aft[0].y;
		rct.right = (long)aft[1].x;
		rct.bottom = (long)aft[1].y;

		fnt.lfCharSet = DEFAULT_CHARSET;//SHIFTJIS_CHARSET;
		wcscpy_s( fnt.lfFaceName, memcnt(fnt.lfFaceName), dat->fnt );			// フォント名
		fnt.lfHeight = (long)(mng->dev->dpp * dat->siz + 0.5);// フォントサイズ
		if ( !fnt.lfHeight )	fnt.lfHeight = 1;	// 最低値
		fnt.lfUnderline = !!( dat->uln );			// 文字飾り～下線
		fnt.lfStrikeOut = !!( dat->cln );			// 文字飾り～打消線
		fnt.lfItalic = !!( dat->itl ); 			  	// スタイル～斜体
		fnt.lfWeight = dat->bld;	   				// スタイル～ボールド（400,700)
		fnt.lfQuality = PROOF_QUALITY;
		fnh = CreateFontIndirect( &fnt );
		if ( fnh != NULL ){
			ofn = SelectObject( dvh, fnh );
			otc = GetTextColor( dvh );
			SetTextColor( dvh, dat->fcl );
			obc = GetBkColor( dvh );
			SetBkColor( dvh, dat->bcl );
			if ( dat->fcl == dat->bcl )
				omd = SetBkMode( dvh, TRANSPARENT );// トランスペアレント設定
			DrawText( dvh, dat->txt, -1, &rct, dat->mod );
			if ( dat->fcl == dat->bcl )
				SetBkMode( dvh, omd );
			SelectObject( dvh, ofn );
			SetTextColor( dvh, otc );
			SetBkColor( dvh, obc );
			DeleteObject( fnh );
		}
	}
}

static	void	out_arc_(				// 円弧タイプ出力
HDC			dvh,						// デバイスハンドル
Arc_def		*dat, 						// データポインタ
Prn_mng		*mng,						// 印刷データ構造
int			mag )						// マージン有無
{
	int				i;
	int				htg, mod;					// ハッチングモード
	Dpn				bfr[6], aft[6];				// 座標変換前、後
	HPEN			pen;						// ペン
	HBRUSH			brs;						// ブラシ
	HGDIOBJ			opn, obs;					// ﾃﾞﾌｫﾙﾄﾍﾟﾝ、ﾌﾞﾗｼobj
	static	double	cst = 10000.0;

	if ( dvh && dat->vsb ){
		bfr[0].x = dat->xps - dat->xrs;			// 矩形left
		bfr[0].y = dat->yps - dat->yrs;			// 矩形top
		bfr[1].x = dat->xps + dat->xrs;			// 矩形right
		bfr[1].y = dat->yps + dat->yrs;			// 矩形bottom
		bfr[2].x = dat->xps;					// 中心ｘ座標
		bfr[2].y = dat->yps;					// 中心ｙ座標
		bfr[3].x = 0;				   			// 線幅
		bfr[3].y = dat->lnw;		   			// 線幅
		if ( dat->typ != TYP_CCL ){
		//  円弧の始終点角は右手系にて指定の為、符号逆転
			bfr[4].x = bfr[2].x + cst * cos( -dat->brd );	// 開始角ｘ座標
			bfr[4].y = bfr[2].y + cst * sin( -dat->brd );	// 開始角ｙ座標
			bfr[5].x = bfr[2].x + cst * cos( -dat->erd );	// 終了角ｘ座標
			bfr[5].y = bfr[2].y + cst * sin( -dat->erd );	// 終了角ｙ座標
		}
		Dpn_Cnv( aft, bfr, 6, mng->mtx );		// 変換座標取得
		if ( mag ){
			for ( i=0; i<=2; i++ ){
				aft[i].x -= mng->dev->hmg;
				aft[i].y -= mng->dev->vmg;
			}
		}
		if ( dat->lnw > 0 ){					// ペン作成
			if ( aft[3].y > 0 && aft[3].y < 1 )		aft[3].y = 1;
			mak_pen_( &pen, dat->pns, (int)aft[3].y, dat->fcl );
		}
		else	pen = (HPEN)::GetStockObject( NULL_PEN );


		if ( dat->brs && dat->typ != TYP_ARC ){	// ブラシ作成
			slc_brs_( dat->brs, &htg, &mod );
			if ( htg )			brs = CreateHatchBrush( mod, dat->bcl );
			else				brs = CreateSolidBrush( dat->bcl );
		}
		else					brs = (HBRUSH)::GetStockObject( NULL_BRUSH );

		if ( pen &&  ( brs || dat->typ == TYP_ARC ) ){
			opn = SelectObject( dvh, pen );
			obs = SelectObject( dvh, brs );
			switch ( dat->typ ){
			case	TYP_ARC:					// 円弧タイプ
				Arc( dvh, (int)aft[0].x, (int)aft[0].y, (int)aft[1].x, (int)aft[1].y,
						(int)aft[4].x, (int)aft[4].y, (int)aft[5].x, (int)aft[5].y );
				break;
			case	TYP_CHD:					// 弦タイプ
				Chord( dvh, (int)aft[0].x, (int)aft[0].y, (int)aft[1].x, (int)aft[1].y,
						(int)aft[4].x, (int)aft[4].y, (int)aft[5].x, (int)aft[5].y );
				break;
			case	TYP_PIE:					// 扇タイプ
				Pie( dvh, (int)aft[0].x, (int)aft[0].y, (int)aft[1].x, (int)aft[1].y,
						(int)aft[4].x, (int)aft[4].y, (int)aft[5].x, (int)aft[5].y );
				break;
			case	TYP_CCL:					// 円タイプ
				Ellipse( dvh, (int)aft[0].x, (int)aft[0].y, (int)aft[1].x, (int)aft[1].y );
				break;
			}
			SelectObject( dvh, opn );
			SelectObject( dvh, obs );
			if ( dat->lnw > 0 )	DeleteObject( pen );
			if ( dat->brs || dat->typ != TYP_ARC )	DeleteObject( brs );
		}
	}
}

static	void	out_rct_(				// 矩形タイプ出力
HDC			dvh,						// デバイスハンドル
Rct_def		*dat, 						// データポインタ
Prn_mng		*mng,						// 印刷データ構造
int			mag )						// マージン有無
{
	int				lft, top, rgt, btm;			// 印刷位置
	int				htg, mod;					// ハッチングモード
	Dpn				bfr[4], aft[4];				// 座標変換前、後
	HPEN			pen;						// ペン
	HBRUSH			brs;						// ブラシ
	HGDIOBJ			opn, obs;					// ﾃﾞﾌｫﾙﾄﾍﾟﾝ、ﾌﾞﾗｼobj

	if ( dvh && dat->vsb ){
		bfr[0].x = dat->xps;
		bfr[0].y = dat->yps;
		bfr[1].x = dat->wid;
		bfr[1].y = dat->hgt;
		bfr[2].x = 0;
		bfr[2].y = dat->lnw;
		bfr[3].x = 0;
		bfr[3].y = 40;							// 角丸半径(固定)

		Dpn_Cnv( aft, bfr, 4, mng->mtx );		// 変換座標取得
		lft = (int)aft[0].x ;
		top = (int)aft[0].y ;
		rgt = (int)(aft[0].x + aft[1].x);
		btm = (int)(aft[0].y + aft[1].y);
		rgt = ( rgt == lft )?	lft + 1 : rgt;
		btm = ( btm == top )?	top + 1 : btm;
		if ( mag ){
			lft -= mng->dev->hmg;
			top -= mng->dev->vmg;
			rgt -= mng->dev->hmg;
			btm -= mng->dev->vmg;
		}
		if ( dat->lnw > 0 ){					// ペン作成
			if ( aft[2].y > 0 && aft[2].y < 1 )		aft[2].y = 1;
			mak_pen_( &pen, dat->pns, (int)aft[2].y, dat->fcl );
		}
		else	pen = (HPEN)::GetStockObject( NULL_PEN );
		if ( dat->brs ){						// ブラシ作成
			slc_brs_( dat->brs, &htg, &mod );
			if ( htg )			brs = CreateHatchBrush( mod, dat->bcl );
			else				brs = CreateSolidBrush( dat->bcl );
		}
		else					brs = (HBRUSH)::GetStockObject( NULL_BRUSH );

		if ( pen && brs ){
			opn = SelectObject( dvh, pen );
			obs = SelectObject( dvh, brs );
			if ( dat->rnd ){
				RoundRect( dvh, lft, top, rgt, btm, (int)aft[3].y, (int)aft[3].y );
			}
			else	Rectangle( dvh, lft, top, rgt, btm );
			SelectObject( dvh, opn );
			SelectObject( dvh, obs );
			if ( dat->lnw > 0 )	DeleteObject( pen );
			if ( dat->brs )		DeleteObject( brs );
		}
	}
}

static void out_lin_(					// 線タイプ印刷
HDC			dvh,						// デバイスハンドル
Lin_def		*dat, 						// データポインタ
Prn_mng		*mng,						// 印刷データ構造
int			mag )						// マージン有無
{
	int				bgx, bgy, edx, edy;			// 印刷位置
	Dpn				bfr[3], aft[3];				// 座標変換前、後
	HPEN			pen = NULL;					// ペンハンドル
	HGDIOBJ			opn;						// ﾃﾞﾌｫﾙﾄﾍﾟﾝ

	if ( dvh && dat->vsb && dat->lnw > 0 ){
		bfr[0].x = dat->bgx;
		bfr[0].y = dat->bgy;
		bfr[1].x = dat->edx;
		bfr[1].y = dat->edy;
		bfr[2].x = 0;
		bfr[2].y = dat->lnw;
		Dpn_Cnv( aft, bfr, 3, mng->mtx );		// 変換座標取得
		bgx = (int)aft[0].x;
		bgy = (int)aft[0].y;
		edx = (int)aft[1].x;
		edy = (int)aft[1].y;

		if ( mag ){
			bgx -= mng->dev->hmg;
			bgy -= mng->dev->vmg;
			edx -= mng->dev->hmg;
			edy -= mng->dev->vmg;
		}
		if ( aft[2].y > 0 && aft[2].y < 1 )		aft[2].y = 1;
		mak_pen_( &pen, dat->pns, (int)aft[2].y, dat->fcl );
		if ( pen ){
			opn = SelectObject( dvh, pen );
			MoveToEx( dvh, bgx, bgy, NULL );
			LineTo( dvh, edx, edy );
			SelectObject( dvh, opn );
			DeleteObject( pen );
		}
	}
}

static	void	out_bmp_(				// ビットマップタイプ出力
HDC			dvh,						// デバイスハンドル
Bmp_def		*dat,						// データポインタ
Prn_mng		*mng,						// 印刷データ構造
int			mag )						// マージン有無
{
	int			omd, ifs;
	DWORD		ims;
	char		*img;
	int			chk;
	HBITMAP		bmp;
	BITMAPINFO	*bif;
	Dpn			bfr[2], aft[2];					// 座標変換前、後

	if ( dvh && dat->vsb && mng->ins && dat->bmp ){
		bfr[0].x = dat->xps;
		bfr[0].y = dat->yps;
		bfr[1].x = dat->wid;
		bfr[1].y = dat->hgt;
		Dpn_Cnv( aft, bfr, 2, mng->mtx );			// 変換座標取得
		if ( mag ){
			aft[0].x -= mng->dev->hmg;
			aft[0].y -= mng->dev->vmg;
		}
		chk = GetDeviceCaps( dvh, RASTERCAPS ) & (RC_STRETCHBLT|RC_PALETTE);
		if ( chk ){
			bmp = LoadBitmap( mng->ins, dat->bmp );
			if ( bmp ){
				GetDIBSizes_( bmp, &ifs, &ims );
				bif = (BITMAPINFO *)calloc(1, ifs);
				img = (char *)calloc(1, ims);
				if ( bif && img ){
					GetDIB_( bmp, NULL, bif, img );
					omd = SetStretchBltMode( dvh, COLORONCOLOR );
					StretchDIBits( dvh,
						(int)aft[0].x, (int)aft[0].y, (int)aft[1].x, (int)aft[1].y,
						0, 0, bif->bmiHeader.biWidth, bif->bmiHeader.biHeight,
						img, bif, DIB_RGB_COLORS, SRCCOPY );
					SetStretchBltMode( dvh, omd );
				}
				if ( bif )	free( bif );
				if ( img )	free( img );
			}
		}
	}
}

static void out_blk_(					//	ブロックタイプ出力
HDC			dvh,						// デバイスハンドル
Blk_def		*dat,						// データポインタ
Prn_mng		*mng,						// 印刷データ構造
int			mag,						// マージン有無
HRGN		org )						// デフォルトリージョン(Prvのみ)
{
	int			i;
	void		*ptr;							// データポインタ
	Rct_def		flm;							// ブロック枠
	POINT		opt;							// 以前の原点位置
	HRGN		rgn;							// クリップリージョン
	Dpn			bfr[2], aft[2];					// 座標変換前、後

	if ( dvh && dat->vsb ){

		if ( dat->lnw > 0 || dat->brs ){		// 枠出力
			flm.xps = dat->xps - dat->lnw / 2;
			flm.yps = dat->yps - dat->lnw / 2;
			flm.wid = dat->wid + dat->lnw;
			flm.hgt = dat->hgt + dat->lnw;
			flm.pns = dat->pns;
			flm.lnw = dat->lnw;
			flm.fcl = dat->fcl;
			flm.brs = dat->brs;
			flm.bcl = dat->bcl;
			flm.rnd = 0;
			flm.vsb = true;
			out_rct_( dvh, &flm, mng, mag );
		}

		bfr[0].x = dat->xps;
		bfr[0].y = dat->yps;
		bfr[1].x = dat->xps + dat->wid;
		bfr[1].y = dat->yps + dat->hgt;
		Dpn_Cnv( aft, bfr, 2, mng->mtx );
		if ( mag ){
			for( i=0; i<2; i++ ){
				aft[i].x -= mng->dev->hmg;
				aft[i].y -= mng->dev->vmg;
			}
		}
		SetViewportOrgEx( dvh, (int)aft[0].x,
									(int)aft[0].y, &opt );// 原点位置変更
		if ( !mag ){
			SetViewportOrgEx( dvh, (int)aft[0].x + opt.x,
										(int)aft[0].y + opt.y, NULL );
			aft[0].x += opt.x;
			aft[0].y += opt.y;
			aft[1].x += opt.x;
			aft[1].y += opt.y;
		}
		rgn = CreateRectRgn( (int)aft[0].x, (int)aft[0].y, (int)aft[1].x, (int)aft[1].y );

		if ( rgn ){
			ExtSelectClipRgn( dvh, rgn, RGN_AND );			// 重複リージョン
			if ( ( ptr = dat->txt ) != NULL ){
				for ( i=0; i < dat->txc; i++ ){
					out_txt_( dvh, (Txt_def *)ptr, mng, false );	// 文字列出力
					ptr = ((Txt_def *)ptr) + 1;
				}
			}
			if ( ( ptr = dat->rct ) != NULL ){
				for ( i=0; i < dat->rcc; i++ ){
					out_rct_( dvh, (Rct_def *)ptr, mng, false );	// 矩形出力
					ptr = ((Rct_def *)ptr) + 1;
				}
			}
			if ( ( ptr = dat->lin ) != NULL ){
				for ( i=0; i < dat->lic; i++ ){
					out_lin_( dvh, (Lin_def *)ptr, mng, false );	// 線出力
					ptr = ((Lin_def *)ptr) + 1;
				}
			}
			if ( ( ptr = dat->arc ) != NULL ){
				for ( i=0; i < dat->acc; i++ ){
					out_arc_( dvh, (Arc_def *)ptr, mng, false );	// 円弧出力
					ptr = ((Arc_def *)ptr) + 1;
				}
			}
			if ( ( ptr = dat->bmp ) != NULL ){
				for ( i=0; i < dat->bmc; i++ ){
					out_bmp_( dvh, (Bmp_def *)ptr, mng, false );	// ﾋﾞｯﾄﾏｯﾌﾟ出力
					ptr = ((Bmp_def *)ptr) + 1;
				}
			}
			SelectObject( dvh, org );
			DeleteObject( rgn );
		}
		SetViewportOrgEx( dvh, opt.x, opt.y, NULL );
	}
}

void	Prn_Out_Dat(					// 印刷データ出力
HDC			dvh,						// デバイスハンドル
Prn_mng		*mng, 						// 印刷データ
int			mod,						// 出力先
HRGN		rgn )						// デフォルトリージョン
{
	int			cnt;
	int			mag;					// マージン指定
	Txt_def		*txt = mng->prn->txt;
	Rct_def		*rct = mng->prn->rct;
	Lin_def		*lin = mng->prn->lin;
	Arc_def		*arc = mng->prn->arc;
	Bmp_def		*bmp = mng->prn->bmp;
	Blk_def		*blk = mng->prn->blk;

	mag = ( mod == MOD_PRN )? true : false;
	if ( bmp ){
		for ( cnt = 0; cnt < mng->prn->bmc; cnt++ ){
			if ( bmp && bmp->vsb )
				out_bmp_( dvh, bmp, mng, mag );
			bmp++;
		}
	}
	if ( txt ){
		for ( cnt = 0; cnt < mng->prn->txc; cnt++ ){
			if ( txt && txt->vsb )
				out_txt_( dvh, txt, mng, mag );
			txt++;
		}
	}
	if ( rct ){
		for ( cnt = 0; cnt < mng->prn->rcc; cnt++ ){
			if ( rct && rct->vsb )
				out_rct_( dvh, rct, mng, mag );
			rct++;
		}
	}
	if ( lin ){
		for ( cnt = 0; cnt < mng->prn->lic; cnt++ ){
			if ( lin && lin->vsb )
				out_lin_( dvh, lin, mng, mag );
			lin++;
		}
	}
	if ( arc ){
		for ( cnt = 0; cnt < mng->prn->acc; cnt++ ){
			if ( arc && arc->vsb )
				out_arc_( dvh, arc, mng, mag );
			arc++;
		}
	}
	if ( blk ){
		for ( cnt = 0; cnt < mng->prn->blc; cnt++ ){
			if ( blk && blk->vsb )
				out_blk_( dvh, blk, mng, mag, rgn );
			blk++;
		}
	}
}

int			Prn_Opn(				// プリンターオープン
WCHAR			*pnm,				// プリンター名
Prn_mng			*mng )				// 印刷管理構造体
{
	DOCINFO		doc[1];
	int			chk;

	if ( chk = !!( mng->hdc = CreateDC( _T("WINSPOOL"), pnm, NULL, NULL ) ) ){
		doc->cbSize = sizeof( DOCINFO );
		doc->lpszDocName = mng->ppr->ttl;
		doc->lpszOutput = (LPTSTR)NULL;
		if ( !( chk = StartDoc( mng->hdc, doc ) != SP_ERROR ) ){
			DeleteDC( mng->hdc );
			mng->hdc = NULL;
		}
	}
	return( chk );
}

int			Prn_Exc(				// 印刷実行
int				cpy,				// 出力部数
Prn_mng			*mng )				// 印刷管理構造体
{
	int			chk, i;
	HRGN		rgn;

	if ( chk = !!mng->hdc ){
		rgn = CreateRectRgn( 0, 0,
				mng->dev->xpc + mng->dev->hmg, mng->dev->ypc + mng->dev->vmg );
		if ( chk = !!rgn ){
			SelectObject( mng->hdc, rgn );
			for ( i=0; chk && i<cpy; i++ ){
				if ( chk = StartPage( mng->hdc ) != SP_ERROR ){
					Prn_Out_Dat( mng->hdc, mng, MOD_PRN, rgn );
				}
				EndPage( mng->hdc );
			}
			DeleteObject( rgn );
		}
	}
	return( chk );
}

int			Prn_Cls(				// プリンタークローズ
Prn_mng			*mng )				// 印刷管理構造体
{
	int			chk;

	if ( chk = !!mng->hdc ){
		EndDoc( mng->hdc );
		DeleteDC( mng->hdc );
	}
	return( chk );
}
