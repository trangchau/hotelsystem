/*
	行列演算ユーティリティ
	Copyright(C) Arsnova Corp. 2003
*/

#define AFN_CNV_EXP_	1
#include <math.h>
#include "afn_cnv.h"

static void fld_chk_(				/*  縮尺率正規化  */
	Fld_def			*src,			/*  参照側        */
	Fld_def			*dst)			/*  出力側        */
{
	double         dx, dy, tmp, drt, srt;
	double         a2 = 2.0;

	drt = (     dst->ey - dst->sy) / (     dst->ex - dst->sx);
	srt = (dy = src->ey - src->sy) / (dx = src->ex - src->sx);

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

void Set_Mtx(						/*  作用素算定  */
	Mtx_def			*mat,			/*  変換行列    */
	Fld_def			*dst,			/*  出力側      */
	Fld_def			*src,			/*  入力側      */
	int				dvc)			/*  異種座標    */
{
	double         dx, dy, sx, sy;

	mat->xa = mat->ya = mat->xc = 0.0;
	mat->yb = mat->xb = mat->yc = 0.0;
	dx = dst->ex - dst->sx, dy = dst->ey - dst->sy;
	sx = src->ex - src->sx, sy = src->ey - src->sy;
	if (dvc==2){/* Ｘ軸反転なら */
		mat->xa = -dx / sx;
		mat->xc = dst->ex - (src->sx * mat->xa);
	} else{
		mat->xa =  dx / sx;
		mat->xc = dst->sx - (src->sx * mat->xa);
	}

	if (dvc==1){/* Ｙ軸反転なら */
		mat->yb = -dy / sy;
		mat->yc = dst->ey - (src->sy * mat->yb);
	} else{
		mat->yb =  dy / sy;
		mat->yc = dst->sy - (src->sy * mat->yb);
	}
}

void Get_Mtx(						/*  作用素算定(出力を優先)          */
	Mtx_def			*mat,			/*  変換行列                        */
	Fld_def			*dst,			/*  出力側                          */
	Fld_def			*src,			/*  入力側                          */
	int				dvc)			/*  異種座標(0:無 1:Y反転 2:X反転)  */
{
	Fld_def			sr2;

	sr2.sx = src->sx, sr2.sy = src->sy;
	sr2.ex = src->ex, sr2.ey = src->ey;
	fld_chk_(&sr2, dst);

	Set_Mtx(mat, dst, &sr2, dvc);
}

void Dpn_Cnv(						/*  座標変換  */
	Dpn				*dst,			/*  変換後    */
	Dpn				*src,			/*  変換前    */
	int				n,				/*  座標数    */
	Mtx_def			*mat)			/*  変換行列  */
{
	Dpn				tmp;

	for (; n--; dst++, src++){
		tmp = *src;
		dst->x = tmp.x * mat->xa + tmp.y * mat->xb + mat->xc;
		dst->y = tmp.x * mat->ya + tmp.y * mat->yb + mat->yc;
	}
}

void Trs_Mtx(						/*  変換仕様生成  */
	Mtx_def			*mat,			/*  変換仕様      */
	double			alp,			/*  回転量        */
	double			scl,			/*  縮尺率        */
	Dpn				*op,			/*  変換前の位置  */
	Dpn				*cp)			/*  変換後の位置  */
{
	static Dpn		org;
	double			cc, ss;

	if (!op) op = &org;
	if (!cp) cp = &org;
	cc = cos(alp) * scl, ss = sin(alp) * scl;

	mat->xa =  cc, mat->xb = -ss;
	mat->ya =  ss, mat->yb =  cc;
	mat->xc =  cp->x - (op->x * cc - op->y * ss);
	mat->yc =  cp->y - (op->x * ss + op->y * cc);
}

void Prd_Mtx(						/*  行列の積(P = D × S)  */
	Mtx_def		*prd,				/*  積                    */
	Mtx_def		*dst,				/*  １項                  */
	Mtx_def		*src)				/*  ２項                  */
{
	Mtx_def		tmp;

	tmp.xa = dst->xa * src->xa + dst->xb * src->ya;
	tmp.xb = dst->xa * src->xb + dst->xb * src->yb;
	tmp.xc = dst->xa * src->xc + dst->xb * src->yc + dst->xc;

	tmp.ya = dst->ya * src->xa + dst->yb * src->ya;
	tmp.yb = dst->ya * src->xb + dst->yb * src->yb;
	tmp.yc = dst->ya * src->xc + dst->yb * src->yc + dst->yc;

	prd->xa = tmp.xa, prd->xb = tmp.xb, prd->xc = tmp.xc;
	prd->ya = tmp.ya, prd->yb = tmp.yb, prd->yc = tmp.yc;
}

void Unt_Mtx(						/*  単位行列設定  */
	Mtx_def			*mtx)			/*  設定行列      */
{
	double			one = 1.0, zro = 0.0;

	mtx->xa = mtx->yb                     = one;
	mtx->xb = mtx->ya = mtx->xc = mtx->yc = zro;
}
