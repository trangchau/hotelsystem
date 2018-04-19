/*
	�V�X�e�����ʒ�`�t�@�C��
*/
#ifndef	sys_def
#define	sys_def

#include <windows.h>
#include "bsc_def.h"
#include "trd_mng.h"
#include "lnk_mng.h"

#define	TRC_FNM	".\\Trace.log"			// �g���[�X�t�@�C����

#define	MAX_TDC			20				// �ő�X���b�h��

typedef	struct {						// ���O�Ǘ��������d�l
	WCHAR				*lgp;			// ���O�t�@�C���p�X
	int					lkd;			// ���O�ێ�����
	WCHAR				*lds;			// ���O�t�@�C���̑���
	int					mxl;			// �\���ő�s��
	WCHAR				*edt;			// �\���G�f�B�^�[
}LOG_INI;

typedef	struct {						// ���O�Ǘ�
	int					opn;			// ���O�I�[�v���t���O
	WCHAR				pth[MAX_PATH];	// ���O�i�[�p�X
	int					dsp;			// ���O�t�@�C������
	int					dys;			// �ێ�����
	int					tdn;			// ���O�X���b�h�ԍ�
	int					mxl;			// �\���ő�s��
	WCHAR				exn[MAX_PATH];	// ���s���O�t�@�C����
	WCHAR				sdn[MAX_PATH];	// ���M���O�t�@�C����
	WCHAR				rcn[MAX_PATH];	// ��M���O�t�@�C����
	WCHAR				edt[MAX_PATH];	// �\���G�f�B�^�[
	CRITICAL_SECTION	crs[1];			// �r������
}LOG_MNG;

typedef	struct {						// �V�X�e����񏉊����d�l
	int					tso;			// �X���b�h�N���^�C���A�E�g
	int					tto;			// �X���b�h�I���^�C���A�E�g
}SYS_INI;

typedef	struct	{						// �V�X�e�����
	int					typ;			// �ʐM�^�C�v
	int					tst;			// �X���b�h�J�n�^�C���A�E�g�l
	int					ted;			// �X���b�h�I���^�C���A�E�g�l
}SYS_INF;

typedef	struct	{						// �V���A���ʐM�������d�l
	int					pot;			// �|�[�g�ԍ�
	int					brt;			// �{�[���[�g
	int					dtl;			// �f�[�^��
	int					stl;			// �X�g�b�v�r�b�g��
	char				*pty;			// �p���e�B����
}CMM_INI;

typedef	struct	{						// �V���A���ʐM�ݒ���
	int					pot;			// �|�[�g�ԍ�
	int					brt;			// �{�[���[�g
	int					dtl;			// �f�[�^��
	int					stl;			// �X�g�b�v�r�b�g��
	char				pty;			// �p���e�B����
}CMM_INF;

typedef	struct	{						// �f�[�^�x�[�X�ڑ��������d�l
	WCHAR				*dsn;			// �f�[�^�Z�b�g��
	WCHAR				*sid;			// �T�[�r�XID
	WCHAR				*uid;			// ���[�U�[ID
	WCHAR				*pwd;			// �p�X���[�h
	WCHAR				*scc;			// security code
	int					cto;			// �ڑ��^�C���A�E�g�l
	int					pfl;			// �v���t�B�[����
}DBS_INI;

typedef	struct	{						// �f�[�^�x�[�X�ڑ���`
	WCHAR				dsn[MAX_PATH];	// �f�[�^�Z�b�g��
	WCHAR				sid[MAX_PATH];	// �T�[�r�XID
	WCHAR				uid[MAX_PATH];	// ���[�U�[ID
	WCHAR				pwd[MAX_PATH];	// �p�X���[�h
	int					pfl;			// �v���t�B�[����
}DBS_INF;

typedef	struct	SYS_DEF	{				// �V�X�e�����ʊǗ��\��
	SYS_INF				sys[1];			// �V�X�e�����
	LOG_MNG				log[1];			// ���O�Ǘ����
	TRD_INF				trd[MAX_TDC];	// �X���b�h�Ǘ��f�[�^
	HINSTANCE			ins;			// �A�v���P�[�V�����C���X�^���X
	HWND				wnd;			// ���C���E�B���h�E�n���h��
	HANDLE				mtx;			// ��d�N���h�~�~���[�e�b�N�X
	void				*ini;			// ���������|�C���^
	int					ver[4];			// ���g�̃o�[�W����
	WCHAR				pgn[128];		// ���g�̃v���O������
	WCHAR				ttl[128];		// �v���O�����^�C�g��
	CRITICAL_SECTION	lbx[1];			// ���O���X�g�A�N�Z�X�r������
	Lnk					lgm[1];			// ���O���b�Z�[�W���X�g
	int					abt;			// �A�{�[�g�t���O
}SYS_MNG;

#endif
	