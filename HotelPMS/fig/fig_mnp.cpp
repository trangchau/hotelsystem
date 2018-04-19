/*
	�}�`�쐬�E���샋�[�`���������֐�
*/

#include "..\resource.h"
//#include <windows.h>
#include "fig_mnp.h"
#include "fig_mrk.h"
#include "fig_win.h"
#include "fig_ope.h"

#define	QUE_CNT			64					// �����L���[�\��

static	void Fld_Chk(				/*  �k�ڗ����K��  */
	Fld_def			*src,			/*  �Q�Ƒ�        */
	Fld_def			*dst)			/*  �o�͑�        */
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

static void mak_cnv_(						// �ϊ��s��쐬
Fig_mng			*mng )						// �`��Ǘ��\��
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

Fig_mng		*Fig_Set_Cbk(					// �R�[���o�b�N�֐��ݒ�
HANDLE		hnd,							// �`��Ǘ��\���n���h��
Mus_Cbk		mus,							// �}�E�X�R�[���o�b�N
Ope_Cbk		ope,							// ����R�[���o�b�N
Drw_Cbk		drw,							// �`��R�[���o�b�N
LPVOID		usr )							// ���[�U�[�f�[�^
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

HWND		Fig_Ini(							// �`�揉��������
HINSTANCE	ins,							// �A�v���P�[�V�����C���X�^���X
HWND		par,							// �e�E�B���h�E�n���h��
Area		*dvc,							// �q�E�B���h�E�ʒu�E�T�C�Y
Area		*vpt,							// �f�o�C�X�r���[�|�[�g
Area		*wpt,							// �V�X�e���E�B���h�E�|�[�g
Csr			top,							// ��w�̊􉽍\����
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

void	Fig_Shw(							// �E�B���h�E�\���^��\������
HANDLE	hnd,								// �`��Ǘ��\���n���h��
int		shw )								// �\���t���O
{
	Fig_mng			*mng = (Fig_mng *)hnd;

	if ( mng ){
		ShowWindow( mng->wnd, shw ? SW_SHOW: SW_HIDE );
	}
}

void	Fig_Set_Swn(						// �V�X�e�����W�n�̍X�V
HANDLE	hnd,								// �`��Ǘ��\���n���h��
Area	*wpt,								// �V�����V�X�e�����W�̈�
int		rdw )								// �ĕ`��t���O
{
	Fig_mng			*mng = (Fig_mng *)hnd;

	if ( mng ){
		mng->wpt = *wpt;
		mak_cnv_( mng );
		if ( rdw )	Fig_Rdw( hnd );
	}
}

void	Fig_Set_Dwn(						// �f�o�C�X���W�n�̍X�V
HANDLE	hnd,								// �`��Ǘ��\���n���h��
Area	*wpt,								// �V�����V�X�e�����W�̈�
Area	*dvc,								// �V�����f�o�C�X�̈�
Area	*vpt,								// �V�����f�o�C�X�`��̈�
int		rdw,								// �ĕ`��t���O
int		mtx )								// �ϊ��}�g���b�N�X�X�V�t���O
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

void	Fig_Rdw(							// �S�ĕ`�揈��
HANDLE	hnd )								// �`��Ǘ��\���n���h��
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

void	Fig_Rct_Drw(						// �w���`���ĕ`��
HANDLE	hnd,								// �`��Ǘ��\���n���h��
Dpn		*sp,								// ��`�n�_
Dpn		*ep )								// ��`�I�_
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

int		Fig_Set_Ope(						// �}�`����̐ݒ�
HANDLE		hnd,							// �`��Ǘ��\���n���h��
int			ope,							// �J�n���鑀����
int			rtp )							// ���o�[�o���h�̎�ʁi�쐬�p�j
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

void	Fig_Drw_Elm(						// �P��􉽍\���̍ĕ`��
HANDLE	hnd,								// �`��Ǘ��\���n���h��
Fig_elm	*elm )								// �`��Ώۊ􉽍\��
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

int		Fig_Fnt_Siz(						// �f�o�C�X�n�����T�C�Y�擾
HANDLE	hnd,								// �`��Ǘ��\���n���h��
double	siz )								// �����T�C�Y�i���[���h���W�P�ʁj
{
	Fig_mng			*mng = (Fig_mng *)hnd;
	int				ret = 1;

	if ( mng ){
		ret = (int)(siz * mng->frt);
	}
	return( ret );
}

int		Fig_Ent_Tmk(						// �[�_�}�[�N�o�^
HANDLE	hnd,								// �`��Ǘ��\���n���h��
Trm_mrk	*ent )								// �o�^����[�_�`��Ǘ��\��
{
	Fig_mng			*mng = (Fig_mng *)hnd;
	int				ret = 0;

	if ( mng ){
		ret = Ent_Mrk( ent, mng );
	}
	return( ret );
}

void	Fig_Trm(							// �`��I������
HANDLE	hnd )								// �`��Ǘ��\���n���h��
{
	Fig_mng			*mng = (Fig_mng *)hnd;

	if ( mng ){
		SendMessage( mng->wnd, WM_LBUTTONUP, 0, 0 );
		Del_All( mng->mrk );
		SendMessage( mng->wnd, WM_CLOSE, 0, 0 );
		free( mng );
	}
}

