/*
	�����O�o�b�t�@�Ǘ�
*/

#ifndef	rng_bufH
#define	rng_bufH

typedef	DWORD	RBF_HDL;		// �����O�o�b�t�@�n���h��

RBF_HDL	Cre_Rbf( DWORD, int, int );			// �����O�o�b�t�@����(��R�����͕ۑ��̗v�� TRUE/FALSE�j
void	Cst_Rbf( RBF_HDL );					// �����O�o�b�t�@�j��
bool	Wrt_Rbf( void *, RBF_HDL );			// �����O�o�b�t�@��������
bool	Red_Rbf( void *, RBF_HDL );			// �����O�o�b�t�@�ǂݍ���
bool	Ref_Rbf( void *, int, RBF_HDL );	// �����O�o�b�t�@�Q��(�f�L���[�����̎擾)
int		Cnt_Rbf( RBF_HDL );					// �o�b�t�@�����O���擾
HANDLE	Rbf_Smp( RBF_HDL );					// �Z�}�t�H�n���h���擾
int		Sav_Rbf( WCHAR * );					// �����O�o�b�t�@�ۑ�
int		Rst_Rbf( WCHAR * );					// �����O�o�b�t�@����
RBF_HDL	Get_Rbf( RBF_HDL );					// ���������O�o�b�t�@�擾
RBF_HDL	Xch_Rbf( RBF_HDL, RBF_HDL );		// �����O�o�b�t�@����ւ��擾
void	Rbf_Prg( void );					// ���������O�o�b�t�@�j��

#endif
