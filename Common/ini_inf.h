/*
		�������t�@�C���A�N�Z�X�֐���`
*/
#ifndef Ini_InfH
#define Ini_InfH

//#include <windows.h>
#include "bsc_def.h"

#define	ITP_INT		1			// ����
#define	ITP_STR		2			// ������

#define	VAL_SUC		0			// ����I��
#define	VAL_NON		1			// �l�Ȃ�
#define	VAL_MEM		2			// �������[�s��

typedef	struct {				// �i�[�e�[�u����`
	WCHAR	*sct;				// �Z�N�V����������
	WCHAR	*key;				// �L�[������
	int		mcl;				// �����J������񂠂�
	int		mlt;				// �����s��񂠂�
	char	typ;				// �L�[�̑���
	void	*val;				// �i�[�ϐ��̈ʒu
}Ini_inf;

int		Get_Ini( Ini_inf *, int, WCHAR * );		// �v���O�������̎�荞��
int		Set_Ini( Ini_inf *, int, WCHAR * );		// �v���O�������̏�������
void	Cst_Ini( Ini_inf *, int );				// �v���O�������̔j��

#endif
