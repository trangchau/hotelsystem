/*
	�}�`�쐬�E���샋�[�`��
	�Z�_�}�[�N�����֐�
*/

#ifndef	fig_mrkH
#define	fig_mrkH

#include "fig_mnp.h"

//-- �}�[�N�ǉ��w��
#define	MRK_LFT		1				// �����}�[�N�t���w��(���n�_)
#define	MRK_RIT		2				// �E���}�[�N�t���w��(���I�_)
#define	MRK_TOP		4				// �㑤�}�[�N�t���w��
#define	MRK_BTM		8				// �����}�[�N�t���w��

#define	MRK_DEF		0				// �V�X�e���K��Z�_�`��

#define	tmk_siz(x)	(sizeof(Trm_mrk)+sizeof(Dpn)*((x)-1))
#define	nxt_tmk(x)	((Trm_mrk *)(((char *)x)+tmk_siz((x)->pct)))

int		Add_Mrk( void *, Fig_elm *, Fig_mng * );	// �Z�_�`��t��
int		Ent_Mrk( Trm_mrk *, Fig_mng * );			// ���[�U�[��`�[�_�`��o�^
double	Mrk_Siz( Mtx_def * );						// �����p�}�[�N�T�C�Y�̎擾

#endif
