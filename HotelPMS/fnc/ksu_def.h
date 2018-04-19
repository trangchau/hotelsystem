/*------------------------------------------
    �H�����̓v���O���� for Windows Ver 5.0
		Copyright 2001 S.Nagaoka
			�e���`�t�@�C��
--------------------------------------------*/
#ifndef	ksu_defH
#define	ksu_defH

#include "..\cmn\bsc_def.h"
#include "..\cmn\lnk_mng.h"
#include "..\cmn\dbs_def.h"
#include "ksu_ini.h"

#define	INI_FNM		".\\kosu.ini"
#define	PSL_FNM		".\\Pslset.ini"
#define	MTH_FNM		".\\%02d%02d%04d.kdt"

#define	NAM_PST		"Personal-Setting Data Arsnova Corp. Allrights Reserved"

#define	GetMainFrm()	((CMainFrame *)((CKosuApp *)AfxGetApp())->m_pMainWnd)

typedef	struct	{					// �`�摮����`
	HFONT		hFont;				// �t�H���g�n���h��
	int			fhg;				// �t�H���g����
	HBRUSH		mbs;				// ��E�B���h�E�F�u���V
}DrawAttr;

#define	MAN_WIN	0					// ���C���E�B���h�E���
#define	KSU_WIN	1					// �H���E�B���h�E���
#define	KIN_WIN	2					// �ΑӃE�B���h�E���
#define	GPH_WIN	3					// �O���t�E�B���h�E���

typedef	struct	{					// �E�B���h�E���
	int			psx;				// �w���W
	int			psy;				// �x���W
	int			wid;				// ��
	int			hgt;				// ��
	int			mxd;				// �ő剻
}Win_inf;

typedef	struct	{					// �l�ݒ���
	int			red;				// �ǂݍ��ݍ�
	Win_inf		wif[4];				// �e�E�B���h�E���
	int			wop[4];				// �E�B���h�E�I�[�v�����
	int			yer;				// �Ō�ɕҏW�����N
	int			mth;				// �Ō�ɕҏW������
	int			fsz;				// �t�H���g�T�C�Y
	TCHAR		fnm[LF_FACESIZE];	// �t�H���g��
	int			zom;				// �Αӕ\�\���k�ڗ�
	int			ksz;				// �H���\�\���k�ڗ�
	Win_inf		kpw[1];				// �H���\�E�B���h�E���
	int			rsv[94];			// �\���̈�
}Psl_set;

typedef	struct	{					// �H�����ԃO���t�f�[�^
	int			day;				// ���t�C���f�b�N�X
	int			ser;				// �Ǘ��V���A���ԍ�
	int			bgn;				// �J�n����(��Ύ���)
	int			tim;				// ����(��������)
}Ksu_gph;

typedef	struct	{					// �H������
	JobDtl		*dtl;				// �i�n�a���׃|�C���^
	int			jno;				// �i�n�a�ԍ�
	int			sub;				// �T�u�R�[�h
	int			jdv;				// �i�n�a�敪
	JobDiv		*jdp;				// �i�n�a�敪�|�C���^
	int			ser;				// �Ǘ��V���A���ԍ�
	char		div[2];				// �����敪
	int			tim[33];			// �H���i�������ԁj�c���v�܂�
}Ksu_dtl;

typedef	struct	{					// �ΑӃf�[�^
	int			stt[31];			// �J�n����
	int			edt[31];			// �I������
	int			inr[31];			// �莞�ԓ�
	int			otr[31];			// �莞�ԊO
	int			mnt[31];			// �[����
	int			hwk[31];			// ���E�j�����
	int			hol[31];			// �x���敪
	int			pnl[31];			// �y�i���e�B���
	int			rat[31];			// �x������
	int			ery[31];			// ���ގ���
	int			flp[31];			// �U�֋x�������t
	int			ser;				// �Ǘ��ő�V���A���ԍ�
}Kin_dat;

typedef	struct	{					// �H���f�[�^
	int			eno;				// �Ј��ԍ�
	int			yer;				// �N
	int			mth;				// ��
	int			day;				// ��(index)
	int			cls;				// ���ߓ�
	int			dct;				// ����
	int			dys[31];			// ��
	int			knd[31];			// �j��
	Kin_dat		kin[1];				// �ΑӃf�[�^
	int			ttl[32];			// �����v�i�����v�܂ށj
	Lnk			dtl[1];				// �H������
	Lnk			gph[1];				// �O���t�f�[�^
}Mth_dat;

typedef	struct	{					// �S�̊Ǘ��f�[�^
	int				svr;			// �T�[�o�[�ڑ���
	int				cdy;			// ���ߓ�(-1�͖����j
	Day				now[1];			// ���ݓ��t
	Lnk				cdr[1];			// �J�����_�}�X�^�f�[�^
	Lnk				els[1];			// �]�ƈ��}�X�^�f�[�^
	Lnk				prj[1];			// �v���W�F�N�g�}�X�^
	Lnk				job[1];			// �i�n�a�}�X�^
	Lnk				jdv[1];			// �i�n�a�敪�}�X�^
	Lnk				div[1];			// �����}�X�^
	Lnk				grp[1];			// �O���[�v�}�X�^
	Empl			epl[1];			// ���O�C���]�ƈ�
	Sysinf			sys[1];			// �V�X�e�����
	Lnk				brk[1];			// �x�e���ԃe�[�u��
	Mth_dat			mdt[1];			// �P�������f�[�^
	Psl_set			psl[1];			// �l�ݒ���
	Ini_fil			*ini;			// �������f�[�^
	int				cwi;			// �����E�B���h�E�ԍ�
	DrawAttr		atr[1];			// �`�摮��
}Mng_dat;

#define	FNT_SIZ	12					// �t�H���g�T�C�Y
#define	FNT_NAM	"�l�r �S�V�b�N"		// �t�H���g��
#define	KIN_ZOM	50					// �Αӕ\�\���k�ڋK��l
#define	KZM_MIN	10					// �Αӕ\�\���k�ڍŏ��l
#define	KZM_MAX	150					// �Αӕ\�\���k�ڍő�l

#define	CLR_MAN	RGB(113,153,193)	// ���C���E�B���h�E�F

#define	CLR_NML	RGB(255,255,255)	// �����`��F
#define	CLR_HOL	RGB(189,204,253)	// �x���`��F
#define	CLR_SUN	RGB(252,154,156)	// ���j�`��F
#define	CLR_SEL	RGB(214,248,243)	// �I���Z���`��F

#define	JNM_LEN	60					// �i�n�a���̒���

#define	MSG_LOGIN	(WM_USER+5000)

#endif
