/*
	���������ʐݒ�t�@���N�V������`
*/

#ifndef	set_iniH
#define	set_iniH

#define	IST_SUC		0			// ����������
#define	IST_MEM		1			// �������[�s��
#define	IST_OSE		2			// �n�^�r�G���[
#define	IST_INI		3			// �����ݒ�t�@�C���̌��
#define	IST_LGO		4			// ���O�I�[�v���G���[

int		Set_Cmm( CMM_INF *, CMM_INI * );			// �ʐM����������ݒ�
int		Set_Sys( SYS_INF *, SYS_INI * );			// �V�X�e���\��������
int		Set_Log( LOG_MNG *, LOG_INI *, char * );	// ���O�Ǘ�������
int		Set_Dbs( DBS_INF *, DBS_INI *, char * );	// �f�[�^�x�[�X�ڑ�������
int		Get_Pwd( char *, char *, int, char * );		// �p�X���[�h�擾

#endif
