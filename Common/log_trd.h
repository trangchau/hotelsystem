/*
	���O�X���b�h������`
*/

#ifndef	Log_TrdH
#define	Log_TrdH

#include	"bsc_def.h"
#include	"rng_buf.h"

#define	LGQ_CNT		64				// ���O�L���[��

#define	LDS_DEL		1				// ����폜
#define	LDS_KEP		2				// ���O�t�@�C���ێ�

typedef	struct	{					// ���C���X���b�h�p�����[�^
	TRD_CMN				*tcm;		// �X���b�h���ʒ�`
	SYS_MNG				*mng;		// �V�X�e���Ǘ����
	RBF_HDL				que;		// ���O�v���L���[
	int					tdn;		// �X���b�h�ԍ�
}LOG_PRM;

typedef	struct	{					// ���O�L���[�\��
	int					mdn;		// ���W���[���ԍ�
	WCHAR				nam[11];	// �X���b�h����
	int					knd;		// ���O���
	int					usr;		// ���[�U�[��`���
	WCHAR				*msg;		// ���O���b�Z�[�W
	Day					dte[1];		// ��������
}LOG_QUE;

#include	"sys_log.h"

int		Bgn_Log_Trd( int, int, SYS_MNG * );				// ���O�X���b�h�J�n
void	End_Log_Trd( int, int, SYS_MNG * );				// ���O�X���b�h�I��

#endif
