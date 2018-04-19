/*
	�}�`�쐬�E���샋�[�`��
	�Z�_�}�[�N�����֐�
*/

#include "fig_mnp.h"
#include "fig_mrk.h"

#define	ALW_AGL		15					// �����}�[�N�i���j�p�x(�f�O���[)
#define	ALW_SIZ		15					// �����}�[�N�i���j�T�C�Y�i�f�o�C�X�j

static	Dpn		Lft_mrk[] =	{			// �����}�[�N�i�������j�_��
	{ 151,107 }, { 151,116 }, { 136, 87 }, { 151, 58 }, { 151, 67 }
};

static	Dpn		Rit_mrk[] =	{			// �E���}�[�N�i�E�����j�_��
	{ 136,107 }, { 136,116 }, { 151, 87 }, { 136, 58 }, { 136, 67 }
};

typedef	struct	{						// ��܋�`
	Dpn		sp;							// �n�_
	Dpn		ep;							// �I�_
}Mrk_sbx;

static	Mrk_sbx		Mrk_srn = {			// ��L����܋�`
	{ 136, 58 }, { 151, 116 }
};

static	int		lin_mrk_(				// �����v�f�Ɍ`��t��
Lin		**dst,							// �i�[�̈�
Fig_elm	*elm,							// �����Ώۗv�f
Mtx_def	*mtx )							// ���W�ϊ��W��
{
	double		sta, siz, dx, dy;
//	double		sin(double), cos(double), atan2( double, double );
	Lin			*lin = &elm->elm.lin, *ptr;
	Dpn			ps1, ps2, pe1, pe2;
	int			cnt=0;

	dx = lin->ep->x - lin->sp->x;
	dy = lin->ep->y - lin->sp->y;

	siz = ALW_SIZ * mtx->xa;

	sta = atan2( dy, dx );
	if ( elm->mrk&MRK_LFT ){
		ps1.x = lin->sp->x + siz * cos( sta + to_rad(ALW_AGL) );
		ps1.y = lin->sp->y + siz * sin( sta + to_rad(ALW_AGL) );
		ps2.x = lin->sp->x + siz * cos( sta + to_rad(360.0-ALW_AGL) );
		ps2.y = lin->sp->y + siz * sin( sta + to_rad(360.0-ALW_AGL) );
		cnt += 2;
	}
	if ( elm->mrk&MRK_RIT ){
		pe1.x = lin->ep->x + siz * cos( sta + C_PI - to_rad(ALW_AGL) );
		pe1.y = lin->ep->y + siz * sin( sta + C_PI - to_rad(ALW_AGL) );
		pe2.x = lin->ep->x + siz * cos( sta + C_PI + to_rad(ALW_AGL) );
		pe2.y = lin->ep->y + siz * sin( sta + C_PI + to_rad(ALW_AGL) );
		cnt += 2;
	}
	*dst = ptr = (Lin *)calloc( cnt, sizeof(Lin) );
	if ( ptr ){
		if ( elm->mrk&MRK_LFT ){
			*ptr->sp = ps1;	*ptr->ep = *lin->sp;
			ptr++;
			*ptr->sp = ps2;	*ptr->ep = *lin->sp;
			ptr++;
		}
		if ( elm->mrk&MRK_RIT ){
			*ptr->sp = pe1;	*ptr->ep = *lin->ep;
			ptr++;
			*ptr->sp = pe2;	*ptr->ep = *lin->ep;
		}
	}
	return( ptr ? cnt: -1 );
}

static	void	srd_box_(				// �[�_�`��̕�܋�`�쐬
Mrk_sbx	*dst,							// �i�[��
Trm_mrk	*mrk )							// �[�_�`��Ǘ��\��
{
	Dpn			*ptr=mrk->pnt;
	int			cnt=mrk->pct-1;

	dst->sp = dst->ep = *ptr++;
	while( cnt-- ){
		dst->sp.x = min_( dst->sp.x, ptr->x );
		dst->sp.y = min_( dst->sp.y, ptr->y );
		dst->ep.x = max_( dst->ep.x, ptr->x );
		dst->ep.y = max_( dst->ep.y, ptr->y );
		ptr++;
	}
}

static	void	mak_mtx_(				// �ϊ��}�g���b�N�X�쐬
Mtx_def	*mtx,							// �i�[�̈�
Rct		*rct,							// �v�f�̋�`
Mrk_sbx	*sbx,							// ��܋�`
int		dir )							// ���E����
{
	Fld_def		src, dst;
	double		rto;

	rto = ( sbx->ep.x - sbx->sp.x ) / ( sbx->ep.y - sbx->sp.y );

	if ( dir == MRK_LFT ){
		dst.sx = rct->sp->x;	dst.sy = rct->sp->y;
		dst.ex = rct->sp->x + ( rct->ep->y - rct->sp->y + 1 ) * rto;
		dst.ey = rct->ep->y;
	}
	else{
		dst.sx = rct->ep->x - ( rct->ep->y - rct->sp->y + 1 ) * rto;
		dst.sy = rct->sp->y;
		dst.ex = rct->ep->x;	dst.ey = rct->ep->y;
	}
	src.sx = sbx->sp.x;	src.sy = sbx->sp.y;
	src.ex = sbx->ep.x;	src.ey = sbx->ep.y;

	Set_Mtx( mtx, &dst, &src, 0 );
}

static	int		add_hlf_(				// �Б��}�[�N�_��f�[�^�ǉ�
Trm_mrk	**dst,							// �i�[�̈�
Dpn		*dot,							// �}�[�N�_��
int		dct,							// �}�[�N�_��
Fig_elm	*elm,							// �����Ώۗv�f
int		dir )							// ���E����
{
	Dpn		*ptr, add[3];
	int		i, cnt=dct+3;

	*dst = (Trm_mrk *)calloc( 1, tmk_siz(cnt) );
	if ( *dst ){
		(*dst)->pct = cnt;
		ptr = (*dst)->pnt;
		add[1].y = dot->y;
		add[2] = *dot;
		while( dct-- )	*ptr++ = *dot++;
		add[0].y = (ptr-1)->y;
		if ( dir==MRK_LFT )	add[0].x = add[1].x = elm->elm.rct.ep->x;
		else				add[0].x = add[1].x = elm->elm.rct.sp->x;
		for ( i=0; i<3; i++ )	*ptr++ = add[i];
	}
	return( (*dst) ? cnt: -1 );
}

static	int		add_ful_(				// �Б��}�[�N�_��f�[�^�ǉ�
Trm_mrk		**dst,						// �i�[�̈�
Dpn			*lft,						// �}�[�N�_��
int			lct,						// �}�[�N�_��
COLORREF	lbc,						// Color of mark of left
Dpn			*rit,						// �}�[�N�_��
COLORREF	rbc,						// Color of mark of right
int			rct )						// �}�[�N�_��
{
	Dpn		*ptr, add[1];
	int		cnt=rct+lct+1;

	*dst = (Trm_mrk *)calloc( 1, tmk_siz(cnt) );
	if ( *dst ){
		(*dst)->pct = cnt;
		ptr = (*dst)->pnt;
		add[0] = *lft;
		while( lct-- )	*ptr++ = *lft++;
		rit += (rct-1);
		while( rct-- )	*ptr++ = *rit--;
		*ptr = add[0];
	}
	return( (*dst) ? cnt: -1 );
}

static	int		rct_def_(				// �f�t�H���g�̒[�_�}�[�N
Trm_mrk	**dst,							// �i�[�̈�
Fig_elm	*elm )							// �����Ώۗv�f
{
	Mtx_def		mtx;
	Dpn			lft[memcnt(Lft_mrk)], rit[memcnt(Rit_mrk)];
	int			cnt;
	Rct			*rct = &elm->elm.rct;

	if ( elm->mrk&MRK_LFT ){
		mak_mtx_( &mtx, &elm->elm.rct, &Mrk_srn, MRK_LFT );
		Dpn_Cnv( lft, Lft_mrk, memcnt(Lft_mrk), &mtx );
	}
	if ( elm->mrk&MRK_RIT ){
		mak_mtx_( &mtx, &elm->elm.rct, &Mrk_srn, MRK_RIT );
		Dpn_Cnv( rit, Rit_mrk, memcnt(Rit_mrk), &mtx );
	}
	if ( (elm->mrk&MRK_LFT) && (elm->mrk&MRK_RIT) ){
		cnt = add_ful_( dst, lft, memcnt(Lft_mrk), rct->lmc, rit, memcnt(Rit_mrk), rct->rmc );
	}
	else{
		if ( elm->mrk&MRK_LFT ){
			cnt = add_hlf_( dst, lft, memcnt(Lft_mrk), elm, MRK_LFT );
		}
		else{
			cnt = add_hlf_( dst, rit, memcnt(Rit_mrk), elm, MRK_RIT);
		}
	}
	if ( cnt != -1 )	(*dst)->bcl = elm->bcl;
	return( cnt!=1 ? 1: -1 );
}

static	int		mak_pnt_(				// �ϊ��_��쐬
Trm_mrk	**dst,							// �i�[�̈�
Trm_mrk	*mrk,							// �[�_�`��
Fig_elm	*elm,							// �����Ώۗv�f
int		dir )							// ���E����
{
	Mrk_sbx		sbx;
	Mtx_def		mtx;

	*dst = (Trm_mrk *)calloc( 1, tmk_siz( mrk->pct+1 ) );
	if ( dst ){
		srd_box_( &sbx, mrk );
		mak_mtx_( &mtx, &elm->elm.rct, &sbx, dir );
		Dpn_Cnv( (*dst)->pnt, mrk->pnt, mrk->pct, &mtx );
		(*dst)->bcl = mrk->bcl;
		(*dst)->pct = mrk->pct+1;
		(*dst)->pnt[mrk->pct] = (*dst)->pnt[0];
	}
	return( !!(*dst) );
}

static	int		mak_mdl_(				// ���Ԑ}�`�_��쐬
Trm_mrk	**dst,							// �i�[�̈�
Trm_mrk	*lft,							// ���[�_�`��
Trm_mrk	*rit,							// �E�[�_�`��
Fig_elm	*elm )							// �����Ώۗv�f
{
	int			cnt=( lft ? lft->pct+1: 4 ) + 1;
	Dpn			*pnt;

	*dst = (Trm_mrk *)calloc( 1, tmk_siz(cnt) );
	if ( *dst ){
		pnt = (*dst)->pnt;
		if ( lft ){
			memcpy( pnt, lft->pnt, sizeof(Dpn)*(lft->pct-1) );
			pnt += lft->pct - 1;
			if ( rit ){
				*pnt++ = *(rit->pnt + rit->pct-2);
				*pnt++ = *(rit->pnt);
			}
			else{
				pnt->x = elm->elm.rct.ep->x;
				pnt->y = (lft->pnt+lft->pct-2)->y;
				pnt++;
				pnt->x = elm->elm.rct.ep->x;
				pnt->y = lft->pnt->y;
				pnt++;
			}
		}
		else{
			pnt->x = elm->elm.rct.sp->x;
			pnt->y = rit->pnt->y;
			pnt++;
			pnt->x = rit->pnt->x;
			pnt->y = rit->pnt->y;
			pnt++;
			pnt->x = rit->pnt->x;
			pnt->y = (rit->pnt+rit->pct-2)->y;
			pnt++;
			pnt->x = elm->elm.rct.sp->x;
			pnt->y = (rit->pnt+rit->pct-2)->y;
			pnt++;
		}
		*pnt = *((*dst)->pnt);
		(*dst)->bcl = elm->bcl;
		(*dst)->pct = cnt;
	}
	return( !!elm );
}

static	int		rct_usr_(				// ���[�U�[��`�̒[�_�}�[�N
Trm_mrk	**dst,							// �i�[�̈�
Fig_elm	*elm,							// �����Ώۗv�f
Trm_mrk	*lft,							// �����`��
Trm_mrk	*rit )							// �E���`��
{
	Trm_mrk	*lpt=NULL, *rpt=NULL, *mpt=NULL, *ptr;
	int		cnt=0, lck=TRUE, rck=TRUE, siz;

	if ( lft && (elm->mrk&MRK_LFT) ){
		lck = mak_pnt_( &lpt, lft, elm, MRK_LFT );
		cnt++;
	}
	if ( rit && (elm->mrk&MRK_RIT) ){
		rck = mak_pnt_( &rpt, rit, elm, MRK_RIT );
		cnt++;
	}
	if ( lck && rck ){
		if ( mak_mdl_( &mpt, lpt, rpt, elm ) )	cnt++;
		else									cnt = -1;
	}
	else{
		cnt = -1;
	}
	if ( cnt != -1 ){
		siz  = lpt ? tmk_siz(lpt->pct): 0;
		siz += rpt ? tmk_siz(rpt->pct): 0;
		siz += mpt ? tmk_siz(mpt->pct): 0;

		if ( ( *dst = (Trm_mrk *)calloc( 1, siz ) ) != NULL ){
			ptr = *dst;
			if ( mpt ){
				memcpy( ptr, mpt, tmk_siz(mpt->pct) );
				ptr = nxt_tmk(ptr);
			}
			if ( lpt ){
				memcpy( ptr, lpt, tmk_siz(lpt->pct) );
				ptr->bcl = elm->elm.rct.lmc;
				ptr = nxt_tmk(ptr);
			}
			if ( rpt ){
				memcpy( ptr, rpt, tmk_siz(rpt->pct) );
				ptr->bcl = elm->elm.rct.rmc;
			}
		}
		else{
			cnt = -1;
		}
	}
	if ( lpt )	free( lpt );
	if ( rpt )	free( rpt );
	if ( mpt )	free( mpt );
	return( cnt );
}

static	int		rct_mrk_(				// ��`�v�f�Ɍ`��t��
Trm_mrk	**dst,							// �i�[�̈�
Fig_elm	*elm,							// �����Ώۗv�f
Fig_mng	*mng )							// �}�`�Ǘ��\��
{
	int			i, cnt, lmk, rmk, ect=Cnt_Lnk(mng->mrk);
	Trm_mrk		*lft=NULL, *rit=NULL;
	Csr			csr;

	lmk = elm->elm.rct.lmk;
	rmk = elm->elm.rct.rmk;

	if ( lmk==0 && rmk==0 ){
		cnt = rct_def_( dst, elm );
	}
	else{
		if ( lmk && lmk<=ect ){
			for ( i=0, csr=NULL; i<lmk; i++ )	csr = Nxt_Lnk( mng->mrk, csr );
			lft = Rec_Of(Trm_mrk, csr);
		}
		if ( rmk && rmk<=ect ){
			for ( i=0, csr=NULL; i<rmk; i++ )	csr = Nxt_Lnk( mng->mrk, csr );
			rit = Rec_Of(Trm_mrk, csr);
		}
		cnt = rct_usr_( dst, elm, lft, rit );
	}
	return( cnt );
}

int		Add_Mrk(						// �Z�_�`��t��
void	*dst,							// �i�[�̈�
Fig_elm	*elm,							// �����Ώۗv�f
Fig_mng	*mng )							// �}�`�Ǘ��\��
{
	int			cnt = 0;

	switch( elm->typ ){
	case ELM_LIN:	cnt = lin_mrk_( (Lin **)dst,     elm, mng->d2s );	break;
	case ELM_RCT:	cnt = rct_mrk_( (Trm_mrk **)dst, elm, mng );		break;
	}
	return( cnt );
}

int		Ent_Mrk(						// ���[�U�[��`�[�_�`��o�^
Trm_mrk	*ent,							// �o�^����[�_�`��Ǘ��\��
Fig_mng	*mng )							// �}�`�Ǘ��\��
{
	int			chk;

	chk = !!Add_Lnk( mng->mrk, (char *)ent, tmk_siz(ent->pct) );
	return( chk ? Cnt_Lnk(mng->mrk): 0 );
}

double	Mrk_Siz(						// �����p�}�[�N�T�C�Y�̎擾
Mtx_def		*mtx )						// �f�o�C�X���V�X�e�����W�ϊ��W��
{
	double		siz;//, cos(double);

	siz = ALW_SIZ * mtx->xa;
	return( siz * cos( to_rad(ALW_AGL) ) );
}
