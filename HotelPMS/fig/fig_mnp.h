/*
	�}�`�쐬�E���샋�[�`����`
*/

#ifndef	fig_mnpH
#define	fig_mnpH

//#include <windows.h>
#include <afx.h>
#include <math.h>
#include "..\..\Print\afn_cnv.h"
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\lnk_mng.h"

#define	SEL_ALL		-1				// Select all of layers

#define	COL_BLK		(0)				// Black
#define	COL_MRN		(128)			// maroon
#define	COL_GRN		(32768)			// green
#define	COL_OLV		(32896)			// olive
#define	COL_NVY		(8388608)		// naivy
#define	COL_PRP		(8388736)		// purple
#define	COL_TEL		(8421376)		// �e�B�A��
#define	COL_GRY		(8421504)		// glay
#define	COL_SVR		(12632256)		// silver
#define	COL_RED		(255)			// red
#define	COL_LIM		(65280)			// lime
#define	COL_YLW		(65535)			// yellow
#define	COL_BLU		(16711680)		// blue
#define	COL_FCS		(16711935)		// �H
#define	COL_AQA		(16776960)		// light blue
#define	COL_LGL		(12632256)		// light glay
#define	COL_DGL		(8421504)		// dark gray
#define	COL_WHT		(16777215)		// whote
#define	COL_NON		(536870911)		// none

#define	MOD_NML		0				// normal
#define	MOD_SEL		1				// selecting�i���o�[�o���h�j���[�h
#define	MOD_CRT		2				// creating�i���o�[�o���h�j���[�h
#define	MOD_MOV		3				// moving element�i�V���h�[�j���[�h
#define	MOD_CHG		4				// chainging shape of element�i�V���h�[�j���[�h
#define	MOD_CUT		5				// cutting element�i���o�[�o���h�j���[�h


#define	C_PI		3.141592654
#define	to_rad(x)	((x)*(C_PI/180.0))
#define	RDW_OFS		10				// �ĕ`��̈�̃I�t�Z�b�g�T�C�Y�i������󕪁j

#define	min_(x,y)	((x)<(y) ? (x):(y))
#define	max_(x,y)	((x)>(y) ? (x):(y))

#define	ELM_LIN		1				// �����v�f
#define	ELM_RCT		2				// ��`�v�f

typedef	void (*Drw_txt)(HDC,WCHAR *,Dpn *,Dpn *,double,int,HWND);		// ������`������֐�
typedef	void (*Mus_Cbk)( int, int, int, LPVOID );						// �}�E�X����R�[���o�b�N�֐���`
typedef	int	(*Ope_Cbk)( int, int, int, Dpn *, Dpn *, void *, LPVOID );	// �}�`����R�[���o�b�N�֐���`
typedef	void (*Drw_Cbk)( HDC, void *, Drw_txt, HWND, LPVOID );			// �}�`�`��R�[���o�b�N�֐���`

typedef struct {					//  ���W
	int				x, y;			//  ���W�l
}Ipn;

typedef struct {					//  ������
	Dpn				sp[1];			//  �n�_
	Dpn				ep[1];			//  �I�_
}Lin;

typedef	struct	{					// �̈��`���
	double	top;					// �c�����擪�ʒu
	double	lft;					// �������擪�ʒu
	double	wid;					// �̈敝
	double	hit;					// �̈捂
}Area;

typedef	struct	{					// ��`
	Dpn				sp[1];			// �n�_
	Dpn				ep[1];			// �I�_
	int				lmk;			// �����[�_�`��ԍ�
	COLORREF		lmc;			// Color of left terminal mark
	int				rmk;			// �E���[�_�`��ԍ�
	COLORREF		rmc;			// Color of right terminal mark
//	WCHAR			*txt;			// Text of displaying data
}Rct;

typedef	struct	{					// �[�_�}�[�N�Ǘ��\��
	int				bcl;			// �h��Ԃ��F
	int				pct;			// ���_��
	Dpn				pnt[1];			// ���_��
}Trm_mrk;

typedef	struct	FIG_ELM {			// �v�f�\��
	int				typ;			// �v�f���
	int				mrk;			// �Z�_�`��t������
	int				lyr;			// �}�`�K�w(���[�U�[�̈�)
	Lnk				sbl[1];			// ���w�v�f(Fig_elm�^�ŉ��w���C�����`)
	struct	{						// ��܋�`
		Dpn			sp;				// ���������W
		Dpn			ep;				// �E������W
	}srn;
	int				vis;			// ������
	int				sel;			// �I���t���O
	int				pcl;			// �y���̐F
	int				bcl;			// �u���V�̐F
	union	{						// �v�f��`
		Lin			lin;			// �����v�f
		Rct			rct;			// ��`�v�f
	}elm;
	LPVOID			usr;			// Pointer of user data
}Fig_elm;

typedef	struct	{
	HWND			par;			// �e�E�B���h�E�n���h��
	HWND			wnd;			// �q�E�B���h�E�n���h��
	Area			dvc;			// �f�o�C�X�̈�i�e�E�B���h�E��̑��Έʒu�j
	Area			vpt;			// �f�o�C�X�`��̈�i�r���[�|�[�g�j
	Area			wpt;			// ���[���h���W�̈�i�E�B���h�E�|�[�g�j
	Mtx_def			s2d[1];			// ���[���h�|�f�o�C�X�ϊ��s��
	Mtx_def			d2s[1];			// �f�o�C�X�|���[���h�ϊ��s��
	Csr				top;			// ��w�􉽍\����
	double			frt;			// �t�H���g�T�C�Y�ϊ��W��
	int				ope;			// �}�`���쏈�����s���t���O
	int				drg;			// �h���b�O���t���O
	int				rbr;			// ���o�[�o���h�\�����t���O
	int				rtp;			// ���o�[�o���h��ʁi�}�`�쐬�p�j
	int				dtl;			// ����ڍ�
	int				dir;			// �ό`�^�ؒf����
	int				sts;			// �}�E�X���
	int				dst;			// �}�E�X���(�ڍ�)
	int				drm;			// Dragging method on dagging
	Fig_elm			*fnd;			// �ڍב��쎞�Ƀq�b�g�����v�f
	struct	{
		Dpn			sp;				// �}�`�n�_�ƃ}�E�X�|�C���g�̍�
		Dpn			ep;				// �}�`�I�_�ƃ}�E�X�|�C���g�̍�
	}dif;
	Fig_elm			*hit;			// ���O�Ƀq�b�g�����v�f
	Ipn				mss;			// �I���J�n�}�E�X���W
	Ipn				mse;			// �I���I���}�E�X���W
	Fig_elm			*rht;			// Hit figure on right button down

	Mus_Cbk			mcb;			// �}�E�X����R�[���o�b�N
	Ope_Cbk			ocb;			// �}�`����R�[���o�b�N
	Drw_Cbk			dcb;			// �`�摀��R�[���o�b�N
	Lnk				mrk[1];			// �[�_�`��i�[��
	HINSTANCE		ins;			// �C���X�^���X�n���h��
	LPVOID			usr;			// ���[�U�[�f�[�^
}Fig_mng;

HWND	Fig_Ini( HINSTANCE, HWND, Area *, Area *, Area *, Csr, LPVOID );
Fig_mng	*Fig_Set_Cbk( HANDLE, Mus_Cbk, Ope_Cbk, Drw_Cbk, LPVOID );
void	Fig_Set_Swn( HANDLE, Area *, int );
void	Fig_Set_Dwn( HANDLE, Area *, Area *, Area *, int, int );
void	Fig_Rdw( HANDLE );
void	Fig_Shw( HANDLE, int );
void	Fig_Rct_Drw( HANDLE, Dpn *, Dpn * );
int		Fig_Set_Ope( HANDLE, int, int );
void	Fig_Drw_Elm( HANDLE, Fig_elm * );
int		Fig_Fnt_Siz( HANDLE, double );
int		Fig_Ent_Tmk( HANDLE, Trm_mrk * );
void	Fig_Trm( HANDLE );

#endif
