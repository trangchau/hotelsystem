/*
	Hotel Property Management System
	Booking status graphic manipration functions
*/

#include <afx.h>
#include <afxwin.h>
#include <stdio.h>
#include <math.h>
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\lnk_mng.h"
#include "..\..\print\afn_cnv.h"
#include "..\fig\fig_mnp.h"
#include "..\fig\fig_ope.h"
#include "..\fig\fig_mrk.h"
#include "..\fig\rbr_bnd.h"
#include "..\MainFrm.h"

#include "bst_gph.h"
#include "bst_fnc.h"
#include "bst_sql.h"

// Symbole of terminal on the graphic element ( These datas are readonly )
static	Dpn			Alw_lft[] = { { 37, 3 }, { 30, 17 }, { 37, 31 } };
static	Dpn			Alw_rit[] = { {  5, 3 }, { 12, 17 }, {  5, 31 } };
static	Dpn			Rct_lft[] = { { 46, 3 }, { 40,  9 }, { 40, 25 }, { 46, 31 }};
static	Dpn			Rct_rit[] = { { 16, 3 }, { 22,  9 }, { 22, 25 }, { 16, 31 }};

#define	GFT_SIZ			12.0						// Font size of gage drawing(Unit:Point)
#define	VIW_MGN			(0.02)						// Displaying mergin
#define	day_gph_(x,y)	(dif_dys(y,x)*2*GPH_UNT)	// 日付→グラフ変換


static	int		ovr_lap_(				// 矩形の重なりテスト
Rct		*src,							// チェック基準
Rct		*obj )							// チェック対象
{
	return( 	src->sp->x<obj->ep->x && obj->sp->x<src->ep->x
			&&	src->sp->y<obj->ep->y && obj->sp->y<src->ep->y );
}

static int clc_dys( Day *dte )
{
	int		dy, c, dl, dm;
	if (dte->mth <= 2)	{					// 1・2月 → 前年の13・14月
		--dte->yer;
		dte->mth += 12;
	}

	dy = 365 * (dte->yer - 1);				// 経過年数×365日
	c = dte->yer / 100;
	dl = (dte->yer >> 2) - c + (c >> 2);	// うるう年分
	dm = (dte->mth * 979 - 1033) >> 5;		// 1月1日から m 月1日までの日数

	return( dy + dl + dm + dte->day - 1 );
}

static	int		dif_dys(				// 日数(泊数)計算
Day		*sta,							// 開始日付
Day		*end )							// 終了日付
{
	return( clc_dys(sta) - clc_dys(end) );
}

static	void	gag_cbk_(				// Call back function from Fig_Mnp on gage drawing 
HDC		hdc,							// Windows device context
void	*ptr,							// Pointer of graphic element
Drw_txt	dtx,							// Pointer of function which drawing text
HWND	wnd,							// Window handle
LPVOID	usr )							// Data of user definition
{
	Fig_elm		*elm = (Fig_elm *)ptr;
	GAG_DAT		*gag;
	WCHAR		buf[20];
	Dpn			pos[2];

	if ( elm->lyr == LYR_VGG ){
		gag = (GAG_DAT *)(elm+1);
		if ( gag->mth != -1 ){
			swprintf_s( buf, memcnt(buf), _T("%d/%d"), gag->mth, gag->day );
			pos->x = elm->srn.sp.x;
			pos->y = elm->srn.ep.y;
			(pos+1)->x = elm->srn.ep.x;
			(pos+1)->y = elm->srn.sp.y;
			dtx( hdc, buf, pos, pos+1, GFT_SIZ, COL_BLK, wnd );
		}
	}
}

static	void	drw_cbk_(				// Call back function on drawing each element
HDC		hdc,							// device context
void	*ptr,							// pointer of element
Drw_txt	dtx,							// pointer of drawing text function
HWND	wnd,							// window handle
LPVOID	usr )							// pointer of user data
{
	Fig_elm		*elm = (Fig_elm *)ptr;
	WCHAR		buf[520];
	Dpn			pos[2];
	BST_ROM_STS	*sts;

//	if ( elm->lyr == LYR_DAT ){
		if ( sts = (BST_ROM_STS *)(elm->usr) ){
			pos->x = elm->elm.rct.sp->x;
			pos->y = elm->elm.rct.ep->y;
			(pos+1)->x = elm->elm.rct.ep->x;
			(pos+1)->y = elm->elm.rct.sp->y;
			swprintf_s( buf, memcnt(buf), _T("%s\n%s"), EDT_DBF_WST(sts->fnm, _T("")), EDT_DBF_WST(sts->lnm, _T("")) ); 
			dtx( hdc, buf, pos, pos+1, 12, COL_WHT, wnd );
		}
//	}
}

static	void	set_edf_(				// 図形要素規定値設定
Fig_elm	*dst,							// 格納領域
int		lyr,							// レイヤ番号
int		bcl,							// 背景色
int		typ )							// 図形種別
{
	dst->lyr = lyr;
	dst->pcl = COL_BLK,	dst->bcl = bcl;
	dst->vis = TRUE;
	dst->sel = FALSE;
	dst->typ = typ;
}

static	void	srd_box_(				// 包含矩形算出・格納
Fig_elm		*elm )						// 対象要素
{
	Dpn			sp, ep;

	switch( elm->typ ){
	case ELM_LIN:	sp = *elm->elm.lin.sp;	ep = *elm->elm.lin.ep; break;
	case ELM_RCT:	sp = *elm->elm.rct.sp;	ep = *elm->elm.rct.ep; break;
	}
	elm->srn.sp.x = min_( sp.x, ep.x );	elm->srn.sp.y = min_( sp.y, ep.y );
	elm->srn.ep.x = max_( sp.x, ep.x );	elm->srn.ep.y = max_( sp.y, ep.y );
}

static	void	sel_clr_(				// 全選択解除
Csr		rot,							// データ根
BST_MNG	*bst )							// Booking status graph management structure
{
	Fig_elm		*elm;
	Csr			csr=NULL;

	while( ( csr = Nxt_Lnk( rot, csr ) ) != NULL ){
		elm = Rec_Of(Fig_elm, csr);
		if ( elm->sel ){
			elm->sel = FALSE;
			Fig_Drw_Elm( bst->hnd, elm );
		}
	}
}

static	int		ins_chk_(				// 挿入可否検定
Rct		*dst,							// 被検査矩形
int		lyr,							// 行番号
Csr		*pos,							// 挿入位置
Csr		rot,							// 既存データ根
Csr		vid )							// 検査対象外データ
{
	Csr			csr=NULL;
	Fig_elm		*elm;

	*pos = NULL;
	while( ( csr = Nxt_Lnk( rot, csr ) ) != NULL ){
		elm = Rec_Of(Fig_elm, csr);
		if ( !is_sep_(elm->lyr) && elm->lyr==lyr ){
			if ( !vid || ( vid != csr ) ){
				if ( ovr_lap_( &elm->elm.rct, dst ) )	break;
				if ( !(*pos) && elm->elm.rct.sp->x >= dst->ep->x )	*pos = csr;
			}
		}
	}
	return( !csr );
}


static	int		exs_lin_(				// 指定行番号存在検査
Csr		rot,							// データ根
int		lyr )							// 指定行番号
{
	Csr			csr=NULL;
	Fig_elm		*elm;

	while( ( csr = Nxt_Lnk( rot, csr ) ) != NULL ){
		elm = Rec_Of(Fig_elm, csr);
		if ( is_sep_(elm->lyr) && lin_of_(elm->lyr)==lyr )	break;
	}
	return( !!csr );
}

static	void	set_elm_(				// 図形要素データ設定
Fig_elm	*elm,							// 格納領域
int		lyr,							// レイヤ
double	sx,								// Ｘ方向始点座標
double	sy,								// Ｙ方向始点座標
double	ex,								// Ｘ方向終点座標
int		bcl,							// 表示色
BST_MNG	*bst )							// Booking status graph management structure
{
	elm->lyr = lyr;
	elm->pcl = COL_BLK;	elm->bcl = bcl;
	elm->vis = TRUE;
	elm->typ = ELM_RCT;
	elm->elm.rct.sp->x = sx;	elm->elm.rct.sp->y = sy-ARW_HIT;
	elm->elm.rct.ep->x = ex;	elm->elm.rct.ep->y = sy;
	elm->elm.rct.rmk = bst->arn;
	elm->elm.rct.lmk = bst->aln;
	elm->mrk = MRK_LFT|MRK_RIT;
	srd_box_( elm );
}

static	int		rom_day_(				// Convert to room number and day
BST_ROM_LST		**rom,					// Room information storeing pointer
Day				*std,					// Start date storering pointer
Day				*edd,					// End date storering pointer
Dpn				*mss,					// Mouse drag start position
Dpn				*mse,					// Mouse drag end position( NULL if no need )
BST_MNG			*bst )					// Booking status graphic management structure
{
	Csr			csr;
	double		sx, ex, sy, ceil( double );
	int			lin, chk;

	if ( mse ){				// optimize coodinate
		sx=min( mss->x, mse->x );
		ex=max(mss->x, mse->x);
		mss->x=sx, mse->x=ex;
	}
	else{
		sx = mss->x;
	}

	sx = ceil( sx / (GPH_UNT * 2) - 1 );		// convert to days from mouse coodinate
	sy = ceil( mss->y / ( GPH_HIT + 1.0 ) );	// convert to line number(Room line) from mouse coodinate
	lin = (int)(sy*-1.0) + 1;
	if ( chk = !!( csr=Idx_Lnk( bst->rom, lin-1 ) ) ){
		if ( rom )	*rom = Rec_Of(BST_ROM_LST, csr);
		*std = *bst->std;
		Add_Dte(std, (int)sx );
		if ( !!edd && !!mse ){
			ex = ceil( ex / (GPH_UNT * 2) - 1 );
			*edd = *bst->std;
			Add_Dte(edd, (int)ex );
		}
	}

	return( chk );
}

static	int		crt_elm_(				// 図形要素の作成
Csr		rot,							// 格納先
Dpn		*mss,							// マウス操作開始座標
Dpn		*mse,							// マウス操作完了座標
int		typ,							// 追加される要素データ種別
BST_MNG	*bst )							// Booking status graph management structure
{
	Fig_elm		elm[1];
	Csr			csr=NULL/*, pos*/;
	//double		sx, ex, sy, ceil( double );
	int			chk/*, lyr, dus*/;
	Day			std[1], edd[1];
	BST_ROM_LST	*rom;
	GPH_NTF		*ntf;
	GPH_RDY		*rdy;

	memset( elm, 0, sizeof(elm) );

	if ( rom_day_( &rom, std, edd, mss, mse, bst ) ){
		if ( Cmp_Dte(std,bst->mng->bdt)>=0){		// Available after today
			if ( chk = !!( ntf = (GPH_NTF *)calloc( sizeof(GPH_NTF), 1 ) ) ){
				if ( chk = !!( rdy = (GPH_RDY *)calloc( 1, sizeof(GPH_RDY) ) ) ){
					ntf->ntf = GPN_CREATE;
					rdy->rom = rom;
					*rdy->nsd = *std;
					*rdy->ned = *edd;
					PostMessage( bst->par, WM_GRAPHNOTIFY, (WPARAM)(LONG_PTR)ntf, (WPARAM)(LONG_PTR)rdy );
/*
					sx = ceil( mss->x / (GPH_UNT*2) - 1 );		// convert to day point
					ex = ceil( mse->x / (GPH_UNT*2) - 1 );

					if ( dus = sx == ex ){
						sx = sx * (GPH_UNT*2);
						ex = ( ex + 1 ) * (GPH_UNT*2);
					}
					else{
						sx = sx * (GPH_UNT*2) + GPH_UNT;
						ex = ex * (GPH_UNT*2) + GPH_UNT;
					}

					sy = ceil( mss->y / ( GPH_HIT + 1.0 ) );
					lyr = (int)(sy*-1.0) + 1;
					sy = sy * ( GPH_HIT + 1.0 ) - 3.0;

					if ( sx != ex && exs_lin_( rot, lyr ) ){
						sel_clr_( rot, bst );
						set_elm_( elm, lyr, sx, sy, ex, dus ? COL_DUS: COL_STY, bst );
						elm->elm.rct.lmc = elm->bcl;
						elm->elm.rct.rmc = elm->bcl;
						elm->pcl = elm->bcl;
						elm->elm.rct.txt = NULL;
						elm->sel = TRUE;
						if ( ins_chk_( &elm->elm.rct, lyr, &pos, rot, NULL ) ){
							if ( pos )	csr = Ins_Lnk( rot, pos, (char *)elm, sizeof(Fig_elm) );
							else		csr = Add_Lnk( rot, (char *)elm, sizeof(Fig_elm) );
							if ( csr ){
								Fig_Drw_Elm( bst->hnd, elm );
							}
						}
					}
*/
				}
				else{
					free( ntf );
				}
			}
		}
	}
	return( !!csr );
}

static	int		drg_pos_(				// Position of dragging 
Dpn		*mss,							// マウス操作開始座標
Dpn		*mse,							// マウス操作完了座標
LPVOID	usr )							// ゆーざー定義データ
{
	BST_MNG		*mng = (BST_MNG *)usr;
	GPH_NTF		*ntf;
	GPH_RDY		*rdy;
	Day			std[1], edd[1];
	BST_ROM_LST	*rom;
	int			chk;

	if ( chk = rom_day_( &rom, std, edd, mss, mse, mng ) ){
		if ( chk = !!( ntf = (GPH_NTF *)calloc( sizeof(GPH_NTF), 1 ) ) ){
			if ( chk = !!( rdy= (GPH_RDY *)calloc( sizeof(GPH_RDY), 1 ) ) ) {
				ntf->ntf = GPN_DRAG;
				rdy->rom = rom;
				*rdy->nsd = *std;
				if ( mse )	*rdy->ned = *edd;
				PostMessage( mng->par, WM_GRAPHNOTIFY, (WPARAM)(LONG_PTR)ntf, (WPARAM)(LONG_PTR)rdy );
			}
		}
		else{
			free( ntf );
		}
	}

	return( chk );
}

static	int		mov_exc_(
Fig_elm		*elm,						// 移動対象図形
Dpn			*mss,
Dpn			*mse,
BST_MNG		*bst )						// Booking status management structure
{
	Day			std[1], edd[1];
	BST_ROM_LST	*rom;
	BST_ROM_STS	*sts;
	GPH_NTF		*ntf;
	GPH_RDY		*rdy;
	int			chk, exc;

	sts = (BST_ROM_STS *)elm->usr;

	if ( chk = rom_day_( &rom, std, edd, mss, mse, bst ) ){
		exc  = Cmp_Dte( DBS_DAY_PTR(sts->req->usd), std ) != 0;
		exc |= Cmp_Dte( DBS_DAY_PTR(sts->req->ued), edd ) != 0;
		exc |= DBS_INT_VAL(sts->req->eii) != DBS_INT_VAL(rom->id); 
		if ( exc /*&& ( chk = Cmp_Dte(std,bst->mng->bdt)>=0 )*/ ){		// Available after today
			chk  = !!( ntf = (GPH_NTF *)calloc( 1, sizeof(GPH_NTF) ) );
			chk &= !!( rdy = (GPH_RDY *)calloc( 1, sizeof(GPH_RDY) ) );
			if ( sts->cut ){
				chk &= rom_day_( NULL, rdy->osd, rdy->oed, elm->elm.rct.sp, elm->elm.rct.ep, bst );
			}
			else{
				*rdy->osd = *DBS_DAY_PTR(sts->req->usd);
				*rdy->oed = *DBS_DAY_PTR(sts->req->ued);
			}
			if ( chk ){
				ntf->ntf = GPN_CHANGE;
				ntf->usr = elm->usr;
				rdy->elm = elm;
				rdy->rom = rom;
				*rdy->nsd = *std;
				*rdy->ned = *edd;
				PostMessage( bst->par, WM_GRAPHNOTIFY, (WPARAM)(LONG_PTR)ntf, (WPARAM)(LONG_PTR)rdy );
			}
			else{
				if ( ntf )	free( ntf );
				if ( rdy )	free( rdy );
			}
		}
	}
	return( chk );
}

static	int		mov_elm_(				// 図形要素の位置移動
Csr			rot,						// 格納先
Fig_elm		*elm,						// 移動対象図形
double		mvx,						// 水平方向移動量
double		mvy,						// 垂直方向移動量
int			dys,						// 最大日数
BST_MNG		*bst )						// Booking status management structure
{
	//Day			std[1], edd[1];
	Dpn			mss[1], mse[1];
	double		dx;

	mss->x = elm->elm.rct.sp->x + mvx;
	mse->x = elm->elm.rct.ep->x + mvx;

	dx = fmod( mss->x, GPH_UNT );
	mss->x = mss->x - fmod( mss->x, GPH_UNT ) + ( dx > 10.0 ? GPH_UNT: 0.0 );
	dx = fmod( mse->x, GPH_UNT );
	mse->x = mse->x - fmod( mse->x, GPH_UNT ) + ( dx > 10.0 ? GPH_UNT: 0.0 );
	mss->y = elm->elm.rct.sp->y + mvy;// - 6.0;
	return( mov_exc_( elm, mss, mse, bst ) );

	/*
	double		dx, sx, ex, sy;//, fmod( double, double ), ceil( double );
	Rct			rct;
	Csr			csr = ((Csr)elm)-1, nnn, pos;
	int			lyr, chk = FALSE;
	Fig_elm		*nel;

	sy = elm->elm.rct.sp->y + mvy;

	sy = ceil( sy / ( GPH_HIT + 1.0 ) );
	lyr = (int)(sy*-1.0) + 1;
	sy = sy * ( GPH_HIT + 1.0 ) - 3.0;

	sx = elm->elm.rct.sp->x + mvx;
	ex = elm->elm.rct.ep->x + mvx;

	dx = fmod( sx, GPH_UNT );
	sx = sx - fmod( sx, GPH_UNT ) + ( dx > 10.0 ? GPH_UNT: 0.0 );
	dx = fmod( ex, GPH_UNT );
	ex = ex - fmod( ex, GPH_UNT ) + ( dx > 10.0 ? GPH_UNT: 0.0 );

	rct = elm->elm.rct;
	rct.sp->x = sx, rct.ep->x = ex;
	rct.sp->y = rct.ep->y = sy - 6.0;

	if ( sx >= 0.0 && ex <= dys*2*GPH_UNT && exs_lin_( rot, lyr ) ){
		if ( ins_chk_( &rct, lyr, &pos, rot, csr ) ){
			if ( pos )	nnn = Ins_Lnk( rot, pos, NULL, sizeof(Fig_elm) );
			else		nnn = Add_Lnk( rot, NULL, sizeof(Fig_elm) );
			if ( nnn ){
				chk = TRUE;
				nel = Rec_Of(Fig_elm, nnn);
				*nel = *elm;
				if ( chk ){
					elm = Rec_Of(Fig_elm, nnn);
					elm->lyr = lyr;
					elm->vis = TRUE;
					elm->elm.rct.sp->x = elm->srn.sp.x = sx;
					elm->elm.rct.ep->x = elm->srn.ep.x = ex;
					elm->elm.rct.sp->y = elm->srn.sp.y = sy - ARW_HIT;
					elm->elm.rct.ep->y = elm->srn.ep.y = sy;
					Fig_Drw_Elm( bst->hnd, elm );
				}
				else{
					Del_Lnk( rot, nnn );
				}
			}
		}
	}
	return( chk ? lyr: -1 );
*/
}

static	int		chg_chk_(				// 時間軸変更可否判定
Fig_elm		*elm,						// 移動対象図形
Fig_elm		**efe,						// 影響を受ける要素
double		mvx,						// 水平方向移動量
int			typ,						// 変形方式
Csr			rot )						// 既存データ
{
	Csr		csr = ((Csr)elm)-1;
	int		chk = TRUE;

	*efe = NULL;
	if ( typ & SID_LFT ){
		while( ( csr = Prv_Lnk( rot, csr ) ) != NULL ){
			*efe = Rec_Of(Fig_elm, csr);
			if ( (*efe)->lyr == elm->lyr )	break;
		}
		if ( csr ){
			if ( elm->elm.rct.sp->x+mvx < (*efe)->elm.rct.sp->x+GPH_UNT ){
				chk = FALSE;
			}
			else{
				if ( (*efe)->elm.rct.ep->x <= elm->elm.rct.sp->x + mvx ){
					*efe = NULL;
				}
			}
		}
		else{
			*efe = NULL;
		}
		if ( elm->elm.rct.ep->x - (elm->elm.rct.sp->x+mvx) < GPH_UNT ){
			chk = FALSE;
		}
	}
	else{
		while( ( csr = Nxt_Lnk( rot, csr ) ) != NULL ){
			*efe = Rec_Of(Fig_elm, csr);
			if ( (*efe)->lyr == elm->lyr )	break;
		}
		if ( csr ){
			if ( elm->elm.rct.ep->x+mvx > (*efe)->elm.rct.ep->x - GPH_UNT ){
				chk = FALSE;
			}
			else{
				if ( (*efe)->elm.rct.sp->x >= elm->elm.rct.ep->x + mvx ){
					*efe = NULL;
				}
			}
		}
		if ( (elm->elm.rct.ep->x+mvx) - elm->elm.rct.sp->x < GPH_UNT ){
			chk = FALSE;
		}
	}
	return( chk );
}

static	void	chg_elm_(				// 図形要素の変形
Fig_elm		*elm,						// 移動対象図形
double		mvx,						// 水平方向移動量
double		mvy,						// 垂直方向移動量
int			typ,						// 変形方式
BST_MNG		*bst )						// 
{
	Dpn			mss[1], mse[1];
	Rct			*rct;
	double		dx, fmod( double, double );
//	Fig_elm		*efe;

	dx = fmod( mvx, GPH_UNT );
	if ( mvx > 0.0 ){
		mvx = mvx - fmod( mvx, GPH_UNT ) + ( dx > 10.0 ? GPH_UNT: 0.0 );
	}
	else{
		mvx = mvx - fmod( mvx, GPH_UNT ) - ( dx < -10.0 ? GPH_UNT: 0.0 );
	}
	rct = &elm->elm.rct;
	*mss = *rct->sp;
	*mse = *rct->ep;
	if ( typ&SID_LFT ){
		mss->x += mvx;
		mov_exc_( elm, mss, mse, bst );
	}
	if ( typ&SID_RIT ){
		mse->x += mvx;
		mov_exc_( elm, mss, mse, bst );
	}
	srd_box_( elm );
}

static	int		cut_elm_(				// 要素切断
Csr		rot,							// 追加データ根
Fig_elm	*elm,							// 切断対象図形
Dpn		*mss,							// マウス操作開始座標
Dpn		*mse,							// マウス操作完了座標
int		typ,							// 切断種別
BST_MNG	*bst )							// Booking status graph management structure
{
	Fig_elm		nnn[1];
	Rct			*nwr, *orr=&elm->elm.rct;
	Csr			csr=NULL;
	int			chk, exc=FALSE;
	double		sx=mss->x;//dx, , fmod( double, double ), fabs(double);
	WCHAR		*msg=NULL;
	BST_ROM_STS	*sts = (BST_ROM_STS *)elm->usr;

	if ( chk = Cmp_Dte( DBS_DAY_PTR(sts->req->ued), bst->mng->bdt ) > 0 ){
		if ( chk = !sts->cut ){
			sx = orr->sp->x + floor( ( sx - orr->sp->x ) / ( GPH_UNT * 2 ) ) * ( GPH_UNT * 2 );
			if ( sx >= orr->sp->x+GPH_UNT*2 ){
				*nnn = *elm;
				nwr = &nnn->elm.rct;
				nwr->sp->x = sx;
				srd_box_( nnn );
				nnn->vis = TRUE;
				sts->cut++;
				csr = Nxt_Lnk( rot, ((Csr)(elm))-1 );
				if ( csr )	csr = Ins_Lnk( rot, csr, (char *)nnn, sizeof(Fig_elm) );
				else		csr = Add_Lnk( rot, (char *)nnn, sizeof(Fig_elm) );
				if ( csr ){
					orr->ep->x = sx;
					srd_box_( elm );
					Fig_Drw_Elm( bst->hnd, nnn );
				}
			}
		}
		else{
			msg = _T("This reservation already cutted.\nPlease execute room changing at first.");
		}
	}
	else{
		msg = _T("Can not modify past reservation.");
	}
	if ( chk ){
		((CMainFrame *)(AfxGetApp()->GetMainWnd()))->SetBookingStsButton( BSB_CRT );
	}
	else{
		::MessageBox( bst->par, msg, NULL, MB_ICONEXCLAMATION|MB_OK );
	}
	return( !!csr );
}

static	int		mus_rdn_(			// Right button down event 
int		msx,						// Mouse device coodinate
int		msy,						// Mouse device coodinate
Dpn		*sp,						// Start point
BST_MNG	*bst )						// Booking status graph management structure
{
	Dpn			*mus;
	GPH_NTF		*ntf;
	int			chk=TRUE;
	POINT		pnt[1] = { msx, msy };
	BST_ROM_STS	*sts;

	mus = (Dpn *)malloc( sizeof(Dpn) );
	ntf = (GPH_NTF * )calloc( 1, sizeof(GPH_NTF) );
	if ( chk = ( !!mus && !!ntf ) ){
		ntf->ntf = GPN_RBUTTONDOWN;
		if ( ClientToScreen( bst->fig->wnd, pnt ) ){
			ntf->msx = pnt->x;
			ntf->msy = pnt->y;
		}
		*mus = *sp;
		if ( bst->fig->rht && bst->fig->rht->usr ){
			sts = (BST_ROM_STS *)bst->fig->rht->usr;
			ntf->rii = DBS_INT_VAL(sts->req->rii);
			ntf->rdi = DBS_INT_VAL(sts->req->rdi);
			ntf->usr = sts;
		}
		else{
			ntf->rii = 0;
			ntf->rdi = 0;
			ntf->usr = NULL;
		}
		PostMessage( bst->par, WM_GRAPHNOTIFY, (WPARAM)(LONG_PTR)ntf, (LPARAM)(LONG_PTR)mus );
	}
	else{
		if ( mus )	free( mus );
		if ( ntf )	free( ntf );
	}

	return( chk );
}

static	int		ope_cbk_(				// Call back function on figure operation
int		evt,							// Content of event
int		typ,							// 図形要素種別／操作種別
int		dtl,							// 操作詳細
Dpn		*mss,							// マウス操作開始座標
Dpn		*mse,							// マウス操作完了座標
void	*ptr,							// 図形要素ポインタ
LPVOID	usr )							// ユーザー定義データ
{
	Fig_elm			*elm = (Fig_elm *)ptr;
	BST_MNG			*bst = (BST_MNG *)usr;
	double			dfx, dfy;
	Csr				rot, csr=NULL;
	int				chk=FALSE;

	switch( evt ){
	case EVT_CRE:	crt_elm_( bst->rot, mss, mse, typ, bst );
					break;
	case EVT_HIT:	sel_clr_( bst->rot, bst );
					if ( elm ){
						elm->sel = TRUE;
						Fig_Drw_Elm( bst->hnd, elm );
					}
					break;
	case EVT_CTE:	elm->vis = FALSE;
					//elm->bcl = COL_AQA;
					Fig_Drw_Elm( bst->hnd, elm );
					if (cut_elm_( bst->rot,elm,mss,mse,dtl,bst)) elm->sel=FALSE;
					elm->vis = TRUE;
					Fig_Drw_Elm( bst->hnd, elm );
					break;
	//case EVT_CGE:	elm->vis = FALSE;
	//				//elm->bcl = COL_AQA;
	//				Fig_Drw_Elm( bst->hnd, elm );
	//				dfx = mse->x - mss->x;
	//				dfy = mse->y - mss->y;
	//				if ( evt == EVT_CGE ){
	//					chg_elm_( elm, dfx, dfy, dtl, bst );
	//				}
	//				if ( elm ){
	//					elm->vis = TRUE;
	//					Fig_Drw_Elm( bst->hnd, elm );
	//				}
	//				break;
	case EVT_CGE:	dfx = mse->x - mss->x;
					dfy = mse->y - mss->y;
					chg_elm_( elm, dfx, dfy, dtl, bst );
					break;
	case EVT_MVE:	dfx = mse->x - mss->x;
					dfy = mse->y - mss->y;
					mov_elm_( bst->rot, elm, dfx, dfy, bst->dys+1, bst );
					break;
	case EVT_SLE:	sel_clr_( bst->rot, bst );
					rot = (Csr)ptr;
					while( ( csr = Nxt_Lnk( rot, csr ) ) != NULL ){
						elm = *(Rec_Of(Fig_elm *, csr));
						elm->sel = TRUE;
						Fig_Drw_Elm( bst->hnd, elm );
					}
					Del_All( rot );
					free( rot );
					break;
	case EVT_CGS:
	case EVT_CTS:
	case EVT_MVS:	chk = !is_sep_(elm->lyr);
					break;
	case EVT_CRS:	chk = TRUE;
					break;
	case EVT_DRG:	chk = drg_pos_( mss, mse, usr );
					break;
	case EVT_RDN:	chk = mus_rdn_( typ, dtl, mss, bst );
					break;
	}

	return( chk );
}

static	void	ent_mrk_(				// ユーザー定義端点マーク登録
BST_MNG	*bst )							// Booking status graph management structure
{
	int		*not[] = { &bst->aln, &bst->arn, &bst->rln, &bst->rrn };
	Dpn		*dpt[] = {	Alw_lft,  Alw_rit,  Rct_lft,  Rct_rit };
	int		cnt[]  = {	memcnt(Alw_lft), memcnt(Alw_rit),
						memcnt(Rct_lft), memcnt(Rct_rit) };
	int		i, col[] = { COL_BLU, COL_BLU, COL_YLW, COL_YLW };
	Trm_mrk	*mrk;

	for ( i=0; i<4; i++ ){
		if ( ( mrk = (Trm_mrk *)calloc( 1, tmk_siz(cnt[i]) ) ) != NULL ){
			mrk->bcl =  col[i];
			mrk->pct =  cnt[i];
			memcpy( mrk->pnt, dpt[i], sizeof(Dpn) * cnt[i] );
			*not[i] = Fig_Ent_Tmk( bst->hnd, mrk );
			free( mrk );
		}
	}
}

static	HWND	ini_gag(				// Initialize the date gage
HINSTANCE	ins,						// Application instance handle
HANDLE		hnd,						// Parent window handle
Area		*dvc,						// Device coodinate
Area		*sys,						// System coodinate
Csr			rot,						// Root of fiure data
LPVOID		usr )						// User definition data
{
	Area		vpt;
	HWND		wnd;

//	Gag_rot = rot;

	if ( dvc->wid > 1.0 && dvc->hit > 1.0 ){
		vpt.top = 0;
		vpt.lft = 0;
		vpt.wid = dvc->wid;
		vpt.hit = dvc->hit;
		wnd = Fig_Ini( ins, (HWND)hnd, dvc, &vpt, sys, rot, usr );
		Fig_Set_Cbk( wnd, NULL, NULL, gag_cbk_, usr );
	}
	return( wnd );
}

static	void	mak_gag(				// ゲージデータ作成
Csr		rot,							// Destination pointer which store data 
int		yps,							// 表示Ｙ座標
Day		*std,							// Booking status management segment
int		dys,							// days of period
HWND	wnd )							// Window handle of gage window
{
	Fig_elm		*elm;
	GAG_DAT		*gag;
	Csr			csr;
	Day			dte[1];
	int			i, siz = sizeof(Fig_elm) + sizeof(GAG_DAT);

	*dte = *std;
	if ( ( csr = Add_Lnk( rot, NULL, sizeof(Fig_elm) ) ) != NULL ){
		elm = Rec_Of(Fig_elm, csr);
		set_edf_( elm, LYR_HGG, DGG_BCL, ELM_RCT );
		elm->elm.rct.sp->x = -1;
		elm->elm.rct.sp->y = 0;
		elm->elm.rct.ep->x = (dys+1)*2*GPH_UNT;
		elm->elm.rct.ep->y = yps;

		if ( ( csr = Add_Lnk( rot, NULL, sizeof(Fig_elm) ) ) != NULL ){
			elm = Rec_Of(Fig_elm, csr);
			set_edf_( elm, LYR_HGG, COL_RED, ELM_LIN );
			elm->elm.lin.sp->x = 0;
			elm->elm.lin.sp->y = 5;
			elm->elm.lin.ep->x = (dys+1)*2*GPH_UNT;
			elm->elm.lin.ep->y = 5;

			for ( i=0; i<(dys+1)*2; i++ ){
				if ( ( csr = Add_Lnk( rot, NULL, siz ) ) != NULL ){
					elm = Rec_Of(Fig_elm, csr);
					set_edf_( elm, LYR_VGG, COL_RED, ELM_LIN );
					elm->elm.lin.sp->x = i * GPH_UNT;
					elm->elm.lin.sp->y = ( i % 2 ) ? 8: 15;
					elm->elm.lin.ep->x = i * GPH_UNT;
					elm->elm.lin.ep->y = 5;
					elm->srn.sp.x = i * GPH_UNT - GPH_UNT;
					elm->srn.sp.y = 0;
					elm->srn.ep.x = elm->srn.sp.x + GPH_UNT * 2;
					elm->srn.ep.y = 20;
					gag = (GAG_DAT *)(elm+1);
					gag->mth = (short)(( i % 2 ) ? dte->mth: -1);
					gag->day = dte->day;
					if ( ( i % 2 ) )	Add_Dte( dte, 1 );
				}
				else{
					break;
				}
			}
		}
	}
	if ( csr )	Fig_Rdw( wnd );
	else		Del_All( rot );
}

static	HWND	ini_gph(				// Initialize the booking status graphic
HINSTANCE	ins,						// Application instance handle
HWND		hnd,						// Parent window handle
Area		*dvc,						// Device coodinate
Area		*sys,						// System coodinate
Csr			rot,						// Root of figure data
int			dys,						// Day number of displaying period
BST_MNG		*bst )						// User data
{
	Area		vpt;

	if ( dvc->wid > 1.0 && dvc->hit > 1.0 ){
		vpt.top = 0;
		vpt.lft = 0;
		vpt.wid = dvc->wid;
		vpt.hit = dvc->hit;
		bst->hnd = Fig_Ini( ins, hnd, dvc, &vpt, sys, rot, bst );
		bst->fig = Fig_Set_Cbk( bst->hnd, NULL, ope_cbk_, drw_cbk_, bst );
		ent_mrk_( bst );
	}
	return( bst->hnd );
}

static	void	del_bst(			// 部屋別予約状況データ削除
Csr			rot )					// 削除対象
{
	Csr			csr;
	Fig_elm		*elm;

	csr = Nxt_Lnk( rot, NULL );
	while( csr ){
		elm = Rec_Of(Fig_elm, csr);
		if ( !is_sep_(elm->lyr) ){
			csr = Del_Lnk( rot, csr );
		}
		else{
			csr = Nxt_Lnk( rot, csr );
		}
	}
}

static	Csr		add_bst(	// Addition the booking data
Csr			rot,			// Destination root
BST_ROM_STS	*sts,			// Reservation status data
Day			*std,			// Start day of display period
int			lin,			// Line(Room) index
LPVOID		usr,			// User definition data
BST_MNG		*bst )			// Booking status management structure
{
	Csr			csr;
	Fig_elm		*elm;
	double		sx, sy, ex;
	int			dus, siz, eci, lco;
	Day			*usd, *ued, *arv, *dpt;
	WCHAR		*fnm, *lnm;

	arv = DBS_DAY_PTR(sts->arv);
	dpt = DBS_DAY_PTR(sts->dpt);
	usd = DBS_DAY_PTR(sts->req->usd);
	ued = DBS_DAY_PTR(sts->req->ued);
	fnm = Nul_Fld(sts->fnm, FLD_WST) ? _T(""): DBS_WST_PTR(sts->fnm);
	lnm = Nul_Fld(sts->lnm, FLD_WST) ? _T(""): DBS_WST_PTR(sts->lnm);
	eci = DBS_INT_VAL(sts->eci);
	lco = DBS_INT_VAL(sts->lco);

	dus = Clc_Dys(usd, ued)==0;
	siz = (int)wcslen( fnm ) + (int)wcslen( lnm ) + wsizeof( 5 );
	if ( csr = Add_Lnk( rot, NULL, sizeof(Fig_elm) ) ){
		elm = Rec_Of(Fig_elm, csr);
		elm->usr = usr;
		if ( dus ){				// day use pattern
			sx = day_gph_(std,usd);
			ex = day_gph_(std,ued) + GPH_UNT*2;
		}
		else{
			sx = day_gph_(std,usd) + GPH_UNT;
			ex = day_gph_(std,ued) + GPH_UNT;
		}
		sy = (double)-lin * ( GPH_HIT + 1.0 ) - 3.0;
		set_elm_( elm, lin+1, sx, sy, ex, dus ? COL_DUS: COL_STY, bst );
		if ( Cmp_Dte(arv, usd )==0 )		elm->elm.rct.lmc = eci ? COL_ELO: elm->bcl;
		else								elm->elm.rct.lmc = COL_YLW;
		if ( Cmp_Dte(dpt, ued )==0 )		elm->elm.rct.rmc = lco ? COL_ELO: elm->bcl;
		else								elm->elm.rct.rmc = COL_YLW;
		elm->pcl = elm->bcl;
		elm->sel = FALSE;
	}
	return( csr );
}

static	int		bst_gph(			// Converting from booking status to graphic
BST_MNG		*bst )					// Booking status management segment
{
	Csr			csr=NULL, cse=NULL, rot=bst->rom;
	BST_ROM_LST	*dtl;
	BST_ROM_STS	*sts;
	int			chk=TRUE, lin;

	for ( lin=0; chk && !!( csr = Nxt_Lnk( rot, csr ) ); lin++ ){
		dtl = Rec_Of(BST_ROM_LST, csr);
		while ( cse = Nxt_Lnk( dtl->bst, cse ) ){
			sts = Rec_Of(BST_ROM_STS, cse);
			chk = !!( sts->fig = add_bst( bst->rot, sts, bst->std, lin, sts, bst ) );
//break;
		}
//break;
	}
	return( chk );
}

static	int		add_gpl(			// グラフ１行追加処理
int			lno,					// 行番号
BST_MNG		*bst )					// Booking status graph management structure
{
	Fig_elm		*elm;
	Csr			csr;
	double		sy = -( GPH_HIT + 1.0 ) * (double)lno;
	int			chk;

	if ( chk = !!( csr = Add_Lnk( bst->rot, NULL, sizeof(Fig_elm) ) ) != NULL ){
		elm = Rec_Of(Fig_elm, csr);
		elm->lyr = lno | LIN_SEP;
		elm->pcl = COL_BLK,	elm->bcl = COL_BLK;
		elm->vis = TRUE;	elm->sel = FALSE;
		elm->typ = ELM_LIN;
		elm->elm.lin.sp->x = 0;
		elm->elm.lin.ep->x = GPH_WID(bst->dys+1);
		elm->elm.lin.sp->y = elm->elm.lin.ep->y = sy;
		Fig_Rdw( bst->hnd );
	}
	return( chk );
}

int		Mak_Gph(				// 子ウィンドウ生成処理
BST_MNG		*mng,				// データ管理構造
HINSTANCE	ins,				// アプリケーションインスタンス
HWND		par,				// 親ウィンドウハンドル
int			cnt )				// 既存データ件数
{
	int			chk = false, cx, cy, i;
	double		wid;
	RECT		rct[1];

	::GetClientRect( par, rct );

	cx = rct->right - rct->left + 1;
//	cy = rct->bottom - rct->top + 1;
	cy = GAG_HGT + ( GPH_HIT + 1 ) * cnt + 1;

	mng->dys = Clc_Dys( mng->std, mng->edd );

	mng->gdv->lft = (mng->rnw+mng->rtw) - 1;
	mng->gdv->top = 0;
	mng->gdv->wid = ( wid = cx - (mng->rnw+mng->rtw) ) > (mng->dys+1)*2*GPH_UNT ? wid: (mng->dys+1)*2*GPH_UNT;
	mng->gdv->wid = cx - (mng->rnw+mng->rtw);
	mng->gdv->hit = GAG_HGT;

	mng->dvc->lft = (mng->rnw+mng->rtw) - 1;
	mng->dvc->top = mng->gdv->hit - 1;
	mng->dvc->wid = ( wid = cx - (mng->rnw+mng->rtw) ) > (mng->dys+1)*2*GPH_UNT ? wid: (mng->dys+1)*2*GPH_UNT;
	mng->dvc->hit = cy - GAG_HGT + 1;

	*(mng->gsy) = *(mng->gdv);
	*(mng->sys) = *(mng->dvc);
	mng->gsy->lft = 0;

	mng->sys->lft = 0;
	mng->sys->top = 0;
	mng->sys->hit *= -1.0;

	mng->ghd = ini_gag( ins, par, mng->gdv, mng->gsy, mng->grt, mng );
	mng->hnd = ini_gph( ins, par, mng->dvc, mng->sys, mng->rot, mng->dys, mng );

	if ( mng->ghd && mng->hnd ){
		mak_gag( mng->grt, rct->bottom - rct->top + 1, mng->std, mng->dys, mng->ghd );
		Chg_Mod( OPE_CRT|OPE_CHG, mng );
		for ( i=0; i<cnt; i++ )	add_gpl( i+1, mng );
		mng->coe = 1.0;
		chk = true;
	}
	else{
		if ( mng->ghd )	Fig_Trm( mng->ghd );
		if ( mng->hnd )	Fig_Trm( mng->hnd );
	}
	if ( chk ){
		del_bst( mng->rot );
		chk = bst_gph( mng );
	}
	return( chk );
}

int			Rmk_Gph(				// remake graphic data
RECT		*rct,
BST_MNG		*mng )					// データ管理構造
{
	int			i, cnt = Cnt_Lnk( mng->rom );

	Dst_Gph( mng->grt );
	mak_gag( mng->grt, rct->bottom - rct->top + 1, mng->std, mng->dys, mng->ghd );
	del_bst( mng->rot );
	for ( i=0; i<cnt; i++ )	add_gpl( i+1, mng );
	return( bst_gph( mng ) );
}

int			Del_Gph(				// Delete graph from list
int			rdi,					// reservation_detail_id
int			rei,					// reservation_equipment_id of delete data
BST_MNG		*bst )					// Booking status management segment
{
	Csr			csr=NULL;
	BST_ROM_STS	*sts;
	Fig_elm		*elm;
	int			chk=FALSE;

	csr = Nxt_Lnk( bst->rot, csr );
	while ( csr ){
		elm = Rec_Of(Fig_elm, csr);
		if ( !!( sts = (BST_ROM_STS *)elm->usr ) && ( rdi==DBS_INT_VAL(sts->req->rdi) || rei==DBS_INT_VAL(sts->req->id) ) ){
			Fig_Drw_Elm( bst->hnd, elm );
			csr = Del_Lnk( bst->rot, csr );
			chk = TRUE;
			if ( !!rei )	break;
		}
		else{
			csr = Nxt_Lnk( bst->rot, csr );
		}
	}
	return( chk );
}

int			Upd_Gph(				// Change the graphic element
BST_ROM_STS	*udt,					// update data
BST_MNG		*bst )					// Booking status management segment
{
	Csr			csr=NULL;
	BST_ROM_LST	*dtl;
	int			chk=TRUE, lin;

	// delete old data
	Del_Gph( 0, DBS_INT_VAL(udt->req->id), bst );

	for ( lin=0, csr=NULL; chk && !!( csr = Nxt_Lnk( bst->rom, csr ) ); lin++ ){
		dtl = Rec_Of(BST_ROM_LST, csr);
		if ( DBS_INT_VAL(dtl->id) == DBS_INT_VAL(udt->req->eii) ){
			chk = !!( udt->fig = add_bst( bst->rot, udt, bst->std, lin, udt, bst ) );
			Fig_Drw_Elm( bst->hnd, Rec_Of(Fig_elm, udt->fig) );
			break;
		}
	}
	return( chk );
}

void		Chg_Mod(					// 図形作成モード切替え
int			ope,						// 切替えるモード
BST_MNG		*bst )						// Booking status graph management structure
{
	Fig_Set_Ope( bst->hnd, ope, RBR_LIN );
}

static	void		ers_elm(			// 全選択要素削除
Csr			rot,						// Destination root
int			*tbl,						// 削除マークテーブル
int			max,						// テーブル最大数
BST_MNG		*bst )						// Booking status graph management structure
{
	Fig_elm		*elm;
	Csr			csr;

	if ( ( csr = Nxt_Lnk( bst->rot, NULL ) ) != NULL ){
		do{
			elm = Rec_Of(Fig_elm, csr);
			if ( elm->sel ){
				if ( lin_of_(elm->lyr) < max ){
					if (tbl)	*(tbl+lin_of_(elm->lyr)-1) = TRUE;
				}
				elm->vis = FALSE;
				Fig_Drw_Elm( bst->hnd, elm );
				csr = Del_Lnk( bst->rot, csr );
			}
			else{
				csr = Nxt_Lnk( bst->rot, csr );
			}
		} while( csr );
	}
}

void	Ers_Bok(				// Cancellation booking
BST_MNG		*bst )				// 格納領域
{
	Csr			csr=NULL;
	Fig_elm		*elm;
	BST_ROM_STS	*sts;
	WCHAR		buf[2014], bfb[20], btb[20];
	while ( csr = Nxt_Lnk( bst->rot, csr ) ){
		elm = Rec_Of(Fig_elm, csr);
		if ( elm->sel ){
			sts = (BST_ROM_STS *)elm->usr;
			swprintf_s( buf, memcnt(buf),
				_T("Do you want to cancel following reservation?\n")
				_T("Rsv.ID	: %d(E.ID=%s)\n")
				_T("Name	: %s %s\n")
				_T("Stay	: %s-%s %s(%s)"),
				DBS_INT_VAL(sts->rvn), EDT_DBF_WST(sts->ern,_T("None")),
				EDT_DBF_WST(sts->fnm,_T("")),EDT_DBF_WST(sts->lnm,_T("")),
				Edt_Dte(bfb, memcnt(bfb), DBS_DAY_PTR(sts->arv) ),
				Edt_Dte(btb, memcnt(btb), DBS_DAY_PTR(sts->dpt) ),
				DBS_WST_PTR(sts->rno),DBS_WST_PTR(sts->dtl->tpn) );
			if ( MessageBox( bst->par, buf, NULL, MB_ICONQUESTION|MB_YESNO )==IDYES ){
			}
		}
	}
}

void	Siz_Gph(				// グラフウィンドウサイズ変更処理
BST_MNG		*mng,				// 格納領域
int			cx,					// 幅
int			cy,					// 高
int			cnt,				// 行数
int			rdw )				// 再描画有無
{
	double			wid;
	Area			vpt[1];

	cy = GAG_HGT + ( GPH_HIT + 1 ) * cnt + 1;

	mng->gdv->wid = mng->gsy->wid = ( wid = cx - (mng->rnw+mng->rtw) ) > (mng->dys+1)*2*GPH_UNT ? (mng->dys+1)*2*GPH_UNT: wid;
	mng->gdv->hit = mng->gsy->hit = GAG_HGT;
	mng->dvc->wid = mng->sys->wid = ( wid = cx - (mng->rnw+mng->rtw) ) > (mng->dys+1)*2*GPH_UNT ? (mng->dys+1)*2*GPH_UNT: wid;
	mng->dvc->hit = cy - GAG_HGT + 1;
	mng->sys->hit = mng->dvc->hit * -1.0;

	//if ( mng->gsy->lft + mng->gsy->wid > GPH_WID(mng->dys+1) ){
	//	mng->gsy->lft = mng->sys->lft = ( wid = GPH_WID(mng->dys+1) - mng->gsy->wid ) > 0 ? wid: 0;
	//}
	if ( rdw ){
		vpt->top = 0;	vpt->lft = 0;
		vpt->wid = mng->gdv->wid;
		vpt->hit = mng->gdv->hit;
		Fig_Set_Dwn( mng->ghd, mng->gsy, mng->gdv, vpt, TRUE, TRUE );
		vpt->wid = mng->dvc->wid;
		vpt->hit = mng->dvc->hit;
		Fig_Set_Dwn( mng->hnd, mng->sys, mng->dvc, vpt, TRUE, TRUE );
	}
}

void	Drw_Rif(					// 部屋情報描画
CDC			*pDC,					// デバイスコンテキスト
CWnd		*pWnd,					// ウィンドウハンドル
BST_MNG		*gph )					// 勤怠管理情報
{
	Csr			csr=NULL;
	RECT		rct[1], ttl[1], rif[1], rno[1], rtp[1], txt[1];
	CBrush		cGryBrs, cBlkBrs, cWhtBrs;
	HFONT		oft;
	int			omd, top, rmn, rmi=0;
	BST_ROM_LST	*dtl;

	rmn = top = pWnd->GetScrollPos( SB_VERT );
	::GetClientRect( pWnd->m_hWnd, rct );
	omd = pDC->SetBkMode( TRANSPARENT );
	oft = (HFONT)::SelectObject( pDC->m_hDC, gph->fnt );
	cBlkBrs.CreateSolidBrush( COL_BLK );

	*ttl = *rct;
	ttl->right = ttl->left + (gph->rnw+gph->rtw) + 1;
	ttl->bottom = ttl->top + GAG_HGT;
	cGryBrs.CreateSolidBrush( DGG_BCL );
	pDC->FillRect( ttl, &cGryBrs );
	ttl->right = ttl->left + gph->rnw + 1;
	pDC->FrameRect( ttl, &cBlkBrs );
	pDC->DrawText( _T("Room Number"), ttl, DT_SINGLELINE|DT_CENTER|DT_VCENTER );
	ttl->left = gph->rnw;
	ttl->right = ttl->left + gph->rtw + 1;
	pDC->FrameRect( ttl, &cBlkBrs );
	pDC->DrawText( _T("Room Type"), ttl, DT_SINGLELINE|DT_CENTER|DT_VCENTER );
	cGryBrs.DeleteObject();

	rif->left = rno->left = 0;
	rno->right = gph->rnw + 1;
	rtp->left = gph->rnw;
	rtp->right = rif->right = (gph->rnw+gph->rtw);
	rif->top = rno->top = rtp->top = GAG_HGT - 1;
	rif->bottom = rno->bottom = rtp->bottom = rno->top + GPH_HIT + 2;

	cWhtBrs.CreateSolidBrush( COL_WHT );

	while ( csr = Nxt_Lnk( gph->rom, csr ) ){
		if ( !top ){
			dtl = Rec_Of(BST_ROM_LST, csr);
			pDC->FillRect( rif, &cWhtBrs );
			pDC->FrameRect( rno, &cBlkBrs );
			*txt = *rno;
			txt->left += 2;	txt->right -= 4;
			pDC->DrawText( (WCHAR *)Fld_Ptr(dtl->cod, FLD_WST), txt, DT_SINGLELINE|DT_LEFT|DT_VCENTER );
			pDC->FrameRect( rtp, &cBlkBrs );
			*txt = *rtp;
			txt->left += 2;	txt->right -= 4;
			pDC->DrawText( (WCHAR *)Fld_Ptr(dtl->tpn, FLD_WST), txt, DT_SINGLELINE|DT_LEFT|DT_VCENTER );
			rif->top += GPH_HIT + 1;
			rif->bottom += GPH_HIT + 1;
			rno->top = rtp->top = rif->top;
			rno->bottom = rtp->bottom = rif->bottom;
		}
		else{
			--top;
		}
	}

	while ( rmn-- ){
		pDC->FillRect( rif, &cWhtBrs );
		if ( !rmi++ ){
			pDC->MoveTo( rif->left, rif->top );
			pDC->LineTo( rif->right, rif->top );
		}
//		pDC->FrameRect( rno, &cBlkBrs );
//		pDC->FrameRect( rtp, &cBlkBrs );
		rif->top += GPH_HIT + 1;
		rif->bottom += GPH_HIT + 1;
		rno->top = rtp->top = rif->top;
		rno->bottom = rtp->bottom = rif->bottom;

	}
	cGryBrs.CreateSolidBrush( COL_DGL );
	rct->top = rif->top;
	pDC->FillRect( rct, &cGryBrs );
	cGryBrs.DeleteObject();
	cWhtBrs.DeleteObject();
	cBlkBrs.DeleteObject();
	::SelectObject( pDC->m_hDC, oft );
	omd = pDC->SetBkMode( omd );
}

void	Dst_Gph(
Csr		rot )				// データ管理構造
{
	Del_All( rot );
	/*
	Csr			csr=Nxt_Lnk(rot,NULL);
	Fig_elm		*elm;

	while( csr ){
		elm = Rec_Of(Fig_elm, csr);
		csr = Del_Lnk(rot, csr);
	}
	*/
}
