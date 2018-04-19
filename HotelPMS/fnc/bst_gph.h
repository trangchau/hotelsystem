/*
	Booking status graphic operation manipulator
*/

#ifndef	gph_dspH
#define	gph_dspH

#include "..\..\Common\bsc_def.h"
#include "..\..\Common\dte_mng.h"
#include "bst_sql.h"

// Color codes
#define	COL_STY			RGB(128,128,255)			// Stay arrow 
#define	COL_DUS			RGB(  0,208, 0)				// Day use arrow
#define	COL_ELO			RGB(255,128,128)			// Early checkin/Late checkout terminal symbole 

#define	DGG_BCL			RGB(192, 255, 192)			// Day gage background color 

// Figure layers
#define	LYR_HGG			1							// Horizon gage object
#define	LYR_VGG			2							// Vertical dage object
#define	LYR_DAT			3							// Data object
#define	LYR_ERR			4							// Error object

// Graphic unit
#define	GPH_UNT			40							// Pixcel width unit per 1 data 
#define	GPH_HIT			30							// Pixcel height unit per 1 line
#define	ARW_HIT			24							// Pixcel height unit per 1 arrow
#define	GAG_HGT			32
#define	GPH_WID(x)		(GPH_UNT*2.0*(x))			// Pixcel unit of 1 day

#define	LIN_SEP			0x8000						// �s����������
#define	is_sep_(x)		(!!((x)&LIN_SEP))			// �s����������
#define	lin_of_(x)		((x)&0x7fff)				// �������̍s���o��

typedef	struct	{									// �`��f�[�^���[�U�[�̈�
	short				mth;						// ��
	short				day;						// ��
}GAG_DAT;

typedef	struct	{									// �O���t���͊Ǘ��f�[�^
	HWND				ghd;						// ���t�ڐ��E�B���h�E�n���h��
	Lnk					grt[1];						// ���t�ڐ���
	Area				gsy[1];						// ���t�ڐ��V�X�e�����W
	Area				gdv[1];						// ���t�ڐ��f�o�C�X���W

	HWND				hnd;						// �\��O���t�E�B���h�E�n���h��
	Lnk					rot[1];						// �\��O���t��
	Area				sys[1];						// �\��O���t�V�X�e�����W
	Area				dvc[1];						// �\��O���t�f�o�C�X���W

	int					aln;						// �����o�^�ԍ�
	int					arn;						// �E���o�^�ԍ�
	int					rln;						// ����`�o�^�ԍ�
	int					rrn;						// �E��`�o�^�ԍ�

	int					idx;						// ���t�e�[�u���C���f�b�N�X
	double				coe;						// �g�嗦
	HFONT				fnt;						// �t�H���g�n���h��
	int					fhg;						// �t�H���g����
	int					rnw;						// �����ԍ���
	int					rtw;						// �����^�C�v��
	Day					std[1];						// ���ԊJ�n���t
	Day					edd[1];						// ���ԏI�����t
	int					dys;						// ���ԓ���

	HWND				par;						// �e�E�B���h�E�n���h��
	Lnk					rom[1];						// �������X�g

	Fig_mng				*fig;						// Fig_mng pointer
	PMS_MNG				*mng;						// Property Management System Management structure
}BST_MNG;

typedef	struct	{									// �O���b�h�ʒm�\����
	int					ntf;						// �ʒm�R�[�h
	int					cid;						// �R���g���[���h�c
	int					msx;						// Mouse x device coodinate
	int					msy;						// Mouse y device coodinate
	int					rii;						// Reservation id of hitting data
	int					rdi;						// Reservation detail id of hitting data
	LPVOID				usr;						// Data pointer of user definition
}GPH_NTF;

typedef	struct	{									// room and day position
	BST_ROM_LST			*rom;						// room information
	Day					osd[1];						// Use Start date( old )
	Day					oed[1];						// Use End date( old ) 
	Day					nsd[1];						// USe Start date ( new )
	Day					ned[1];						// Use End date ( new )
	Fig_elm				*elm;						// pointer of element of figure
}GPH_RDY;

#define	WM_GRAPHNOTIFY		(WM_USER+16385)			// Notify message from bst_hph to parent window

// Notify codes
#define	GPN_DRAG			1						// Drag event
#define	GPN_CREATE			2						// Create a reservation event
#define	GPN_CHANGE			3						// Change a reservation event
#define	GPN_RBUTTONDOWN		20						// Rbuttondown event

#define ON_WM_GRAPHNOTIFY() \
	{ WM_GRAPHNOTIFY, 0, 0, 0, AfxSig_lwl, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM) > \
		( &ThisClass :: OnGraphNotify)) },

typedef	void (*APL_CBK)( int, int, int, LPVOID );	// ���[�U�[�R�[���o�b�N�֐�
typedef	void (*RDN_CBK)( int, int, Dpn *, LPVOID );	// ���[�U�[�R�[���o�b�N�֐�
typedef	void (*DRG_CBK)( Dpn *, Dpn *, LPVOID );	// ���[�U�[�R�[���o�b�N�֐�

void	Chg_Mod( int, BST_MNG * );				// ���샂�[�h�ؑւ�
void	Chg_Dvc( HANDLE, Area *, Area * );			// �f�o�C�X�T�C�Y�ύX
void	Chg_Pos( HANDLE, Area * );					// �ʒu�ړ�
void	Trm_Dsp( HANDLE );							// �}�`�\���I������
void	Siz_Gph( BST_MNG *, int, int, int, int );	// �O���t�E�B���h�E�T�C�Y�ύX����
void	Ers_Bok( BST_MNG * );						// Cancellation of booking
int		Mak_Gph( BST_MNG *, HINSTANCE, HWND, int );	// �q�E�B���h�E��������
int		Rmk_Gph( RECT *, BST_MNG * );				// remake graphic data
int		Del_Gph( int, int, BST_MNG * );				// Delete graph from list ( by reservation_equipment_id or reservation_detail_id )
int		Upd_Gph( BST_ROM_STS *, BST_MNG * );		// Change the graphic element
void	Drw_Rif( CDC *, CWnd *, BST_MNG * );		// ROOM���`��
void	Dst_Gph( Csr );								// delete all status data

#endif
