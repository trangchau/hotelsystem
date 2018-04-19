/*
	�q�r�|�Q�R�Q�b �ʐM���[�`��
*/

#ifndef	COM_MNGH
#define	COM_MNGH

// �ʐM����R�[�h
#define	ENQ			0x5
#define	ACK			0x6
#define	NAK			0x15
#define	STX			0x2
#define	ETX			0x3
#define	EOT			0x4
#define	DLE			0x10
#define	DC1			0x11
#define	DC4			0x14
#define	CR			0xd

// �ʐM�G���[�X�e�[�^�X
#define	CER_NON		0					// �G���[����
#define	CER_OVR		1					// �I�[�o�[�����G���[
#define	CER_PTY		2					// �p���e�B�G���[
#define	CER_FRM		3					// �t���[�~���O�G���[
#define	CER_BRK		4					// �u���[�N�L�����N�^�[���o
#define	CER_IOE		5					// �h�^�n�G���[
#define	CER_RBO		6					// ��M�o�b�t�@�I�[�o�[�t���[
#define	CER_SBO		7					// ���M�o�b�t�@�I�[�o�[�t���[
#define	CER_MOD		8					// ���[�h�G���[

#define	CER_OPN		20					// �I�[�v������Ă��Ȃ��|�[�g�̃A�N�Z�X
#define	CER_TOT		50					// �^�C���A�E�g�G���[
#define	CER_MEM		60					// �������[�s���G���[
#define	CER_NDT		61					// �f�[�^�����G���[

#define	CER_UNW		99					// �s���ȃG���[

// �ʐM�C�x���g�e�[�^�X
#define	CEV_RCV		1					// ��M�����C�x���g
#define	CEV_SND		2					// ���M�����C�x���g
#define	CEV_ERR		3					// �G���[�����C�x���g

typedef	struct	{						// �ʐM���e��`
	int		pot;						// �ʐM�|�[�g�ԍ�(1,2,3,...)
	int		brd;						// �{�[���[�g(19200,9600,4800,...)
	int		stp;						// �X�g�b�v�r�b�g��(1=1, 15=1.5, 2=2)
	int		dtl;						// �f�[�^�r�b�g��(5,6,7,8)
	char	pty;						// �p���e�B(E=Even,O=Odd,M=Mark,N=None)
	int		rto;						// ��M�^�C���A�E�g�l(ms)
	int		sto;						// ���M�^�C���A�E�g�l(ms)
	int		(*evt)(int,int,int,void *);	// �C�x���g�n���h��(�R�[���o�b�N)
}Com_def;

// ������`
#define	RRQ_MAX			128				// ��M�v���L���[�ő吔
#define	RQU_MAX			128				// ��M�L���[�ő吔
#define	SQU_MAX			128				// ���M�L���[�ő吔

#define	SRQ_EVT			0				// ��M�v���C�x���g�C���f�b�N�X
#define	SED_EVT			1				// ��M�X���b�h�I���v���C�x���g�C���f�b�N�X

#define	RRQ_EVT			0				// ���M�v���C�x���g�C���f�b�N�X
#define	RED_EVT			1				// ���M�X���b�h�I���v���C�x���g�C���f�b�N�X

#define	EED_EVT			0				// �G���[�Ď��X���b�h�I���v���C�x���g�C���f�b�N�X

#define	TRE_RTY			10				// �X���b�h�I���Ď��ő僊�g���C��

typedef	struct	{						// �ʐM���e�f�[�^
	int		siz;						// �f�[�^�T�C�Y
	int		tim;						// �^�C���A�E�g�l
	char	ecr;						// ��M�I�[�L�����N�^
	char	dat[1];						// ��M�f�[�^
}Com_dat;

typedef	struct	{				// �����O�o�b�t�@�Ǘ��@�\
	int		red;						// �Ǐo�|�C���^�[
	int		wrt;						// �����|�C���^�[
	int		max;						// �ő�o�b�t�@��
	int		siz;						// �P�I�u�W�F�N�g�̃T�C�Y
	void	*buf;						// �f�[�^�|�C���^
}Rng_buf;

#define	Cbf_Siz(x)	(sizeof(Com_dat) + (x) - 1)

typedef	struct	{				// �ʐM����f�[�^������`
	Com_def				def;			// ���[�U�[��`
	CRITICAL_SECTION	rra;			// ��M�v����������p��è�پ����
	CRITICAL_SECTION	rqa;			// ��M��������p��è�پ����
	CRITICAL_SECTION	sqa;			// ���M��������p��è�پ����

	HANDLE				hnd;			// �ʐM�|�[�g�̃n���h��
	OVERLAPPED			sov;			// ���d�h�^�n����(���M)
	OVERLAPPED			rov;			// ���d�h�^�n����(��M)

	HANDLE				rtd;			// ��M�X���b�h�n���h��
	HANDLE				std;			// ���M�X���b�h�n���h��
	int					rid;			// ��M�X���b�h�h�c
	int					sid;			// ���M�X���b�h�h�c
	int					rtm;			// ��M�X���b�h�I��
	int					stm;			// ���M�X���b�h�I��

	HANDLE				rev[2];			// ��M�v���E�X���b�h�I���v���C�x���g
	HANDLE				sev[2];			// ���M�v���E�X���b�h�I���v���C�x���g

	Rng_buf				rrb;			// ��M�v�������O�o�b�t�@
	Rng_buf				rbf;			// ��M�����O�o�b�t�@
	Rng_buf				sbf;			// ���M�����O�o�b�t�@

	void				*usr;			// ���[�U�[��`�f�[�^

	int					end;			// �I���t���O
}Com_ctl;

HANDLE	Com_Opn( Com_def *, void *usr );		// �ʐM����̃I�[�v��
void	Com_Cls( HANDLE );						// �ʐM����̃N���[�Y
int		Com_Snd( HANDLE, char *, int, int );	// �f�[�^�̑��M
int		Com_Rcv( HANDLE, char, int, int );		// �f�[�^�̎�M�v��
int		Com_Get( void *, HANDLE );				// ��M�f�[�^�̎擾
void	Com_Rts( HANDLE, int );					// �q�s�r�̑���
void	Com_Dtr( HANDLE, int );					// �c�s�q�̑���
char	*Edt_Cmf( char *, char * );				// �ʐM�t���[�������O�p�ɕҏW
char	*Get_Cen( int );						// �ʐM�G���[���b�Z�[�W�擾

#endif
