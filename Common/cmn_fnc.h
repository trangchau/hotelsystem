/*
	�e��G���֐��Q
*/

#ifndef	cmn_fncH
#define	cmn_fncH

#include "rng_buf.h"

#define	MKD_WRN						1		// �x�����b�Z�[�W
#define	MKD_FTL						2		// �G���[���b�Z�[�W
#define	MKD_INF						3		// �ē����b�Z�[�W

#define	CEDT_ROUND_UP				1
#define	CEDT_ROUND_TRUNCATION		2

//void	Get_Vsn( HWND, GAZS_MNG * );								// �v���O�����o�[�W�����̎擾
void	DBG_OUT( char *, int, char *, ... );						// �f�o�b�O�o��
int		Fil_Exs( char * );											// �t�@�C�����݌���
void	WStr_NCpy( WCHAR *, WCHAR *, int );							// �ő�T�C�Y�w��̕����񕡎�
WCHAR	*Add_Str( WCHAR *, WCHAR * );								// ������ǉ�
int		xtoi( char * );												// �P�U�i��������o�C�i���[�ϊ�
WORD	Bin_BCD( int );												// �o�C�i����BCD�ϊ�
int		BCD_Bin( WORD );											// BCD���o�C�i���ϊ�
WCHAR	*trim( WCHAR * );											// ������g���~���O
void	Get_Dir( char * );											// �f�B���N�g�������̂ݎ��o��
int		Str_UCmp( char *, char * );									// ������̑啶��������������r
double	Rnd_Dec( double, int, int );								// rounding to ordering digit
WCHAR	*Cma_Edt( double, int );									// �R���}�ҏW
WCHAR	*Prg_Cma( WCHAR *);											// �R���}���폜

#endif
