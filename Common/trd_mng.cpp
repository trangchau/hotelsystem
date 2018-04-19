/*
	�X���b�h�Ǘ�����
*/

#include <afx.h>
#include <windows.h>
#include "sys_def.h"
#include "trd_mng.h"
#include "cmn_fnc.h"
#include "sys_log.h"

static	int	bgn_trd(		// �X���b�h�J�n����
TRD_ENT		ent,			// �X���b�h�G���g���[�A�h���X
LPVOID		prm,			// �X���b�h�p�����[�^
TRD_CMN		*tcm,			// �X���b�h���ʍ\��
int			wat )			// �N����҂����ԁims 0=�҂��Ȃ��j
{
	int			chk;

	tcm->cst = TRS_SUC;

	chk  = !!( tcm->stt = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->cmp = ::CreateEvent( NULL, TRUE, FALSE, NULL ) );	// �����͎蓮���Z�b�g(���ŗp)
	chk &= !!( tcm->stp = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->stc = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->ctn = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->ctc = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->off = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->ofc = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->end = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->abt = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	if ( chk ){
/*
	tcm->stt = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->cmp = ::CreateEvent( NULL, TRUE, FALSE, NULL );	// �����͎蓮���Z�b�g(���ŗp)
	tcm->stp = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->stc = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->ctn = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->ctc = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->off = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->ofc = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->end = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->abt = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	if ( tcm->cmp && tcm->stp && tcm->end ){
*/
		tcm->trd = CreateThread( NULL, 0, ent, prm, 0, &tcm->tid );
		if ( !tcm->trd ){
			tcm->cst = TRS_OSE;
			tcm->oss = GetLastError();
			DBG_OUT( __FILE__, __LINE__, "CreateThread�G���[ OSERROR=%d", tcm->oss );
		}
		else{
			if ( wat ){
				switch( WaitForSingleObject( tcm->stt, wat ) ){
				case WAIT_OBJECT_0:	break;
				case WAIT_TIMEOUT:	tcm->cst = TRS_TMO;
									DBG_OUT( __FILE__, __LINE__,
										"�X���b�h�N���^�C���A�E�g�G���[" );
									break;
				default:			tcm->cst = TRS_OSE;
									tcm->oss = GetLastError();
									DBG_OUT( __FILE__, __LINE__,
										"WaitForSingleObject�G���[ OSERROR=%d", tcm->oss );
									break;
				}
			}
		}
		if ( tcm->cst != TRS_SUC ){
			if ( tcm->cmp )	CloseHandle( tcm->cmp );
			if ( tcm->stp )	CloseHandle( tcm->stp );
			if ( tcm->stc )	CloseHandle( tcm->stc );
			if ( tcm->ctn )	CloseHandle( tcm->ctn );
			if ( tcm->ctc )	CloseHandle( tcm->ctc );
			if ( tcm->off )	CloseHandle( tcm->off );
			if ( tcm->ofc )	CloseHandle( tcm->ofc );
			if ( tcm->end )	CloseHandle( tcm->end );
			if ( tcm->stt )	CloseHandle( tcm->stt );
			if ( tcm->abt )	CloseHandle( tcm->abt );
		}
	}
	else{
		tcm->cst = TRS_MEM;
		tcm->oss = GetLastError();
		DBG_OUT( __FILE__, __LINE__, "CreateEvent�G���[ OSERROR=%d", tcm->oss );
	}
	return( tcm->cst );
}

static	void	end_trd(	// �X���b�h�I��
int			tot,			// �X���b�h�I���҂��^�C���A�E�g�l(ms)
TRD_CMN		*tcm )			// �X���b�h���ʍ\��
{
	if ( tcm && tcm->trd && tcm->tid ){
		SetEvent( tcm->end );
		if ( WaitForSingleObject( tcm->cmp, tot ) == WAIT_TIMEOUT ){
			DBG_OUT( __FILE__, __LINE__, "�X���b�h�I���҂��^�C���A�E�g index=%d", tcm->tix );
			TerminateThread( tcm->trd, 0 );
		}
		if ( tcm->cmp )	CloseHandle( tcm->cmp );
		if ( tcm->stp )	CloseHandle( tcm->stp );
		if ( tcm->stc )	CloseHandle( tcm->stc );
		if ( tcm->ctn )	CloseHandle( tcm->ctn );
		if ( tcm->ctc )	CloseHandle( tcm->ctc );
		if ( tcm->off )	CloseHandle( tcm->off );
		if ( tcm->ofc )	CloseHandle( tcm->ofc );
		if ( tcm->end )	CloseHandle( tcm->end );
		if ( tcm->stt )	CloseHandle( tcm->stt );
		if ( tcm->abt )	CloseHandle( tcm->abt );
		CloseHandle( tcm->trd );
	}
}

int		Trd_Stt(				// �X���b�h�J�n����
TST_PRM		*tst,				// �N���X���b�h���
int			src,				// �N�����X���b�h�ԍ�
void		*mnp,				// �V�X�e���Ǘ����
void		*usr )				// ���[�U�[��`�\��
{
	int			cst;
	SYS_MNG		*mng=(SYS_MNG *)mnp;
	TRD_INF		*trd=mng->trd+tst->tdn;

	trd->prm = calloc( 1, tst->siz );
	trd->tcm->ctf = tst->rcv;

	if ( trd->prm ){
		if ( ( cst = tst->stp( trd->prm, mng, trd->tcm, usr ) ) == TRS_SUC ){
			WStr_NCpy( trd->nam, tst->nam, sizeof(trd->nam)/wsizeof(1) );
			if ( bgn_trd( tst->ent, trd->prm, trd->tcm, mng->sys->tst ) == TRS_SUC ){
				cst = trd->tcm->cst;
				trd->atr = 0;
				trd->tcm->tix = tst->tdn;
				trd->psz = tst->siz;
				DBG_OUT( __FILE__, __LINE__, "TDN=%-2d Name=%s TID=0x%x",
						trd->tcm->tix, tst->nam, trd->tcm->tid );
			}
			else{
				*trd->nam = '\0';
				Sys_Log( src, LOG_ERR, mng, 0,
					_T("%s�X���b�h�N�����s cst=%d ose=%d "),
							tst->nam, trd->tcm->cst, trd->tcm->oss ); 
				cst = trd->tcm->cst;
				free( trd->prm );
				trd->prm = NULL;
			}
		}
	}
	return( cst );
}

void	Trd_End(				// �X���b�h�I������
int			tdn,				// ��~�X���b�h�ԍ�
int			siz,				// �p�����[�^�T�C�Y
CST_TPM		cst,				// �p�����[�^�j���֐�
int			src,				// ��~���X���b�h�ԍ�
void		*mnp )				// �V�X�e���Ǘ����
{
	SYS_MNG		*mng=(SYS_MNG *)mnp;
	TRD_INF		*trd=mng->trd+tdn;

	end_trd( mng->sys->ted, trd->tcm );
	if ( cst && trd->prm ){
		cst( trd->prm );
		free( trd->prm );
	}
	memset( trd, 0, sizeof(TRD_INF) );
}
