/*
	�}�`�쐬�E���샋�[�`���ėp�����֐�
*/

//#include "afx.h"
#include "..\resource.h"
#include "fig_mnp.h"
#include "fig_drw.h"
#include "fig_ope.h"

#define	NER_LIN	2					// ����Ƃ݂Ȃ��͈́i�f�o�C�X�P�ʁj

#define	LFT		0x01				// ���E���̊O��
#define	RIT		0x02				// ���E���̊O��
#define	TOP		0x04				// ���E���̊O��
#define	BTM		0x08				// ���E���̊O��

static	int		pnt_chk_(			// �N���b�v�_ �`�F�b�N
Dpn		*dp,						// �����Ώۓ_
Dpn		*sp,						// �̈��`�n�_
Dpn		*ep )						// �̈��`�I�_
{
	int		chk=0;

	if		( dp->x < sp->x )	chk  = LFT;
	else if ( dp->x > ep->x )	chk |= RIT;

	if		( dp->y > ep->y )	chk |= TOP;
	else if	( dp->y < sp->y )	chk |= BTM;

	return( chk );
}

static	int		pnt_cal_(			// �N���b�v�_�Z�o
int		cod,						// �`�F�b�N�R�[�h
Lin		*lin,						// �Ώې���
Dpn		*sp,						// �̈��`�n�_
Dpn		*ep )						// �̈��`�I�_
{
	double		dx, dy, alp;

	dx = lin->ep->x - lin->sp->x;
	dy = lin->ep->y - lin->sp->y;

	if ( cod & (LFT|RIT) )	alp = dy / dx, cod &= (LFT|RIT);
	else					alp = dx / dy;

	switch (cod){
	case LFT:	lin->sp->y += (sp->x - lin->sp->x) * alp;
				lin->sp->x  =  sp->x;
				break;
	case RIT:	lin->sp->y += (ep->x - lin->sp->x) * alp;
				lin->sp->x  =  ep->x;
				break;
	case TOP:	lin->sp->x += (ep->y - lin->sp->y) * alp;
				lin->sp->y  =  ep->y;
				break;
	case BTM:	lin->sp->x += (sp->y - lin->sp->y) * alp;
				lin->sp->y  =  sp->y;
				break;
	}
	return ( pnt_chk_( lin->sp, sp, ep ) );
}

static	int		lin_clp_(			// ������̈���ɃN���b�s���O
Lin		*lin,						// �Ώے���
Dpn		*sp,						// �̈��`�n�_
Dpn		*ep )						// �̈��`�I�_
{
	int			sc, ec;

	sc = pnt_chk_( lin->sp, sp, ep );
	ec = pnt_chk_( lin->ep, sp, ep );

	while( (sc|ec) && !(sc&ec) ){
		if( sc )	sc = pnt_cal_( sc, lin, sp, ep );
		else		ec = pnt_cal_( ec, lin, sp, ep );
	}
	return( !sc );
}

int		inr_rct_(						// ��܌���
Dpn		*dsp,							// �팟���`�n�_
Dpn		*dep,							// �팟���`�I�_
Dpn		*ssp,							// �����`�n�_
Dpn		*sep )							// �����`�I�_
{
	int		chk;
	double	ssx, ssy, sex, sey, dsx, dsy, dex, dey;

	ssx = min( ssp->x, sep->x );
	ssy = min( ssp->y, sep->y );
	sex = max( ssp->x, sep->x );
	sey = max( ssp->y, sep->y );
	dsx = min( dsp->x, dep->x );
	dsy = min( dsp->y, dep->y );
	dex = max( dsp->x, dep->x );
	dey = max( dsp->y, dep->y );

	chk  = dsx >= ssx && dsx <= sex;
	chk &= dsy >= ssy && dsy <= sey;
	chk &= dex >= ssx && dex <= sex;
	chk &= dey >= ssy && dey <= sey;

	return( chk );
}

static	Fig_elm	*hit_tst_(				// �}�`�q�b�g����
Fig_mng	*mng,							// �I��Ώۃf�[�^
Dpn		*mus )							// �}�E�X���W�i���[���h���W�n�j
{
	Fig_elm		*elm;
	Csr			csr=NULL;
	int			chk=FALSE;
	Dpn			sp, ep;
	double		dlt;
	Lin			lin;

	dlt = NER_LIN * mng->d2s->xa;

	sp.x = mus->x - dlt; sp.y = mus->y - dlt;
	ep.x = mus->x + dlt; ep.y = mus->y + dlt;

	while( ( csr = Prv_Lnk( mng->top, csr ) ) != NULL ){
		elm = Rec_Of(Fig_elm, csr);
		switch( elm->typ ){
		case ELM_LIN:	lin = elm->elm.lin;
						chk = lin_clp_( &lin, &sp, &ep );
						break;
		case ELM_RCT:	chk = inr_rct_( mus, mus, &elm->srn.sp, &elm->srn.ep );
						break;
		}
		if ( chk )	break;
	}
	return( chk ? elm: NULL );
}

static	Csr		sel_end_(				// �͈͑I���I������
Fig_mng	*mng,							// �I��Ώۃf�[�^
Dpn		*wld )							// �}�E�X���W�i�V�X�e�����W�n�j
{
	Csr		csr=NULL, cse;
	Csr		sel;
	Fig_elm	*elm;
	int		chk;

	sel = (Csr)calloc( 1, sizeof(Lnk) );
	if ( sel ){
		while( ( csr = Nxt_Lnk( mng->top, csr ) ) != NULL ){
			elm = Rec_Of(Fig_elm, csr);
			chk = inr_rct_( &elm->srn.sp, &elm->srn.ep, wld, wld+1 );
			if ( chk ){
				if ( !( cse = Add_Lnk( sel, (char *)&elm, sizeof(elm) ) ) ){
					Del_All( sel );
					break;
				}
			}
		}
	}
	return( sel );
}

Fig_elm	*Hit_Tst(						// �}�`�q�b�g����
Fig_mng	*mng )							// �I��Ώۃf�[�^
{
	Dpn			wld[1], dvc[1];
	Fig_elm		*elm;

	dvc[0].x = (double)mng->mss.x;
	dvc[0].y = (double)mng->mss.y;

	Dpn_Cnv( wld, dvc, 1, mng->d2s );

	elm = hit_tst_( mng, wld );
	if ( mng->ocb ){
		mng->ocb( EVT_HIT, mng->rtp, 0, wld, wld, elm, mng->usr );
	}

	return( elm );
}

Fig_elm	*hvr_tst(						// �}�`�q�b�g����
int		msx,							// x coodinate of mouse
int		msy,							// y coodinate of mouse
Fig_mng	*mng )							// �I��Ώۃf�[�^
{
	Dpn			wld[1], dvc[1];
	Fig_elm		*elm;

	dvc[0].x = (double)msx;
	dvc[0].y = (double)msy;

	Dpn_Cnv( wld, dvc, 1, mng->d2s );

	elm = hit_tst_( mng, wld );

	return( elm );
}

int		Ope_Stt(						// ����J�n�C�x���g����
Fig_mng	*mng )							// ����Ώۃf�[�^
{
	Dpn			dvc[1], wld[1];
	int			ope, typ, dtl, chk=FALSE;

	if ( mng->ocb ){
		dvc->x = (double)mng->mss.x;
		dvc->y = (double)mng->mss.y;
		Dpn_Cnv( wld, dvc, 1, mng->d2s );

		switch( mng->dtl ){
		case CHG_TOP:
		case CHG_SID:	ope = EVT_CGS, typ=mng->dtl, dtl=mng->dir;
						break;
		case CHG_CUT:	ope = EVT_CTS, typ=mng->dtl, dtl=mng->dir;
						break;
		default:		if ( ( mng->ope & OPE_CRT) && !mng->hit ){
							ope = EVT_CRS, typ=mng->rtp, dtl=0;
						}
						else{
							if ( mng->hit ){
								ope = EVT_MVS, typ=mng->hit->typ, dtl=0;
							}
							else{
								ope = EVT_SLS, typ=0, dtl=0;
							}
						}
						break;
		}
		chk = mng->ocb( ope, typ, dtl, wld, wld, mng->hit, mng->usr );
	}
	return( chk );
}

int		Ope_End(						// ����I���C�x���g����
double	dtc,							// distance between left button down position and left button up position 
Fig_mng	*mng )							// �I��Ώۃf�[�^
{
	Dpn		wld[2], dvc[2];
	int		ope, typ, dtl, chk=FALSE;
	Fig_elm	*elm=mng->hit;

	if ( mng->ocb ){
		dvc[0].x = (double)mng->mss.x;	dvc[0].y = (double)mng->mss.y;
		dvc[1].x = (double)mng->mse.x;	dvc[1].y = (double)mng->mse.y;

		Dpn_Cnv( wld, dvc, 2, mng->d2s );

		switch( mng->dtl ){
		case CHG_TOP:
		case CHG_SID:	ope = EVT_CGE, typ=mng->dtl, dtl=mng->dir;
						break;
		case CHG_CUT:	ope = EVT_CTE, typ=mng->dtl, dtl=mng->dst;
						SetClassLong( mng->wnd, GCL_HCURSOR, cur_set_(IDC_CURSOR_CW1) );
						break;
		default:		if ( ( mng->ope & OPE_CRT ) && !mng->hit ){
							if ( dtc > 16.0 ){
								ope = EVT_CRE, typ=mng->rtp, dtl=0;
							}
						}
						else{
							if ( mng->hit ){
								if ( dtc > 16.0 ){
									ope = EVT_MVE, typ=mng->hit->typ, dtl=0;
								}
							}
							else{
								elm = (Fig_elm *)sel_end_( mng, wld );
								ope = EVT_SLE, typ=0, dtl=0;
							}
						}
						break;
		}
		chk = mng->ocb( ope, typ, dtl, wld, wld+1, elm, mng->usr );
	}
	mng->drg = FALSE;
	mng->dtl = DTL_NON;
	return( chk );
}

int		Ope_Drg(						// ����I���C�x���g����
int		msx,							// Current mouse x (device)
int		msy,							// Current mouse y (same)
int		mex,
int		mey,
Fig_mng	*mng )							// �I��Ώۃf�[�^
{
	Dpn		wld[2], dvc[2];
	int		dtl=0, chk=FALSE;
	Fig_elm	*elm=mng->hit;

	if ( mng->ocb ){
		if ( mng->drg ){
			dvc[0].x = (double)msx;	dvc[0].y = (double)msy;
			dvc[1].x = (double)mex;	dvc[1].y = (double)mey;
		}
		else{
			dvc[0].x = (double)msx;	dvc[0].y = (double)msy;
			dvc[1].x = 0;	dvc[1].y = 0;
		}

		Dpn_Cnv( wld, dvc, 2, mng->d2s );

		chk = mng->ocb( EVT_DRG, !!mng->drg, dtl, wld, mng->drg ? wld+1: NULL, NULL/*elm*/, mng->usr );
	}

	return( chk );
}

//----------------------------------------------------------------------------
static	int	top_ope_(					// ���_���쌟��
Dpn		*mus,							// �}�E�X���W
Dpn		*top,							// ���_���W
double	mgn )							// ����͈�
{
	Dpn			sp, ep;

	sp.x = top->x - mgn;	sp.y = top->y - mgn;
	ep.x = top->x + mgn;	ep.y = top->y + mgn;
	return( inr_rct_( mus, mus, &sp, &ep ) );

}

static	int			lin_ope_(			// �������쌟��
Fig_mng	*mng,							// �I��Ώۃf�[�^
Lin		*lin,							// ����Ώۃf�[�^
Dpn		*mus )							// �}�E�X���W�i���[���h�j
{
	double		mgn = ( SMK_SIZ / 2 ) * mng->d2s->xa;
	int			chk = DTL_NON;

	mng->dst  = top_ope_( mus, lin->sp, mgn ) ? SID_SP: 0;
	mng->dst |= top_ope_( mus, lin->ep, mgn ) ? SID_EP: 0;

	if ( mng->dst ){
		chk = CHG_TOP;
		SetClassLong( mng->wnd, GCL_HCURSOR, cur_set_( IDC_CURSOR_CRE ) );
	}
	return( chk );
}

static	int			rct_ope_(			// ��`���쌟��
Fig_mng	*mng,							// �I��Ώۃf�[�^
Rct		*rct,							// ����Ώۃf�[�^
Dpn		*mus )							// �}�E�X���W�i���[���h�j
{
	double		mgn = ( SMK_SIZ / 2 ) * mng->d2s->xa;
	Dpn			ltp, rtp, lbt, rbt, sp, ep;
	int			chk = DTL_NON;
	Lin			lin;
	LONG		cnm;

	ltp.x = rct->sp->x;	ltp.y = rct->ep->y;
	rbt.x = rct->ep->x;	rbt.y = rct->sp->y;
	rtp = *(rct->ep);	lbt = *(rct->sp);

	mng->dst  = top_ope_( mus, &lbt, mgn ) ? SID_LFT|SID_LWR: 0;
	mng->dst |= top_ope_( mus, &rtp, mgn ) ? SID_RIT|SID_UPR: 0;
	if ( mng->dst )	cnm = IDC_CURSOR_LDN;
	if ( !mng->dst ){
		mng->dst  = top_ope_( mus, &ltp, mgn ) ? SID_LFT|SID_UPR: 0;
		mng->dst |= top_ope_( mus, &rbt, mgn ) ? SID_RIT|SID_LWR: 0;
		if ( mng->dst )	cnm = IDC_CURSOR_RDN;
	}
	if ( mng->dst ){
		chk = CHG_TOP;
		SetClassLong( mng->wnd, GCL_HCURSOR, cur_set_( cnm ) );
	}

	if ( !chk ){
		sp.x = mus->x - mgn;	sp.y = mus->y - mgn;
		ep.x = mus->x + mgn;	ep.y = mus->y + mgn;

		*lin.sp = ltp;	*lin.ep = rtp;
		mng->dst  = lin_clp_( &lin, &sp, &ep ) ? SID_UPR: 0;
		*lin.sp = lbt;	*lin.ep = rbt;
		mng->dst |= lin_clp_( &lin, &sp, &ep ) ? SID_LWR: 0;
		*lin.sp = ltp;	*lin.ep = lbt;
		mng->dst |= lin_clp_( &lin, &sp, &ep ) ? SID_LFT: 0;
		*lin.sp = rtp;	*lin.ep = rbt;
		mng->dst |= lin_clp_( &lin, &sp, &ep ) ? SID_RIT: 0;
		if ( mng->dst ){
			cnm = ( mng->dst & (SID_RIT|SID_LFT) ) ? IDC_CURSOR_VRT: IDC_CURSOR_HRZ;
			chk = CHG_SID;
			SetClassLong( mng->wnd, GCL_HCURSOR, cur_set_( cnm ) );
		}
	}
	return( chk );
}

void	Set_Ope(						// �ڍב��샂�[�h�ݒ�
Fig_mng	*mng,							// �I��Ώۃf�[�^
int		msx,							// �}�E�XX���W
int		msy,							// �}�E�XY���W
int		key )							// �{�^���������
{
	Fig_elm			*elm, *fnd=NULL;
	int				cnt=0, ovf, dtl;
	Dpn				wld[2], dvc[2], sp[1], ep[1];
	LONG			cur;
	Csr				csr=NULL;

	dvc->x = msx;	dvc->y = msy;
	Dpn_Cnv( wld, dvc, 1, mng->d2s );
	mng->sts = DTL_NON;
	mng->fnd = NULL;

	while( ( csr = Nxt_Lnk( mng->top, csr ) ) != NULL ){
		elm = Rec_Of(Fig_elm, csr);
		if ( elm->sel ){
			if ( !cnt++ )	fnd = elm;
			else			break;
		}
	}
	if ( cnt==1 ){
//TRACE2("On dragging = %d, %d\n", msx, msy);
		if ( mng->ope & OPE_CHG ){
			switch( fnd->typ ){
			case ELM_LIN:	dtl=lin_ope_( mng, &fnd->elm.lin, wld );
							*sp = *fnd->elm.lin.sp;
							*ep = *fnd->elm.lin.ep;
							break;
			case ELM_RCT:	dtl=rct_ope_( mng, &fnd->elm.rct, wld );
							*sp = *fnd->elm.rct.sp;
							*ep = *fnd->elm.rct.ep;
							break;
			}
			mng->sts = dtl;
			mng->fnd = fnd;
			if ( mng->dst & SID_RIT )	mng->drm = SID_RIT;
			if ( mng->dst & SID_LFT )	mng->drm = SID_LFT;
			if ( mng->drg ){
//TRACE2("On dragging = %d, %d\n", msx, msy);
				if ( mng->drm & SID_RIT ){
					Dpn_Cnv( dvc, sp, 1, mng->s2d );
					Ope_Drg( (int)dvc->x, (int)dvc->y, msx, msy, mng );
//TRACE2("On right dragging = %d, %d\n", msx, msy);
				}
				else if ( mng->drm & SID_LFT ){
					Dpn_Cnv( dvc, ep, 1, mng->s2d );
					Ope_Drg( msx, msy, (int)dvc->x, (int)dvc->y, mng );
//TRACE2("On left dragging = %d, %d\n", msx, msy);
				}
			}
			else{
//TRACE2("NOO dragging = %d, %d\n", msx, msy);
				mng->drm = 0;
				Ope_Drg( msx, msy, 0, 0, mng );
			}
		}
		if ( ( mng->ope & OPE_CUT ) && !mng->sts ){
			if ( fnd->typ==ELM_RCT ){
				ovf = inr_rct_( wld, wld, &fnd->srn.sp, &fnd->srn.ep );
				if ( ( !mng->drg && ovf ) || mng->drg ){
					cur = key & MK_LBUTTON ? IDC_CURSOR_CW1: IDC_CURSOR_CW2;
					SetClassLong( mng->wnd, GCL_HCURSOR, cur_set_( cur ) );
					mng->sts = CHG_CUT;
					mng->fnd = fnd;
				}
			}
		}
		if ( !( mng->dtl & CHG_SID ) && mng->drg ){
			int	sx, sy, ex, ey;
			wld->x = fnd->elm.rct.sp->x;
			wld->y = fnd->elm.rct.sp->y;
			(wld+1)->x = fnd->elm.rct.ep->x;
			(wld+1)->y = fnd->elm.rct.ep->y;
			Dpn_Cnv( dvc, wld, 2, mng->s2d );
			sx = (int)dvc->x + ( msx - mng->mss.x );
			sy = (int)dvc->y + ( msy - mng->mss.y );
			ex = (int)(dvc+1)->x + ( msx - mng->mss.x );
			ey = (int)(dvc+1)->y + ( msy - mng->mss.y );
			Ope_Drg( sx, sy, ex, ey, mng );
		}
	}
	else{
//TRACE2("Off dragging = %d, %d\n", msx, msy);
		if ( mng->drg ){
			Ope_Drg( mng->mss.x, mng->mss.y, msx, msy, mng );
		}
		else{
			Ope_Drg( msx, msy, 0, 0, mng );
		}
	}
	if ( mng->sts==DTL_NON ){
		if ( hvr_tst( msx, msy, mng ) )	cur = IDC_CURSOR_ARW;
		else							cur = ( mng->ope & OPE_CRT ) ? IDC_CURSOR_CRE: IDC_CURSOR_ARW;
		SetClassLong( mng->wnd, GCL_HCURSOR, cur_set_( cur ) );
	}
}

void	Rdn_Ope(
Fig_mng	*mng,							// �I��Ώۃf�[�^
int		msx,							// �}�E�XX���W
int		msy,							// �}�E�XY���W
int		key )							// �{�^���������
{
	Fig_elm			*elm, *fnd=NULL;
	int				cnt=0, ovf, dtl;
	Dpn				wld[1], dvc[1], sp[1], ep[1];
	LONG			cur;
	Csr				csr=NULL;

	dvc->x = msx;	dvc->y = msy;
	Dpn_Cnv( wld, dvc, 1, mng->d2s );
	mng->sts = DTL_NON;
	mng->fnd = NULL;

	while( ( csr = Nxt_Lnk( mng->top, csr ) ) != NULL ){
		elm = Rec_Of(Fig_elm, csr);
		if ( elm->sel ){
			if ( !cnt++ )	fnd = elm;
			else			break;
		}
	}
	if ( cnt==1 ){
		if ( mng->ope & OPE_CHG ){
			switch( fnd->typ ){
			case ELM_LIN:	dtl=lin_ope_( mng, &fnd->elm.lin, wld );
							*sp = *fnd->elm.lin.sp;
							*ep = *fnd->elm.lin.ep;
							break;
			case ELM_RCT:	dtl=rct_ope_( mng, &fnd->elm.rct, wld );
							*sp = *fnd->elm.rct.sp;
							*ep = *fnd->elm.rct.ep;
							break;
			}
			mng->sts = dtl;
			mng->fnd = fnd;
			if ( mng->dst & SID_RIT )	mng->drm = SID_RIT;
			if ( mng->dst & SID_LFT )	mng->drm = SID_LFT;
			if ( mng->drg ){
				if ( mng->drm & SID_RIT ){
					Dpn_Cnv( dvc, sp, 1, mng->s2d );
					Ope_Drg( (int)dvc->x, (int)dvc->y, msx, msy, mng );
				}
				else if ( mng->drm & SID_LFT ){
					Dpn_Cnv( dvc, ep, 1, mng->s2d );
					Ope_Drg( msx, msy, (int)dvc->x, (int)dvc->y, mng );
				}
			}
			else{
				mng->drm = 0;
				Ope_Drg( msx, msy, 0, 0, mng );
			}
		}
		if ( ( mng->ope & OPE_CUT ) && !mng->sts ){
			if ( fnd->typ==ELM_RCT ){
				ovf = inr_rct_( wld, wld, &fnd->srn.sp, &fnd->srn.ep );
				if ( ( !mng->drg && ovf ) || mng->drg ){
					cur = key & MK_LBUTTON ? IDC_CURSOR_CW1: IDC_CURSOR_CW2;
					SetClassLong( mng->wnd, GCL_HCURSOR, cur_set_( cur ) );
					mng->sts = CHG_CUT;
					mng->fnd = fnd;
				}
			}
		}
	}
	else{
		if ( mng->drg ){
			Ope_Drg( mng->mss.x, mng->mss.y, msx, msy, mng );
		}
		else{
			Ope_Drg( msx, msy, 0, 0, mng );
		}
	}
	if ( mng->sts==DTL_NON ){
		cur = ( mng->ope & OPE_CRT ) ? IDC_CURSOR_CRE: IDC_CURSOR_ARW;
		SetClassLong( mng->wnd, GCL_HCURSOR, cur_set_( cur ) );
	}
}
