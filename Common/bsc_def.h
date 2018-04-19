/*
	��{�f�[�^��`�t�@�C��
*/
#ifndef	bsc_defH
#define	bsc_defH

typedef	struct	{					// ���t�t�B�[���h
	short		yer;				// �N
	short		mth;				// ��
	short		day;				// ��
	short		hor;				// ��
	short		min;				// ��
	short		sec;				// �b
}Day;

#define		memcnt(x)		((sizeof(x))/(sizeof(x[0])))	// �z����擾
#define		Csr_Of(x)		(((Csr)(x))-1)					// �J�[�\���Z�o
#define		is_kanji(x)	(((x)>=0x80&&(x)<=0xa0)||(x)>=0xe0)
#define		to_dgt(x)	((x)>'9' ? (((x)-0x41)+10):((x)-'0'))
#define		is_num(x)	((x)>='0' && (x)<='9')
#define		is_alp(x)	((x)>='A' && (x)<='Z')

#define		wsizeof(x)	((x)*sizeof(WCHAR))

#define		WM_USER_CLOSE			(WM_USER+8192+99)

char	*Han_Zen( unsigned char *, unsigned char * );	// ���p���S�p�ϊ�
int		Zen_Han( unsigned char *, unsigned char * );	// �S�p�����p�ϊ�

// �X���b�h�֌W
#define	TRS_SUC		1			// ����I��
#define	TRS_MEM		2			// �������[�s��
#define	TRS_PRM		3			// �p�����[�^�ُ�
#define	TRS_USR		4			// ���[�U�[�L�����Z��
#define	TRS_OSE		5			// �I�؃��[�e�B���O�V�X�e��(API)�G���[
#define	TRS_TMO		6			// �N���҂��^�C���A�E�g

#define	LDS_DEL		1			// ����폜
#define	LDS_KEP		2			// ���O�t�@�C���ێ�

#endif
