/*
	�e�L�X�g�t�@�C���A�N�Z�X�֐���`
*/

#ifndef Txt_FilH
#define Txt_FilH

#define	TXT_RED		1
#define	TXT_WRT		2
#define	TXT_APD		3
#define	TOP_ERR		((HANDLE)(LONG_PTR)0xffffffff)

int		Txt_Red( WCHAR *, int, HANDLE );	// �P�s�ǂݍ���
int		Txt_Wrt( WCHAR *, int, HANDLE );	// �P�s��������
HANDLE	Txt_Opn( WCHAR *, int );			// �t�@�C���I�[�v��
void	Txt_Cls( HANDLE );					// �t�@�C���N���[�Y

#endif
