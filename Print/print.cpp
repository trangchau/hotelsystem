/*------------------------------------------
    �H�����̓v���O���� for Windows Ver 5.0
		Copyright 2001 S.Nagaoka
			����֌W�֐��t�@�C��
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

void	Get_Pdv_Inf(			// �v�����^�[���擾
Prn_mng		*mng,				// ���W�ϊ��}�g���b�N�X
int			mod )				// ������[�h
{
	Fld_def		out, inp;						// �o�͑��A���͑�
	HDC			dph;							// �ި���ڲDC�����
	const int	ppi = 72;						// �߲��/inch
	const float	mpi = (float)25.4;						// mm/inch
	double sqrt( double );

	if ( mod == MOD_PRN ){
		if ( mng->hdc ){
			mng->dev->xmm = GetDeviceCaps( mng->hdc, HORZSIZE );	  	// ����\�̈�(mm)
			mng->dev->ymm = GetDeviceCaps( mng->hdc, VERTSIZE );	  	// ����\�̈�(mm)
			mng->dev->xpc = GetDeviceCaps( mng->hdc, HORZRES );  	  	// ����\�̈�(�ޯ�)
			mng->dev->ypc = GetDeviceCaps( mng->hdc, VERTRES ); 	  	// ����\�̈�(�ޯ�)
			mng->dev->hmg = GetDeviceCaps( mng->hdc, PHYSICALOFFSETX );// �����}�[�W��(�ޯ�)
			mng->dev->vmg = GetDeviceCaps( mng->hdc, PHYSICALOFFSETY );// �����}�[�W��(�ޯ�)
			out.sx = 0;
			out.ey = 0;
			out.ex = (double)mng->dev->xpc / mng->dev->xmm;	// 1mm��������ޯĐ��i���j
			out.sy = (double)mng->dev->ypc / mng->dev->ymm;	// 1mm��������ޯĐ��i�x�j
			inp.sx = inp.ey = 0;
			inp.ex = inp.sy = 1 / MM_UNT;
			Get_Mtx( mng->mtx, &out, &inp, DEF_MTX );
			mng->dev->dpp =  (float)mng->dev->ypc / mng->dev->ymm  * mpi / ppi;
		}
	}
	if ( mod == MOD_PRV ){
		dph = CreateIC( _T("DISPLAY"), NULL, NULL, NULL );
		if ( dph ){
			mng->dev->xmm = GetDeviceCaps( dph, HORZSIZE );// ������ʂ̕� (mm)
			mng->dev->ymm = GetDeviceCaps( dph, VERTSIZE );// ������ʂ̍��� (mm)
			mng->dev->xpc = GetDeviceCaps( dph, HORZRES ); // �f�B�X�v���C�̕�(�s�N�Z���P��)
			mng->dev->ypc = GetDeviceCaps( dph, VERTRES ); // �f�B�X�v���C�̍���(���X�^�s�P��)
			out.sx = out.ey = 0;
			out.ex = (double)mng->dev->xpc / mng->dev->xmm;	// 1mm��������߸�ِ��i���j
			out.sy = (double)mng->dev->ypc / mng->dev->ymm;	// 1mm��������߸�ِ��i�x�j
			inp.sx = inp.ey =0;
//			inp.ex = inp.sy = 1 / MM_UNT / ( (double)FScale / 100 );//�g�嗦�i�Ӂj
			inp.ex = inp.sy = ( 1 / MM_UNT ) / ( sqrt ( mng->prv->scl ) / 10 );//�g�嗦�i�ʐρj
			Get_Mtx( mng->mtx, &out, &inp, DEF_MTX );
			mng->dev->dpp =  (float)(mng->dev->ypc / mng->dev->ymm  * mpi / ppi
														* sqrt( mng->prv->scl / 100.0 ));
			DeleteDC( dph );
		}
	}
}

static	void	mak_pen_(				// �y���n���h������
HPEN		*pen,						// �y���n���h��
int			sty,						// �X�^�C��
int			wid,						// �����i1/10mm�j
int			col )						// ���F
{
	LOGBRUSH	brs;						// �u���V�\����

	brs.lbStyle =  BS_SOLID;
	brs.lbColor = col;
	brs.lbHatch = 0;

	*pen = ExtCreatePen( PS_GEOMETRIC | sty | PS_ENDCAP_SQUARE | PS_JOIN_MITER,
													wid, &brs, 0, NULL );
}

static	void	slc_brs_(				// �u���V���[�h�I��
int			brs,						// ���[�U�[�I���u���V
int			*htg,						// �n�b�`���O�L��
int			*mod )						// �n�b�`���O���[�h
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

static	void	out_txt_(				// �e�L�X�g�^�C�v�o��
HDC			dvh,						// �f�o�C�X�n���h��
Txt_def		*dat, 						// �f�[�^�|�C���^
Prn_mng		*mng,						// ����f�[�^�\��
int			mag )						// �}�[�W���L��
{
	int					i, omd;
	RECT				rct;					// �o�͋�`
	Dpn					bfr[2], aft[2];			// ���W�ϊ��O�A��
	HFONT				fnh;					// �t�H���g�n���h��
	HGDIOBJ				ofn;					// ��̫��̫��
	LOGFONT				fnt;		   			// �t�H���g���\����
	COLORREF			otc, obc;				// �f�t�H���g�F

	memset( &fnt, 0, sizeof( LOGFONT ) );
	if ( dvh && dat->vsb && dat->txt ){
		bfr[0].x = dat->xps;
		bfr[0].y = dat->yps;
		bfr[1].x = dat->xps + dat->wid;
		bfr[1].y = dat->yps + dat->hgt;
		Dpn_Cnv( aft, bfr, 2, mng->mtx );			// �ϊ����W�擾

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
		wcscpy_s( fnt.lfFaceName, memcnt(fnt.lfFaceName), dat->fnt );			// �t�H���g��
		fnt.lfHeight = (long)(mng->dev->dpp * dat->siz + 0.5);// �t�H���g�T�C�Y
		if ( !fnt.lfHeight )	fnt.lfHeight = 1;	// �Œ�l
		fnt.lfUnderline = !!( dat->uln );			// ��������`����
		fnt.lfStrikeOut = !!( dat->cln );			// ��������`�ŏ���
		fnt.lfItalic = !!( dat->itl ); 			  	// �X�^�C���`�Α�
		fnt.lfWeight = dat->bld;	   				// �X�^�C���`�{�[���h�i400,700)
		fnt.lfQuality = PROOF_QUALITY;
		fnh = CreateFontIndirect( &fnt );
		if ( fnh != NULL ){
			ofn = SelectObject( dvh, fnh );
			otc = GetTextColor( dvh );
			SetTextColor( dvh, dat->fcl );
			obc = GetBkColor( dvh );
			SetBkColor( dvh, dat->bcl );
			if ( dat->fcl == dat->bcl )
				omd = SetBkMode( dvh, TRANSPARENT );// �g�����X�y�A�����g�ݒ�
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

static	void	out_arc_(				// �~�ʃ^�C�v�o��
HDC			dvh,						// �f�o�C�X�n���h��
Arc_def		*dat, 						// �f�[�^�|�C���^
Prn_mng		*mng,						// ����f�[�^�\��
int			mag )						// �}�[�W���L��
{
	int				i;
	int				htg, mod;					// �n�b�`���O���[�h
	Dpn				bfr[6], aft[6];				// ���W�ϊ��O�A��
	HPEN			pen;						// �y��
	HBRUSH			brs;						// �u���V
	HGDIOBJ			opn, obs;					// ��̫����݁A��׼obj
	static	double	cst = 10000.0;

	if ( dvh && dat->vsb ){
		bfr[0].x = dat->xps - dat->xrs;			// ��`left
		bfr[0].y = dat->yps - dat->yrs;			// ��`top
		bfr[1].x = dat->xps + dat->xrs;			// ��`right
		bfr[1].y = dat->yps + dat->yrs;			// ��`bottom
		bfr[2].x = dat->xps;					// ���S�����W
		bfr[2].y = dat->yps;					// ���S�����W
		bfr[3].x = 0;				   			// ����
		bfr[3].y = dat->lnw;		   			// ����
		if ( dat->typ != TYP_CCL ){
		//  �~�ʂ̎n�I�_�p�͉E��n�ɂĎw��ׁ̈A�����t�]
			bfr[4].x = bfr[2].x + cst * cos( -dat->brd );	// �J�n�p�����W
			bfr[4].y = bfr[2].y + cst * sin( -dat->brd );	// �J�n�p�����W
			bfr[5].x = bfr[2].x + cst * cos( -dat->erd );	// �I���p�����W
			bfr[5].y = bfr[2].y + cst * sin( -dat->erd );	// �I���p�����W
		}
		Dpn_Cnv( aft, bfr, 6, mng->mtx );		// �ϊ����W�擾
		if ( mag ){
			for ( i=0; i<=2; i++ ){
				aft[i].x -= mng->dev->hmg;
				aft[i].y -= mng->dev->vmg;
			}
		}
		if ( dat->lnw > 0 ){					// �y���쐬
			if ( aft[3].y > 0 && aft[3].y < 1 )		aft[3].y = 1;
			mak_pen_( &pen, dat->pns, (int)aft[3].y, dat->fcl );
		}
		else	pen = (HPEN)::GetStockObject( NULL_PEN );


		if ( dat->brs && dat->typ != TYP_ARC ){	// �u���V�쐬
			slc_brs_( dat->brs, &htg, &mod );
			if ( htg )			brs = CreateHatchBrush( mod, dat->bcl );
			else				brs = CreateSolidBrush( dat->bcl );
		}
		else					brs = (HBRUSH)::GetStockObject( NULL_BRUSH );

		if ( pen &&  ( brs || dat->typ == TYP_ARC ) ){
			opn = SelectObject( dvh, pen );
			obs = SelectObject( dvh, brs );
			switch ( dat->typ ){
			case	TYP_ARC:					// �~�ʃ^�C�v
				Arc( dvh, (int)aft[0].x, (int)aft[0].y, (int)aft[1].x, (int)aft[1].y,
						(int)aft[4].x, (int)aft[4].y, (int)aft[5].x, (int)aft[5].y );
				break;
			case	TYP_CHD:					// ���^�C�v
				Chord( dvh, (int)aft[0].x, (int)aft[0].y, (int)aft[1].x, (int)aft[1].y,
						(int)aft[4].x, (int)aft[4].y, (int)aft[5].x, (int)aft[5].y );
				break;
			case	TYP_PIE:					// ��^�C�v
				Pie( dvh, (int)aft[0].x, (int)aft[0].y, (int)aft[1].x, (int)aft[1].y,
						(int)aft[4].x, (int)aft[4].y, (int)aft[5].x, (int)aft[5].y );
				break;
			case	TYP_CCL:					// �~�^�C�v
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

static	void	out_rct_(				// ��`�^�C�v�o��
HDC			dvh,						// �f�o�C�X�n���h��
Rct_def		*dat, 						// �f�[�^�|�C���^
Prn_mng		*mng,						// ����f�[�^�\��
int			mag )						// �}�[�W���L��
{
	int				lft, top, rgt, btm;			// ����ʒu
	int				htg, mod;					// �n�b�`���O���[�h
	Dpn				bfr[4], aft[4];				// ���W�ϊ��O�A��
	HPEN			pen;						// �y��
	HBRUSH			brs;						// �u���V
	HGDIOBJ			opn, obs;					// ��̫����݁A��׼obj

	if ( dvh && dat->vsb ){
		bfr[0].x = dat->xps;
		bfr[0].y = dat->yps;
		bfr[1].x = dat->wid;
		bfr[1].y = dat->hgt;
		bfr[2].x = 0;
		bfr[2].y = dat->lnw;
		bfr[3].x = 0;
		bfr[3].y = 40;							// �p�۔��a(�Œ�)

		Dpn_Cnv( aft, bfr, 4, mng->mtx );		// �ϊ����W�擾
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
		if ( dat->lnw > 0 ){					// �y���쐬
			if ( aft[2].y > 0 && aft[2].y < 1 )		aft[2].y = 1;
			mak_pen_( &pen, dat->pns, (int)aft[2].y, dat->fcl );
		}
		else	pen = (HPEN)::GetStockObject( NULL_PEN );
		if ( dat->brs ){						// �u���V�쐬
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

static void out_lin_(					// ���^�C�v���
HDC			dvh,						// �f�o�C�X�n���h��
Lin_def		*dat, 						// �f�[�^�|�C���^
Prn_mng		*mng,						// ����f�[�^�\��
int			mag )						// �}�[�W���L��
{
	int				bgx, bgy, edx, edy;			// ����ʒu
	Dpn				bfr[3], aft[3];				// ���W�ϊ��O�A��
	HPEN			pen = NULL;					// �y���n���h��
	HGDIOBJ			opn;						// ��̫�����

	if ( dvh && dat->vsb && dat->lnw > 0 ){
		bfr[0].x = dat->bgx;
		bfr[0].y = dat->bgy;
		bfr[1].x = dat->edx;
		bfr[1].y = dat->edy;
		bfr[2].x = 0;
		bfr[2].y = dat->lnw;
		Dpn_Cnv( aft, bfr, 3, mng->mtx );		// �ϊ����W�擾
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

static	void	out_bmp_(				// �r�b�g�}�b�v�^�C�v�o��
HDC			dvh,						// �f�o�C�X�n���h��
Bmp_def		*dat,						// �f�[�^�|�C���^
Prn_mng		*mng,						// ����f�[�^�\��
int			mag )						// �}�[�W���L��
{
	int			omd, ifs;
	DWORD		ims;
	char		*img;
	int			chk;
	HBITMAP		bmp;
	BITMAPINFO	*bif;
	Dpn			bfr[2], aft[2];					// ���W�ϊ��O�A��

	if ( dvh && dat->vsb && mng->ins && dat->bmp ){
		bfr[0].x = dat->xps;
		bfr[0].y = dat->yps;
		bfr[1].x = dat->wid;
		bfr[1].y = dat->hgt;
		Dpn_Cnv( aft, bfr, 2, mng->mtx );			// �ϊ����W�擾
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

static void out_blk_(					//	�u���b�N�^�C�v�o��
HDC			dvh,						// �f�o�C�X�n���h��
Blk_def		*dat,						// �f�[�^�|�C���^
Prn_mng		*mng,						// ����f�[�^�\��
int			mag,						// �}�[�W���L��
HRGN		org )						// �f�t�H���g���[�W����(Prv�̂�)
{
	int			i;
	void		*ptr;							// �f�[�^�|�C���^
	Rct_def		flm;							// �u���b�N�g
	POINT		opt;							// �ȑO�̌��_�ʒu
	HRGN		rgn;							// �N���b�v���[�W����
	Dpn			bfr[2], aft[2];					// ���W�ϊ��O�A��

	if ( dvh && dat->vsb ){

		if ( dat->lnw > 0 || dat->brs ){		// �g�o��
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
									(int)aft[0].y, &opt );// ���_�ʒu�ύX
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
			ExtSelectClipRgn( dvh, rgn, RGN_AND );			// �d�����[�W����
			if ( ( ptr = dat->txt ) != NULL ){
				for ( i=0; i < dat->txc; i++ ){
					out_txt_( dvh, (Txt_def *)ptr, mng, false );	// ������o��
					ptr = ((Txt_def *)ptr) + 1;
				}
			}
			if ( ( ptr = dat->rct ) != NULL ){
				for ( i=0; i < dat->rcc; i++ ){
					out_rct_( dvh, (Rct_def *)ptr, mng, false );	// ��`�o��
					ptr = ((Rct_def *)ptr) + 1;
				}
			}
			if ( ( ptr = dat->lin ) != NULL ){
				for ( i=0; i < dat->lic; i++ ){
					out_lin_( dvh, (Lin_def *)ptr, mng, false );	// ���o��
					ptr = ((Lin_def *)ptr) + 1;
				}
			}
			if ( ( ptr = dat->arc ) != NULL ){
				for ( i=0; i < dat->acc; i++ ){
					out_arc_( dvh, (Arc_def *)ptr, mng, false );	// �~�ʏo��
					ptr = ((Arc_def *)ptr) + 1;
				}
			}
			if ( ( ptr = dat->bmp ) != NULL ){
				for ( i=0; i < dat->bmc; i++ ){
					out_bmp_( dvh, (Bmp_def *)ptr, mng, false );	// �ޯ�ϯ�ߏo��
					ptr = ((Bmp_def *)ptr) + 1;
				}
			}
			SelectObject( dvh, org );
			DeleteObject( rgn );
		}
		SetViewportOrgEx( dvh, opt.x, opt.y, NULL );
	}
}

void	Prn_Out_Dat(					// ����f�[�^�o��
HDC			dvh,						// �f�o�C�X�n���h��
Prn_mng		*mng, 						// ����f�[�^
int			mod,						// �o�͐�
HRGN		rgn )						// �f�t�H���g���[�W����
{
	int			cnt;
	int			mag;					// �}�[�W���w��
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

int			Prn_Opn(				// �v�����^�[�I�[�v��
WCHAR			*pnm,				// �v�����^�[��
Prn_mng			*mng )				// ����Ǘ��\����
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

int			Prn_Exc(				// ������s
int				cpy,				// �o�͕���
Prn_mng			*mng )				// ����Ǘ��\����
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

int			Prn_Cls(				// �v�����^�[�N���[�Y
Prn_mng			*mng )				// ����Ǘ��\����
{
	int			chk;

	if ( chk = !!mng->hdc ){
		EndDoc( mng->hdc );
		DeleteDC( mng->hdc );
	}
	return( chk );
}
