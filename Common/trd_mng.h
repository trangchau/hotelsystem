/*
	�X���b�h�Ǘ�����
*/

#ifndef	trd_mngH
#define	trd_mngH

#include "sys_def.h"

#define	TRD_NON		0				// �X���b�h�s��
#define	TRD_SPD		1				// �T�X�y���h
#define	TRD_RUN		2				// ���s��

typedef	struct	{					// �X���b�h�Ǘ����ʍ���
	HANDLE				trd;		// �X���b�h�n���h��
	DWORD				tid;		// �X���b�h�h�c
	HANDLE				stt;		// �X���b�h�N�������C�x���g
	HANDLE				stp;		// ��~�w�߃C�x���g
	HANDLE				stc;		// ��~�����C�x���g
	HANDLE				ctn;		// �ĊJ�w�߃C�x���g
	HANDLE				ctc;		// �ĊJ�����C�x���g
	HANDLE				off;		// �d���I�t�C�x���g
	HANDLE				ofc;		// �d���I�t�����C�x���g
	HANDLE				end;		// �I���w�߃C�x���g
	HANDLE				cmp;		// �����ʒm�C�x���g
	HANDLE				abt;		// �ً}��~�ʒm�C�x���g
	int					cst;		// �����X�e�[�^�X
	int					oss;		// �I�؃��[�e�B���O�V�X�e���X�e�[�^�X
	int					sts;		// ���ݏ��
	int					tix;		// �X���b�h�C���f�b�N�X
	int					ctf;		// �p������t���O
}TRD_CMN;

typedef	int		(*SET_TPM)( void *, void *, TRD_CMN *, void * );	// �X���b�h�p�����[�^�ݒ�֐�
typedef	void	(*CST_TPM)( void * );								// �X���b�h�p�����[�^�j���֐�

typedef	DWORD (WINAPI *TRD_ENT)( LPVOID );

typedef	struct	{				// �X���b�h�J�n�p�����[�^
	int			tdn;			// �X���b�h�ԍ�
	TRD_ENT		ent;			// �G���g���[�A�h���X
	WCHAR		*nam;			// �X���b�h��
	SET_TPM		stp;			// �p�����[�^�ݒ�֐�
	int			siz;			// �p�����[�^�T�C�Y
	int			rcv;			// �p���^�]�t���O
}TST_PRM;

typedef	struct	{					// �X���b�h�Ǘ�
	TRD_CMN				tcm[1];		// �X���b�h�p�����[�^�i���ʕ��j�|�C���^
	WCHAR				nam[11];	// �X���b�h����
	void				*prm;		// �X���b�h�p�����[�^
	int					psz;		// �p�����[�^�T�C�Y
	DWORD				atr;		// �X���b�h����
	int					sub;		// ����X���b�h�}��
	int					nxt;		// ����X���b�h�`�F�C��
}TRD_INF;

int		Trd_Stt( TST_PRM *, int, void *, void * );				// �X���b�h�J�n����
void	Trd_End( int, int, CST_TPM, int, void * );				// �X���b�h�I������

#endif
