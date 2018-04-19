/*
	Figure window operation processing functions
*/

#ifndef	fig_opeH
#define	fig_opeH

#include "fig_mnp.h"

//-- ����i��
#define	OPE_CRT		1				// ��������
#define	OPE_MOV		2				// �ړ�����
#define	OPE_CHG		4				// �ό`����
#define	OPE_CUT		8				// �ؒf����

//-- ����C�x���g
#define	EVT_SLS		10				// �͈͑I���J�n
#define	EVT_SLE		11				// �͈͑I������
#define	EVT_CRS		12				// �쐬�J�n
#define	EVT_CRE		13				// �쐬����
#define	EVT_CRI		14				// �쐬�����O
#define	EVT_CGS		15				// �ό`�J�n
#define	EVT_CGI		16				// �ό`�����O
#define	EVT_CGE		17				// �ό`����F
#define	EVT_CTS		18				// �ؒf�J�n
#define	EVT_CTI		19				// �ؒf�����O
#define	EVT_CTE		20				// �ؒf����
#define	EVT_MVS		21				// �ړ��J�n
#define	EVT_MVI		22				// �ړ������O
#define	EVT_MVE		23				// �ړ�����
#define	EVT_HIT		24				// �P��v�f�q�b�g

#define	EVT_DRG		25				// Dragging the mouse
#define	EVT_RDN		26				// Right button down on the mouse

#define	SID_LFT		0x100			// ��
#define	SID_RIT		0x200 			// �E
#define	SID_UPR		0x400 			// ��
#define	SID_LWR		0x800 			// ��
#define	SID_SP		0x1000			// �n�_
#define	SID_EP		0x2000			// �I�_

//-- ����ڍ�
#define	DTL_NON		0				// �ό`�w��Ȃ�
#define	CHG_TOP		1				// ���_�ό`
#define	CHG_SID		2				// �ӕό`
#define	CHG_CUT		3				// �ؒf

//-- �ؒf����
#define	CUT_HRZ		1				// ��������
#define	CUT_VRT		2				// ��������

#define	CHG_TOP		1			// ���_�ό`
#define	CHG_SID		2			// �ӕό`
#define	CHG_CUT		3			// �ؒf�ό`

#define	cur_set_(x)	((LONG)(LONG_PTR)LoadCursor(mng->ins,MAKEINTRESOURCE(x)))

Fig_elm	*Hit_Tst( Fig_mng * );						// �}�`�q�b�g����
int		Ope_Stt( Fig_mng * );						// ����J�n�C�x���g����
int		Ope_End( double, Fig_mng * );				// ����I���C�x���g����
int		Ope_Drg( int, int, int, int,  Fig_mng * );	// ����I���C�x���g����
void	Set_Ope(  Fig_mng *, int, int, int );		// �ڍב��샂�[�h�ݒ�
void	Rdn_Ope(  Fig_mng *, int, int, int );

#endif
