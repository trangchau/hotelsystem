/*
	�o�C�i���|�t�@�C���A�N�Z�X�֐���`
*/
#ifndef Bin_FilH
#define Bin_FilH

#define	BIN_RED		1
#define	BIN_WRT		2
#define	BIN_ERR		-1
#define	BOP_ERR		((HANDLE)(LONG_PTR)0xffffffff)

int		Bin_Red( void *, int, HANDLE );		// �u���b�N�ǂݍ���
int		Bin_Wrt( void *, int, HANDLE );		// �u���b�N��������
HANDLE	Bin_Opn( WCHAR *, int );			// �t�@�C���I�[�v��
void	Bin_Cls( HANDLE );					// �t�@�C���N���[�Y

#endif
