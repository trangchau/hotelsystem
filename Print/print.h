/*------------------------------------------
    �H�����̓v���O���� for Windows Ver 5.0
		Copyright 2001 S.Nagaoka
			����֌W��`�t�@�C��
--------------------------------------------*/
#ifndef	printH
#define	printH

#include "afn_cnv.h"

#define	PAI			3.14159265358979323846
#define	HPI			1.57079632679489661923
#define	QPI			0.785398163397448309616
#define	To_Deg(x)	( (x) * 180.0 / PAI )
#define To_Rad(x)	( (x) * PAI / 180.0 )

#define	MAX_PNM			128		// �v�����^�[���ő咷��
#define	MAX_PPR			64		// �p�����ő咷��

#define	COL_BLK		(0)				// ��
#define	COL_MRN		(128)			// �}���[��
#define	COL_GRN		(32768)			// ��
#define	COL_OLV		(32896)			// �I���[�u
#define	COL_NVY		(8388608)		// �l�C�r�[
#define	COL_PRP		(8388736)		// ��
#define	COL_TEL		(8421376)		// �e�B�A��
#define	COL_GRY		(8421504)		// �O���C
#define	COL_SVR		(12632256)		// ��
#define	COL_RED		(255)			// ��
#define	COL_LIM		(65280)			// ���C��
#define	COL_YLW		(65535)			// ��
#define	COL_BLU		(16711680)		// ��
#define	COL_FCS		(16711935)		// �H
#define	COL_AQA		(16776960)		// ���F
#define	COL_LGL		(12632256)		// ���C�g�O���[
#define	COL_DGL		(8421504)		// �_�[�N�O���[
#define	COL_WHT		(16777215)		// ��
#define	COL_NON		(536870911)		// �Ȃ�

#define DEF_MTX 	0		// �َ���W(0:��  1:�x���]  2:�w���])
#define	MM_UNT		0.1		// �T�C�Y�P�� (x/mm)
#define	STC_TXT		1		// ������^�C�v
#define	STC_RCT		2		// �}�`�^�C�v
#define	STC_LIN		3		// ���^�C�v
#define	STC_ARC		4		// �~�ʃ^�C�v
#define	STC_BMP		5		// �r�b�g�}�b�v�^�C�v
#define	STC_BLK		6		// �u���b�N�^�C�v

/*	All_Dat�\����	*/
#define	MOD_PRN     0       // �v�����^�[�o��
#define	MOD_PRV     1		// �v���r���[�o��

/*	�e�L�X�g�^�C�v�\����	*/
#define	FST_NML		400		// �{�[���h�i�W���j
#define	FST_BLD		700		// �{�[���h�i�����j

#define	SGL_LIN		0x0020	// �P��s
#define	HRZ_CNT		0x0001	// ���������Z���^�[�����O
#define	HRZ_LFT		0x0000	// ���l
#define	HRZ_RGT		0x0002	// �E�l
#define	VRT_CNT		0x0004	// ���������Z���^�[�����O�i�P��s���̂݁j
#define	VRT_TOP		0x0000	// ��t��
#define	VRT_BTM		0x0008	// ���t��
#define	WRD_BRK		0x0010	// ���[�h�u���C�N

/*	�~�ʃ^�C�v�\����	*/
#define	TYP_ARC		0		// �~�ʃ^�C�v
#define	TYP_CHD		1		// ���^�C�v
#define	TYP_PIE		2		// ��^�C�v
#define	TYP_CCL		3		// �~�^�C�v

/*	�y���X�^�C��	*/
#define	PNS_SLD		0		// �����̃y��
#define	PNS_DSH		1		// �j���̃y��
#define	PNS_DOT		2		// �_���̃y��
#define	PNS_DSD		3		// ��_�����̃y��
#define	PNS_DDD		4		// ��_�����̃y��

/*	�u���V�X�^�C��	*/
#define	BRS_CLR		0		// �u���V�N���A
#define	BRS_SLD		1		// ���F
#define	BRS_BDI		2		// 45�x�������̃n�b�` (������E��)
#define	BRS_CRS		3		// �����Ɛ����̃N���X�n�b�`
#define	BRS_DCS		4		// 45�x�̃N���X�n�b�`
#define	BRS_FDI		5		// 45�x������̃n�b�` (������E��)
#define	BRS_HRZ		6		// �����n�b�`
#define	BRS_VRT		7		// �����n�b�`

/*	�v�����^�[	*/
#define	MAX_PNM		128		// �v�����^�[���ő咷��
#define	MAX_PPR		64		// �p�����ő咷��

/*	������	*/
#define	PDV_TXT		1		// ������
#define	PDV_RCT		2		// ��`
#define	PDV_LIN		3		// ����
#define	PDV_ARC		4		// �~��
#define	PDV_BMP		5		// �r�b�g�}�b�v
#define	PDV_BLK		6		// �u���b�N

typedef struct{					// ������f�[�^�^�C�v�\����
	char		*nam;			// ���O
	int			xps;			// �����W�i1/10mm�j
	int			yps;			// �����W�i1/10mm�j
	int			wid;			// ���i1/10mm�j
	int			hgt;			// �����i1/10mm�j
	WCHAR		*txt;			// �e�L�X�g������
	int			mod;			// �`�惂�[�h�i�Z���^�����O���j
	unsigned	fcl;			// �O�i�F
	unsigned	bcl;			// �w�i�F
	WCHAR		*fnt;			// �t�H���g��
	int			siz;			// �t�H���g�T�C�Y
	int			uln;			// �����L��
	int			cln;			// �����L��
	int			itl;			// �Α̗L��
	int			bld;			// �����T�C�Y( 0 �` 1000 )
	int			vsb;			// ��
}Txt_def;

typedef struct{					// ��`�f�[�^�^�C�v�\����
	char		*nam;			// ���O
	int			xps;			// �����W�i1/10mm�j
	int			yps;			// �����W�i1/10mm�j
	int			wid;			// ���i1/10mm�j
	int			hgt;			// �����i1/10mm�j
	int 		pns;			// �y���X�^�C��
	int			lnw;			// �����i1/10mm�j
	unsigned	fcl;			// �O�i�F
	int			brs;			// �u���V�X�^�C��
	unsigned	bcl;			// �w�i�F
	int			rnd;			// �p�ۗL��
	int			vsb;			// ��
}Rct_def;

typedef struct{					// �~�ʃf�[�^�^�C�v�\����
	char		*nam;			// ���O
	int			xps;			// ���S�����W�i1/10mm�j
	int			yps;			// ���S�����W�i1/10mm�j
	int			xrs;			// x���a�i1/10mm�j
	int			yrs;			// y���a�i1/10mm�j
	double		brd;			// �n�_�p�i���W�A���j
	double		erd;			// �I�_�p�i���W�A���j
	int			typ;			// �`��^�C�v
	int 		pns;			// �y���X�^�C��
	int			lnw;			// �����i1/10mm�j
	unsigned	fcl;			// �O�i�F
	int			brs;			// �u���V�X�^�C��
	unsigned	bcl;			// �w�i�F
	int			vsb;			// ��
}Arc_def;

typedef struct{					// ���f�[�^�^�C�v�\����
	char		*nam;			// ���O
	int			bgx;			// �n�_�����W�i1/10mm�j
	int			bgy;			// �n�_�����W�i1/10mm�j
	int			edx;			// �I�_�����W�i1/10mm�j
	int			edy;			// �I�_�����W�i1/10mm�j
	int 		pns;			// �y���X�^�C��
	int			lnw;			// �����i1/10mm�j
	unsigned	fcl;			// �O�i�F
	int			vsb;			// ��
}Lin_def;

typedef struct{			 		// �r�b�g�}�b�v�f�[�^�\����
	WCHAR			*nam;		// ���O
	WCHAR			*bmp;		// �r�b�g�}�b�v��
	int				xps;		// �����W�i1/10mm�j
	int				yps;		// �����W�i1/10mm�j
	int				wid;		// ���i1/10mm�j
	int				hgt;		// �����i1/10mm�j
	int				vsb;		// ��
}Bmp_def;

typedef struct{		 			// �u���b�N�f�[�^�^�C�v�\����
	char		*nam;			// ���O
	int			xps;			// �����W�i1/10mm�j
	int			yps;			// �����W�i1/10mm�j
	int			wid;			// ���i1/10mm�j
	int			hgt;			// �����i1/10mm�j
	int 		pns;			// �y���X�^�C��
	int			lnw;			// �����i1/10mm�j
	unsigned	fcl;			// �O�i�F
	int			brs;			// �u���V�X�^�C��
	unsigned	bcl;			// �w�i�F
	Txt_def		*txt;			// ������f�[�^
	int			txc;			// ������f�[�^��
	Rct_def		*rct;			// ��`�f�[�^
	int			rcc;			// ��`�f�[�^��
	Lin_def		*lin;			// ���f�[�^
	int			lic;			// ���f�[�^��
	Arc_def		*arc;			// �~�ʃf�[�^
	int			acc;			// �~�ʃf�[�^��
	Bmp_def		*bmp;			// �r�b�g�}�b�v�f�[�^
	int			bmc;			// �r�b�g�}�b�v�f�[�^��
	int			vsb;			// ��
}Blk_def;

typedef	struct{					// ����f�[�^�\����
	Txt_def		*txt;			// ������f�[�^
	int			txc;			// ������f�[�^��
	Rct_def		*rct;			// ��`�f�[�^
	int			rcc;			// ��`�f�[�^��
	Lin_def		*lin;			// ���f�[�^
	int			lic;			// ���f�[�^��
	Arc_def		*arc;			// �~�ʃf�[�^
	int			acc;			// �~�ʃf�[�^��
	Bmp_def		*bmp;			// �r�b�g�}�b�v�f�[�^
	int			bmc;			// �r�b�g�}�b�v�f�[�^��
	Blk_def		*blk;			// �u���b�N�f�[�^
	int			blc;			// �u���b�N�f�[�^��
}Prn_def;

typedef struct{					// �p���f�[�^�\����
	short		wid; 			// ���[�U�[��`�p����
	short		hgt; 			// ���[�U�[��`�p������
	WCHAR		*pnm;			// �p����
	short		siz; 			// �p���T�C�Y
	short		orn; 			// �p������
	WCHAR		*ttl;			// ���|�[�g�^�C�g��
}Ppr_def;

typedef struct{					// ����f�o�C�X��`
	int			xpc;			// ����\�����̈�(�ޯ�) (�f�B�X�v���C�̕�(�s�N�Z���P��))
	int			ypc;			// ����\�����̈�(�ޯ�) (�f�B�X�v���C�̍���(���X�^�s�P��))
	int			xmm;			// ����\�����̈�(mm)   (������ʂ̕� (mm))
	int			ymm;			// ����\�����̈�(mm)   (������ʂ̍��� (mm))
	int			hmg;			// �����}�[�W��(�ޯ�)
	int			vmg;			// �����}�[�W��(�ޯ�)
	float		dpp;			// �߲�ā��ޯĕϊ��W��
}Dev_inf;

typedef	struct{					// �f�t�H���g�v���r���[�X�^�C���\����
	int			lft;			// �\���ʒu�i������w)(�s�N�Z��)
	int			top;			// �\���ʒu�i������x)(�s�N�Z��)
	int			wid;			// �\����(�s�N�Z��)
	int			hit;			// �\������(�s�N�Z��)
	int			shw;			// �\�����[�h(�s�N�Z��)
	int			scl;			// �f�t�H���g�X�P�[��
	int			mbr;			// ���j���[�o�[�L��
}Prv_def;

typedef struct{					// ����f�[�^�\����
	Ppr_def		*ppr;			// �p���f�[�^�\����
	Prn_def		*prn;			// ����f�[�^�\����
	Prv_def		*prv;			// �f�t�H���g�v���r���[�X�^�C��
	Dev_inf		*dev;			// �f�o�C�X��`�\����
	HINSTANCE	ins;			// ���ع���ݲݽ�ݽ�����
	HWND		par; 			// �e�E�B���h�E�n���h��
	HWND		wnd; 			// �v���r���[�E�B���h�E�n���h��
	int			mod;			// �o�͐�
	Mtx_def		mtx[1];			// ���W�ϊ��}�g���N�X
	HDC			hdc;			// �v�����^�f�o�C�X�n���h��
}Prn_mng;

void	Get_Pdv_Inf( Prn_mng *, int );				// �v�����^�[���擾
void	Prn_Out_Dat( HDC, Prn_mng *, int, HRGN );	// ����f�[�^�o��
int		Prn_Opn( char *, Prn_mng * );				// �v�����^�[�I�[�v��
int		Prn_Exc( int, Prn_mng * );					// ������s
int		Prn_Cls( Prn_mng * );						// �v�����^�N���[�Y

#endif
