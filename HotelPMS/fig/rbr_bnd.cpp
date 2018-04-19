/*
	�}�`�쐬�E���샋�[�`��
	�}�`�e�i���o�[�o���h�j�����֐�
*/

#include "fig_mnp.h"
#include "fig_ope.h"
#include "rbr_bnd.h"

static	void	rbr_bnd_(			// �S���o���h�`��
HDC		hdc,						// �`��Ώۃf�o�C�X�R���e�L�X�g
int		sx,							// �n�_X���W
int		sy,							// �n�_Y���W
int		ex,							// �I�_X���W
int		ey,							// �I�_Y���W
int		typ )						// �S���o���h���
{
	POINT			ply[5];
	int				rop, tpc=0;
	HPEN			pen, org;

	pen = CreatePen( PS_SOLID, 1, RBR_COL );

	if ( pen ){
		rop = SetROP2( hdc, R2_XORPEN );
		org = (HPEN)SelectObject( hdc, pen );

		switch( typ ){
		case ELM_LIN:	ply[0].x = sx;	ply[0].y = sy;
						ply[1].x = ex;	ply[1].y = ey;
						tpc = 2;
						break;
		case ELM_RCT:	ply[0].x = sx;	ply[0].y = sy;
						ply[1].x = ex;	ply[1].y = sy;
						ply[2].x = ex;	ply[2].y = ey;
						ply[3].x = sx;	ply[3].y = ey;
						ply[4].x = sx;	ply[4].y = sy;
						tpc = 5;
						break;
		}
		Polyline( hdc, ply, tpc );

		SelectObject( hdc, org );
		SetROP2( hdc, rop );
		DeleteObject( pen );
	}
}

static	void	lin_sdw_(			// �����ό`�V���h�[�`��
HDC		hdc,						// �`��Ώۃf�o�C�X�R���e�L�X�g
Fig_mng	*mng )						// �}�`�Ǘ��\��
{
	Dpn		dvc[1];
	Lin		*lin = &mng->hit->elm.lin;
	int		sx, sy, ex, ey;

	if ( mng->dir&SID_EP ){
		Dpn_Cnv( dvc, lin->sp, 1, mng->s2d );
		sx = (int)dvc->x;	sy = (int)dvc->y;
		ex = (int)(mng->mse.x + mng->dif.ep.x);
		ey = (int)(mng->mse.y + mng->dif.ep.y);
	}
	else{
		Dpn_Cnv( dvc, lin->ep, 1, mng->s2d );
		sx = (int)dvc->x;	sy = (int)dvc->y;
		ex = (int)(mng->mse.x + mng->dif.sp.x);
		ey = (int)(mng->mse.y + mng->dif.sp.y);
	}
	rbr_bnd_( hdc, sx, sy, ex, ey, ELM_LIN );
}

static	void	rct_sdw_(			// �����ό`�V���h�[�`��
HDC		hdc,						// �`��Ώۃf�o�C�X�R���e�L�X�g
Fig_mng	*mng )						// �}�`�Ǘ��\��
{
	Dpn		dvc[2], wld[2];
	int		sx, sy, ex, ey;
	Rct		*rct = &mng->hit->elm.rct;

	if ( mng->dtl == CHG_TOP ){
		if ( mng->dir&SID_UPR ){
			if ( mng->dir&SID_RIT ){
				Dpn_Cnv( dvc, rct->sp, 1, mng->s2d );
				sx = (int)dvc->x;	sy = (int)dvc->y;
				ex = (int)(mng->mse.x + mng->dif.ep.x);
				ey = (int)(mng->mse.y + mng->dif.ep.y);
			}
			else{
				wld->x = rct->ep->x;	wld->y = rct->sp->y;
				Dpn_Cnv( dvc, wld, 1, mng->s2d );
				sx = (int)dvc->x;	sy = (int)dvc->y;
				ex = (int)(mng->mse.x + mng->dif.sp.x);
				ey = (int)(mng->mse.y + mng->dif.ep.y);
			}
		}
		else{
			if ( mng->dir&SID_LFT ){
				Dpn_Cnv( dvc, rct->ep, 1, mng->s2d );
				sx = (int)dvc->x;	sy = (int)dvc->y;
				ex = (int)(mng->mse.x + mng->dif.sp.x);
				ey = (int)(mng->mse.y + mng->dif.sp.y);
			}
			else{
				wld->x = rct->sp->x;	wld->y = rct->ep->y;
				Dpn_Cnv( dvc, wld, 1, mng->s2d );
				sx = (int)dvc->x;	sy = (int)dvc->y;
				ex = (int)(mng->mse.x + mng->dif.ep.x);
				ey = (int)(mng->mse.y + mng->dif.sp.y);
			}
		}
	}
	else{
		Dpn_Cnv( dvc,   rct->sp, 1, mng->s2d );
		Dpn_Cnv( dvc+1, rct->ep, 1, mng->s2d );
		if ( mng->dir&SID_UPR ){
			sx = (int)dvc->x;		sy = (int)dvc->y;
			ex = (int)(dvc+1)->x;	ey = (int)(mng->mse.y + mng->dif.ep.y);
		}
		if ( mng->dir&SID_LWR ){
			sx = (int)dvc->x;		sy = (int)(mng->mse.y + mng->dif.sp.y);
			ex = (int)(dvc+1)->x;	ey = (int)(dvc+1)->y;
		}
		if ( mng->dir&SID_LFT ){
			sx = (int)(mng->mse.x + mng->dif.sp.x);	sy = (int)(dvc+1)->y;
			ex = (int)(dvc+1)->x;	ey = (int)dvc->y;
		}
		if ( mng->dir&SID_RIT ){
			sx = (int)dvc->x;			sy = (int)(dvc+1)->y;
			ex = (int)(mng->mse.x + mng->dif.ep.x);	ey = (int)dvc->y;
		}
	}
	rbr_bnd_( hdc, sx, sy, ex, ey, ELM_RCT );
}

static	void	cal_cut_(			// �ؒf�p�����v�Z
int		*sx,						// �n�_�w�i�[�̈�
int		*sy,						// �n�_�x�i�[�̈�
int		*ex,						// �I�_�w�i�[�̈�
int		*ey,						// �I�_�x�i�[�̈�
Fig_mng	*mng )						// �}�`�Ǘ��\��
{
	double		atan2( double, double );
	double		dx, dy;
	Dpn			dvc[2], wld[2];
	Rct			*rct = &mng->hit->elm.rct;

	if ( mng->hit ){
		dx = (double)( max_(mng->mss.x,mng->mse.x) - min_(mng->mss.x,mng->mse.x) );
		dy = (double)( max_(mng->mss.y,mng->mse.y) - min_(mng->mss.y,mng->mse.y) );
		wld[0] = *rct->sp;	wld[1] = *rct->ep;
		Dpn_Cnv( dvc, wld, 2, mng->s2d );

		if ( atan2( dy, dx ) > to_rad( 45 ) ){
			*sx = *ex = mng->mss.x;
			*sy = (int)dvc[0].y;	*ey = (int)dvc[1].y;
			mng->dst = CUT_VRT;
		}
		else{
			*sx = (int)dvc[0].x;	*ex = (int)dvc[1].x;
			*sy = *ey = (int)mng->mss.y;
			mng->dst = CUT_HRZ;
		}
	}
	else{
		*sx = *sy = *ex = *ey = 0;
	}
}

void	Rbr_Bnd(					// �v�f�̉e�`��
HDC		hdc,						// �`��Ώۃf�o�C�X�R���e�L�X�g
Fig_mng	*mng )						// �}�`�Ǘ��\��
{
	int		sx, sy, ex, ey, typ=mng->rtp, rbr=TRUE;

	switch( mng->dtl ){
	case CHG_TOP:
	case CHG_SID:	switch( mng->hit->typ ){
					case ELM_LIN:	lin_sdw_( hdc, mng );	break;
					case ELM_RCT:	rct_sdw_( hdc, mng );	break;
					}
					rbr = FALSE;
					break;
	case CHG_CUT:	cal_cut_( &sx, &sy, &ex, &ey, mng );
					typ = ELM_LIN;
					break;
	default:		if ( ( mng->ope & OPE_CRT ) && !mng->hit ){
						sx = mng->mss.x;	sy = mng->mss.y;
						ex = mng->mse.x;	ey = mng->mse.y;
						if ( mng->hit )	typ = RBR_RCT;
					}
					else{
						if ( mng->hit ){
							sx = (int)(mng->mse.x + mng->dif.sp.x);
							ex = (int)(mng->mse.x + mng->dif.ep.x);
							sy = (int)(mng->mse.y + mng->dif.sp.y);
							ey = (int)(mng->mse.y + mng->dif.ep.y);
							typ = mng->hit->typ;
						}
					}
					break;
	}
	if ( rbr )	rbr_bnd_( hdc, sx, sy, ex, ey, typ );
}
