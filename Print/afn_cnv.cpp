/*
	�s�񉉎Z���[�e�B���e�B
	Copyright(C) Arsnova Corp. 2003
*/

#define AFN_CNV_EXP_	1
#include <math.h>
#include "afn_cnv.h"

static void fld_chk_(				/*  �k�ڗ����K��  */
	Fld_def			*src,			/*  �Q�Ƒ�        */
	Fld_def			*dst)			/*  �o�͑�        */
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

void Set_Mtx(						/*  ��p�f�Z��  */
	Mtx_def			*mat,			/*  �ϊ��s��    */
	Fld_def			*dst,			/*  �o�͑�      */
	Fld_def			*src,			/*  ���͑�      */
	int				dvc)			/*  �َ���W    */
{
	double         dx, dy, sx, sy;

	mat->xa = mat->ya = mat->xc = 0.0;
	mat->yb = mat->xb = mat->yc = 0.0;
	dx = dst->ex - dst->sx, dy = dst->ey - dst->sy;
	sx = src->ex - src->sx, sy = src->ey - src->sy;
	if (dvc==2){/* �w�����]�Ȃ� */
		mat->xa = -dx / sx;
		mat->xc = dst->ex - (src->sx * mat->xa);
	} else{
		mat->xa =  dx / sx;
		mat->xc = dst->sx - (src->sx * mat->xa);
	}

	if (dvc==1){/* �x�����]�Ȃ� */
		mat->yb = -dy / sy;
		mat->yc = dst->ey - (src->sy * mat->yb);
	} else{
		mat->yb =  dy / sy;
		mat->yc = dst->sy - (src->sy * mat->yb);
	}
}

void Get_Mtx(						/*  ��p�f�Z��(�o�͂�D��)          */
	Mtx_def			*mat,			/*  �ϊ��s��                        */
	Fld_def			*dst,			/*  �o�͑�                          */
	Fld_def			*src,			/*  ���͑�                          */
	int				dvc)			/*  �َ���W(0:�� 1:Y���] 2:X���])  */
{
	Fld_def			sr2;

	sr2.sx = src->sx, sr2.sy = src->sy;
	sr2.ex = src->ex, sr2.ey = src->ey;
	fld_chk_(&sr2, dst);

	Set_Mtx(mat, dst, &sr2, dvc);
}

void Dpn_Cnv(						/*  ���W�ϊ�  */
	Dpn				*dst,			/*  �ϊ���    */
	Dpn				*src,			/*  �ϊ��O    */
	int				n,				/*  ���W��    */
	Mtx_def			*mat)			/*  �ϊ��s��  */
{
	Dpn				tmp;

	for (; n--; dst++, src++){
		tmp = *src;
		dst->x = tmp.x * mat->xa + tmp.y * mat->xb + mat->xc;
		dst->y = tmp.x * mat->ya + tmp.y * mat->yb + mat->yc;
	}
}

void Trs_Mtx(						/*  �ϊ��d�l����  */
	Mtx_def			*mat,			/*  �ϊ��d�l      */
	double			alp,			/*  ��]��        */
	double			scl,			/*  �k�ڗ�        */
	Dpn				*op,			/*  �ϊ��O�̈ʒu  */
	Dpn				*cp)			/*  �ϊ���̈ʒu  */
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

void Prd_Mtx(						/*  �s��̐�(P = D �~ S)  */
	Mtx_def		*prd,				/*  ��                    */
	Mtx_def		*dst,				/*  �P��                  */
	Mtx_def		*src)				/*  �Q��                  */
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

void Unt_Mtx(						/*  �P�ʍs��ݒ�  */
	Mtx_def			*mtx)			/*  �ݒ�s��      */
{
	double			one = 1.0, zro = 0.0;

	mtx->xa = mtx->yb                     = one;
	mtx->xb = mtx->ya = mtx->xc = mtx->yc = zro;
}
